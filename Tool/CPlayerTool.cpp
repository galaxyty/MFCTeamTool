// CPlayerTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "Include.h"
#include "afxdialogex.h"
#include "CPlayerTool.h"
#include "CDH_FilePath.h"
#include "CTextureMgr.h"

// CPlayerTool 대화 상자

IMPLEMENT_DYNAMIC(CPlayerTool, CDialog)

CPlayerTool::CPlayerTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPlayerTool, pParent), m_iOffsetX(-40), m_iOffsetY(-20), m_PlayerState(), m_CurrentSelection(-1)
	, m_Play(true)
{
}

CPlayerTool::~CPlayerTool()
{
}

void CPlayerTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_PathList);
	DDX_Control(pDX, IDC_LIST4, m_ImageInfo);
	DDX_Control(pDX, IDC_LIST5, m_FrameList);
	DDX_Control(pDX, IDC_PICTURE, m_Picture01);
	DDX_Control(pDX, IDC_EDIT1, m_Delay);
}


BEGIN_MESSAGE_MAP(CPlayerTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST3, &CPlayerTool::OnPathList)
	ON_BN_CLICKED(IDC_BUTTON12, &CPlayerTool::OnPathSave)
	ON_BN_CLICKED(IDC_BUTTON13, &CPlayerTool::OnPathLoad)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &CPlayerTool::OnClickAnimation)
	ON_LBN_SELCHANGE(IDC_LIST5, &CPlayerTool::OnFrameList)
	ON_BN_CLICKED(IDC_BUTTON10, &CPlayerTool::OnMoveRight)
	ON_BN_CLICKED(IDC_BUTTON9, &CPlayerTool::OnMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON14, &CPlayerTool::OnMoveUp)
	ON_BN_CLICKED(IDC_BUTTON11, &CPlayerTool::OnMoveDonw)
	ON_BN_CLICKED(IDC_BUTTON1, &CPlayerTool::OnPlayAnimation)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &CPlayerTool::OnStopAnimation)
END_MESSAGE_MAP()

BOOL CPlayerTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strDelay;
	strDelay.Format(_T("%d"), 120);
	m_Delay.SetWindowText(strDelay);

	SetTimer(1, 120, NULL);


	if (FAILED(CTextureMgr::Get_Instance()->Read_ImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"못 불러온 이미지가 있습니다.");
		return E_FAIL;
	}

	m_ImageInfo.ModifyStyle(0, LVS_REPORT);
	m_ImageInfo.InsertColumn(0, _T("이름"), LVCFMT_LEFT, 100);
	m_ImageInfo.InsertColumn(1, _T("크기"), LVCFMT_LEFT, 100);



	wifstream		fin;
	fin.open(L"../Data/ImgPath.txt", ios::in);
	if (!fin.fail())
	{
		TCHAR		FilePath[MAX_PATH] = L"";

		fin.getline(FilePath, MAX_PATH, '|');
		fin.getline(FilePath, MAX_PATH, '|');
		fin.getline(FilePath, MAX_PATH, '|');
		fin.getline(FilePath, MAX_PATH);


		PathRemoveFileSpec(FilePath);
		PathRemoveFileSpec(FilePath);
		m_PlayerPath = FilePath;
		DisplayFolder(FilePath);
	}

	OnPathLoad();

	m_Picture01.GetWindowRect(&m_Rect); // 초기 위치 저장
	ScreenToClient(&m_Rect);

	return TRUE; 
}


void CPlayerTool::OnPathList()
{
}

//시작 경로 표시하기
void CPlayerTool::DisplayFolder(wstring FilePath)
{
	// 리스트 컨트롤 가져오기
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST4);

	// 기존 항목 초기화
	pListCtrl->DeleteAllItems();

	CFileFind finder;
	wstring searchPath = FilePath + L"\\*.*"; // 경로 내 모든 파일 검색
	BOOL bWorking = finder.FindFile(searchPath.c_str());

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// . 및 .. 디렉터리 무시
		if (finder.IsDots())
			continue;

		CString name = finder.GetFileName(); // 파일 또는 폴더 이름
		CString size;

		// 폴더 확인
		if (finder.IsDirectory())
		{
			size = _T("<Folder>"); // 폴더는 크기 대신 <Folder>
		}
		else
		{
			ULONGLONG fileSize = finder.GetLength(); // 파일 크기
			size.Format(_T("%llu bytes"), fileSize); // 크기를 바이트 단위로 표시
		}

		// 리스트 컨트롤에 데이터 추가
		int index = pListCtrl->InsertItem(0, name); // 이름 추가
		pListCtrl->SetItemText(index, 1, size);    // 크기 추가
	}

	finder.Close(); // 검색 종료
}

