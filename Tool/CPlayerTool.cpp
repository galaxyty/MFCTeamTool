// CPlayerTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "Include.h"
#include "afxdialogex.h"
#include "CPlayerTool.h"
#include "CDH_FilePath.h"
#include "CTextureMgr.h"
#include "DH_HitBox.h"


// CPlayerTool 대화 상자

IMPLEMENT_DYNAMIC(CPlayerTool, CDialog)

CPlayerTool::CPlayerTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPlayerTool, pParent), m_iOffsetX(0), m_iOffsetY(0), m_PlayerState(), m_CurrentSelection(-1)
	, m_Play(true), m_HitBox(nullptr)
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
	DDX_Control(pDX, IDC_PROGRESS1, m_Gage);
	DDX_Control(pDX, IDC_CHECK1, m_CheckHit);
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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK1, &CPlayerTool::OnCheckHit)
	ON_BN_CLICKED(IDC_BUTTON3, &CPlayerTool::OnSaveBtn)
END_MESSAGE_MAP()

BOOL CPlayerTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strDelay;
	strDelay.Format(_T("%d"), 120);
	m_Delay.SetWindowText(strDelay);

	SetTimer(1, 120, NULL);

	m_Gage.SetRange(0, 100); // 최소값: 0, 최대값: 100
	m_Gage.SetPos(0);        // 초기값 설정

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


	for_each(m_SaveHitMap.begin(), m_SaveHitMap.end(), [](auto& MyPair)
		{
			delete MyPair.first;
		});

	m_SaveHitMap.clear();

}

//애니메이션 인포 클릭
void CPlayerTool::OnClickAnimation(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_bCheckHit = false;
	m_CheckHit.SetCheck(false);
	m_Play = true;
	m_HitRect = { 0,0,0,0 };

	CWnd* pPicture = GetDlgItem(IDC_PICTURE);
	if (pPicture)
	{
		CDC* pDC = pPicture->GetDC();
		RECT Base = { 0 , 0 , 287, 267 };
		pDC->FillSolidRect(&Base, GetSysColor(COLOR_BTNFACE));
	}

	///////////////////////////////////////////////////////////////

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->uChanged & LVIF_STATE) // 상태가 변경되었는지 확인
	{
		if (pNMLV->uNewState & LVIS_SELECTED) // 항목이 선택되었는지 확인
		{
			// 선택된 항목의 인덱스
			int index = pNMLV->iItem;

			// 선택된 항목의 첫 번째 컬럼 텍스트 가져오기
			m_SkillKey = m_ImageInfo.GetItemText(index, 0);
			m_AnimationPath = m_PlayerPath + L"\\" + m_SkillKey;

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
 
	if (m_iPrev != iIndex)
	{
		HITBOX* pHitBox = new HITBOX;
		pHitBox->HitKey = folderName;
		pHitBox->HitFrame = m_FrameList.GetCurSel();
		LoadHitBox(*pHitBox);

		delete pHitBox;
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


	m_iPrev = iIndex;
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
	m_bCheckHit = false;
	m_CheckHit.SetCheck(false);
	m_HitRect = { 0,0,0,0 };
	
	CWnd* pPicture = GetDlgItem(IDC_PICTURE);
	if (pPicture)
	{
		CDC* pDC = pPicture->GetDC();
		RECT Base = { 0 , 0 , 287, 267 };
		pDC->FillSolidRect(&Base, GetSysColor(COLOR_BTNFACE));
	}
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
	
	//플레이 중이 아닐 때, 체크박스가 켜져있을 때
	if (nIDEvent == 1 && !m_Play && m_bCheckHit)
	{
		DrawOnPictureControl();
	}
	// 플레이 중일 때
	else if (nIDEvent == 1 && m_Play) // 타이머 ID 확인
	{
		int itemCount = m_FrameList.GetCount();
		m_Gage.SetRange(0, itemCount - 3);
		//재생중인데 아무것도 선택안했을 때
		if (itemCount == 0)
		{		
			Invalidate(FALSE);
			return;
		}
		// 현재 선택된 항목 해제
		m_FrameList.SetCurSel(-1);

		// 다음 항목으로 선택 이동
		m_CurrentSelection = (m_CurrentSelection + 1) % itemCount;
		m_FrameList.SetCurSel(m_CurrentSelection);
		m_Gage.SetPos(m_CurrentSelection);
		OnFrameList();

		// 선택된 텍스트를 가져와서 작업 수행 (필요 시)
		CString selectedText;
		m_FrameList.GetText(m_CurrentSelection, selectedText);

		Invalidate(FALSE);
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

//히트박스 로드
void CPlayerTool::LoadHitBox(HITBOX _HITBOX)
{
	for (auto& Pair : m_SaveHitMap)
	{
		//순회하고 맵에 이미 있는 프레임이면?
		if (Pair.first->HitKey == _HITBOX.HitKey && Pair.first->HitFrame == _HITBOX.HitFrame)
		{
			m_HitRect = Pair.second;
			break;
		}
		else
		{
			//히트 재생시 저장된 박스 외 제거
			m_HitRect = { 0,0,0,0 };
		}
	}
}

//마우스 함수
void CPlayerTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bCheckHit)
	{
		m_bDragging = true;      // 드래그 시작
		m_StartPoint = point;    // 시작 좌표 저장
		m_EndPoint = point;      // 초기화
		SetCapture();            // 마우스 캡처
	}

	CDialog::OnLButtonDown(nFlags, point);
}
void CPlayerTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		m_EndPoint = point;   // 최종 좌표 설정
		m_bDragging = false; // 드래그 종료
		ReleaseCapture();    // 마우스 캡처 해제
	}

	CDialog::OnLButtonUp(nFlags, point);
}
void CPlayerTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		if (point.x <= 358)
			point.x = 359;
		else if (point.x >= 645)
			point.x = 644;
		if (point.y <= 55)
			point.y = 56;
		else if (point.y >= 322)
			point.y = 321;

		m_EndPoint = point;  // 마우스 끝 좌표 갱신
		m_HitRect.SetRect(m_StartPoint, m_EndPoint);  // 드래그 영역 설정
		DrawOnPictureControl();
	}

	CDialog::OnMouseMove(nFlags, point);
}

//히트박스 드래그 함수
void CPlayerTool::OnPaint()
{
	CPaintDC dc(this);
	
}
void CPlayerTool::DrawOnPictureControl()
{
	// Picture Control 가져오기
	CWnd* pPicture = GetDlgItem(IDC_PICTURE); // Picture Control의 ID
	if (pPicture)
	{
		// Picture Control의 클라이언트 영역 가져오기
		CRect pictureRect;
		pPicture->GetClientRect(&pictureRect);       // Picture Control의 클라이언트 영역
		pPicture->ClientToScreen(&pictureRect);      // Picture Control의 화면 좌표
		ScreenToClient(&pictureRect);                // 다이얼로그 기준 좌표로 변환

		// 클릭 좌표를 Picture Control 기준 좌표로 변환
		CRect localRect = m_HitRect;
		localRect.OffsetRect(-pictureRect.left, -pictureRect.top);

		// Picture Control의 DC 가져오기
		CDC* pDC = pPicture->GetDC();
		CBrush brush(RGB(255, 0, 0));

		// 클릭한 위치에 렉트 그리기
		RECT Base = { 0 , 0 , 287, 267 };
		pDC->FillSolidRect(&Base, GetSysColor(COLOR_BTNFACE));
		OnFrameList();
		pDC->FrameRect(&localRect, &brush);

		// DC 해제
		pPicture->ReleaseDC(pDC);
	}
}

//히트 체크박스
void CPlayerTool::OnCheckHit()
{
	UpdateData(TRUE);

	if (m_CheckHit.GetCheck())
	{
		if (m_Play) // m_Play가 활성화된 상태일 경우
		{
			AfxMessageBox(L"현재 재생 중입니다. 체크를 해제합니다.");

			// 체크 해제
			m_CheckHit.SetCheck(BST_UNCHECKED);

			// 동기화
			m_bCheckHit = false;
			UpdateData(FALSE);
		}
		else if (m_FrameList.GetCurSel() == -1)
		{
			AfxMessageBox(L"선택된 애니메이션이 없습니다.");

			// 체크 해제
			m_CheckHit.SetCheck(BST_UNCHECKED);

			// 동기화
			m_bCheckHit = false;
			UpdateData(FALSE);
		}
		else
		{
			m_bCheckHit = true; // 체크 활성화
		}
	}
	else
	{
		m_bCheckHit = false;
	}

	UpdateData(FALSE);
}

//애니메이션 저장
void CPlayerTool::OnSaveBtn()
{
	bool found = false;
	for (auto& Pair : m_SaveHitMap)
	{
		if (Pair.first->HitKey == m_SkillKey &&
			Pair.first->HitFrame == m_FrameList.GetCurSel())
		{
			// 이미 존재하는 키 -> 값 덮어쓰기
			Pair.second = m_HitRect;
			found = true;
			break; 
		}
	}

	if (!found)
	{
		// 키가 존재하지 않는 경우 새로 추가
		HITBOX* pHitBox = new HITBOX;
		pHitBox->HitKey = m_SkillKey;
		pHitBox->HitFrame = m_FrameList.GetCurSel();
		m_SaveHitMap.insert(make_pair(pHitBox, m_HitRect));

		char message[256];
		sprintf_s(message, "%ls 키의 %d 프레임에 히트박스를 저장하였습니다!",
			pHitBox->HitKey.c_str(), pHitBox->HitFrame);
		AfxMessageBox(CString(message));
	}
}