//프레임 업데이트
void CPlayerTool::FrameListUpdate()
{
	m_FrameList.ResetContent();

	// 경로 끝에 '\' 추가 (필요 시)
	CString searchPath = m_AnimationPath.c_str();
	if (searchPath.Right(1) != _T("\\"))
		searchPath += _T("\\");
	searchPath += _T("*.png"); // .png 파일만 검색

	// CFileFind로 파일 검색
	CFileFind finder;
	BOOL isFileFound = finder.FindFile(searchPath); // 첫 번째 파일 검색

	while (isFileFound)
	{
		isFileFound = finder.FindNextFile(); // 다음 파일 검색

		CString filePath = finder.GetFilePath(); // 파일 경로 가져오기
		CString RelativePath = CDH_FilePath::Convert_RelativePath(filePath);
		CString fileName = PathFindFileName(RelativePath);

		// 확장자 명을 제거하는 함수
		TCHAR	szFileName[MAX_STR] = L"";
		lstrcpy(szFileName, fileName.GetString());
		PathRemoveExtension(szFileName);

		m_FrameList.AddString(szFileName); // 리스트 박스에 추가
	}

	finder.Close(); // 검색 종료
}



//이미지 로드하기
void CPlayerTool::OnPathSave()
{
	wofstream		fout;
	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())
	{
		for (auto& pImgPath : m_PathInfoList)
		{
			fout << pImgPath->wstrObjKey << L"|" << pImgPath->wstrStateKey << L"|"
				<< pImgPath->iCount << L"|" << pImgPath->wstrPath << endl;
		}

		fout.close();
	}

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}
void CPlayerTool::OnPathLoad()
{
	UpdateData(TRUE);

	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR		szObjKey[MAX_STR] = L"";
		TCHAR		szStateKey[MAX_STR] = L"";
		TCHAR		szCount[MAX_STR] = L"";
		TCHAR		szPath[MAX_PATH] = L"";

		wstring			wstrCombined = L"";

		m_PathList.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_PathList.AddString(wstrCombined.c_str());

		}

		fin.close();
	}

	//WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

	UpdateData(FALSE);
}
void CPlayerTool::OnDropFiles(HDROP hDropInfo)
{
	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CDH_FilePath::DirInfoExtraction(szFilePath, m_PathInfoList);
	}

	m_PathList.ResetContent();

	wstring		wstrCombined = L"";
	TCHAR		szBuf[MAX_STR] = L"";

	for (auto& pImgPath : m_PathInfoList)
	{
		// _itow_s : 정수를 유니코드 문자열로 전환
		// 10은 10진수 변환하겠다는 의미
		_itow_s(pImgPath->iCount, szBuf, 10);

		wstrCombined = pImgPath->wstrObjKey + L"|" + pImgPath->wstrStateKey
			+ L"|" + szBuf + L"|" + pImgPath->wstrPath;

		m_PathList.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);
}


//소멸자
void CPlayerTool::OnDestroy()
{
	CDialog::OnDestroy();
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), Safe_Delete<IMGPATH*>);
	m_PathInfoList.clear();


	for_each(m_mapPlayerImage.begin(), m_mapPlayerImage.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});

	m_mapPlayerImage.clear();
}

//애니메이션 인포 클릭
void CPlayerTool::OnClickAnimation(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->uChanged & LVIF_STATE) // 상태가 변경되었는지 확인
	{
		if (pNMLV->uNewState & LVIS_SELECTED) // 항목이 선택되었는지 확인
		{
			// 선택된 항목의 인덱스
			int index = pNMLV->iItem;

			// 선택된 항목의 첫 번째 컬럼 텍스트 가져오기
			wstring itemText = m_ImageInfo.GetItemText(index, 0);
			m_AnimationPath = m_PlayerPath + L"\\" + itemText;

			FrameListUpdate();
		}
	}

	*pResult = 0;
}

//프레임 리스트 미리보기
void CPlayerTool::OnFrameList()
{
	UpdateData(TRUE);

	//클릭한 아이템
	int	iIndex = m_FrameList.GetCurSel();
	CString PickedName;
	m_FrameList.GetText(iIndex, PickedName);

	//아이템 미리보기 만들자
	TCHAR	szFilePath[MAX_PATH] = L"";

	//상대경로와 이미지 합치기
	swprintf_s(szFilePath, MAX_PATH, L"%s/%s.png", m_AnimationPath.c_str(), PickedName.GetString());

	//맵에서 찾기
	auto	iter = m_mapPlayerImage.find(PickedName);

	wstring path = m_AnimationPath; // 전체 경로
	size_t lastSlash = path.find_last_of(L"\\"); // 마지막 슬래시 위치 찾기

	wstring folderName;
	if (lastSlash != wstring::npos)
	{
		folderName = path.substr(lastSlash + 1); // 마지막 슬래시 다음부터 끝까지 추출
	}

	wprintf(L"마지막 폴더명: %s\n", folderName.c_str());

	//못찾을 시
	if (iter == m_mapPlayerImage.end())
	{
		//이미지 추가(상대경로)
		CImage* pPngImage = new CImage;
		pPngImage->Load(szFilePath);

		//이미지를 맵에 추가
		iter = m_mapPlayerImage.insert({ folderName.c_str() + PickedName, pPngImage});

		CRect PictureRect = m_Rect;
		PictureRect.OffsetRect(m_iOffsetX, m_iOffsetY); // X축으로 offsetX만큼 이동

		// 위치 변경
		m_Picture01.MoveWindow(&PictureRect);

		// 이미지 출력
		m_Picture01.SetBitmap(*(iter->second));
	}
	else
	{
		CRect PictureRect = m_Rect;
		PictureRect.OffsetRect(m_iOffsetX, m_iOffsetY); // X축으로 offsetX만큼 이동

		// 위치 변경
		m_Picture01.MoveWindow(&PictureRect);

		// 이미지 출력
		m_Picture01.SetBitmap(*(iter->second));
	}

	UpdateData(FALSE);
}

//미리보기 이동
void CPlayerTool::OnMoveRight()
{
	UpdateData(TRUE);

	m_iOffsetX += 10;
	OnFrameList();

	UpdateData(FALSE);
}
void CPlayerTool::OnMoveLeft()
{
	UpdateData(TRUE);

	m_iOffsetX -= 10;
	OnFrameList();

	UpdateData(FALSE);
}
void CPlayerTool::OnMoveUp()
{
	UpdateData(TRUE);

	m_iOffsetY -= 10;
	OnFrameList();

	UpdateData(FALSE);
}
void CPlayerTool::OnMoveDonw()
{
	UpdateData(TRUE);

	m_iOffsetY += 10;
	OnFrameList();

	UpdateData(FALSE);
}

//재생, 일시정지
void CPlayerTool::OnPlayAnimation()
{
	m_Play = true;
}
void CPlayerTool::OnStopAnimation()
{
	m_Play = false;
}

//애니메이션 작동
void CPlayerTool::OnTimer(UINT_PTR nIDEvent)
{
	UpdateTimerSpeed();
	UpdateData(TRUE);
	if (nIDEvent == 1 && m_Play) // 타이머 ID 확인
	{
		// 리스트 박스 항목 개수 확인
		int itemCount = m_FrameList.GetCount();
		if (itemCount == 0) return; // 항목이 없으면 리턴

		// 현재 선택된 항목 해제
		m_FrameList.SetCurSel(-1);

		// 다음 항목으로 선택 이동
		m_CurrentSelection = (m_CurrentSelection + 1) % itemCount;
		m_FrameList.SetCurSel(m_CurrentSelection);
		OnFrameList();

		// 선택된 텍스트를 가져와서 작업 수행 (필요 시)
		CString selectedText;
		m_FrameList.GetText(m_CurrentSelection, selectedText);
		// AfxMessageBox(selectedText); // 디버깅용 (선택된 항목 확인)
	}

	CDialog::OnTimer(nIDEvent);
	UpdateData(FALSE);
}

//시간조절 함수
void CPlayerTool::UpdateTimerSpeed()
{
	// 에딧 컨트롤에서 텍스트 가져오기
	CString strDelay;
	m_Delay.GetWindowText(strDelay);

	// 텍스트를 정수로 변환
	int delayValue = _ttoi(strDelay);

	// 유효성 검사 (0 이하의 값 방지)
	if (delayValue <= 0)
	{
		delayValue = 1;
	}

	// 기존 타이머 중지
	KillTimer(1);

	// 새로운 속도로 타이머 재설정
	SetTimer(1, delayValue, NULL);
}