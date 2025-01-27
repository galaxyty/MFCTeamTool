// CMapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CMapTool.h"
#include "CDevice.h"
#include "CTerrain.h"
#include "MainFrm.h"
#include "CTextureMgr.h"


// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent), m_mapKey(nullptr), m_objectKey(nullptr)
{	
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RATIO_SLIDER, m_RatioSlider);
	DDX_Control(pDX, IDC_RATIO_TEXT, m_RatioText);
	DDX_Control(pDX, IDC_LIST_BOX_MAP, m_ListBoxMap);
	DDX_Control(pDX, IDC_BACKGROUND_PICTURE, m_BGPicture);
	DDX_Control(pDX, IDC_LIST_BOX_OBJECT, m_ListBoxObject);
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);	

	// 드래그해서 가져온 파일 갯수.
	UINT dragCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	// 파일 경로.
	TCHAR* szFilePath = new TCHAR[MAX_PATH];

	// 파일 이름.
	TCHAR* szFileName = new TCHAR[MAX_PATH];

	// 마우스 좌표.
	POINT mousePosition;

	// 마우스 스크린 좌표를 가져옴
	GetCursorPos(&mousePosition);

	// 마우스 스크린 좌표를 클라이언트 좌표로 변환.
	ScreenToClient(&mousePosition);

	// 맵 리스트 박스 렉트.
	CRect mapRect;

	// m_ListBoxMap 윈도우 창 기준 렉트 좌표 반환.
	m_ListBoxMap.GetWindowRect(mapRect);

	// 클라이언트 좌표로 변환.
	ScreenToClient(mapRect);

	// 오브젝트 리스트 박스 크기.
	CRect objectRect;
	m_ListBoxObject.GetWindowRect(objectRect);
	ScreenToClient(objectRect);

	for (int i = 0; i < dragCount; i++)
	{		
		// 드래그 정보에서 파일 절대 경로 추출.
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		// 파일 절대 경로를 상대 경로로 변환.
		CString strRelative = ConvertRelativePath(szFilePath);

		// PathFindFileName : 경로 중 파일 이름만 추출.
		lstrcpy(szFileName, PathFindFileName(strRelative));

		// 확장자 명 제거.
		PathRemoveExtension(szFileName);		

		// 파일 이름을 콤보 박스에 추가
		if (mapRect.PtInRect(mousePosition))
		{
			// 맵 중복 확인.
			auto iter = find_if(m_mapBackground.begin(), m_mapBackground.end(), [szFileName](auto& pair)
				{
					// find_if에서 true를 반환하면 해당 iterator 요소를 반환.
					return pair.first == szFileName;
				});

			if (iter != m_mapBackground.end())
			{
				// 중복 키.
				continue;
			}

			// 맵 리스트 드래그.
			m_ListBoxMap.AddString(szFileName);

			CImage* bg = new CImage();
			bg->Load(strRelative);

			m_mapBackground.insert({ szFileName , bg });
		}
		else if (objectRect.PtInRect(mousePosition))
		{
			// 맵 중복 확인.
			auto iter = find_if(m_objectBackground.begin(), m_objectBackground.end(), [szFileName](auto& pair)
				{
					// find_if에서 true를 반환하면 해당 iterator 요소를 반환.
					return pair.first == szFileName;
				});

			if (iter != m_objectBackground.end())
			{
				// 중복 키.
				continue;
			}

			// 오브젝트 리스트 드래그.
			m_ListBoxObject.AddString(szFileName);

			CImage* bg = new CImage();
			bg->Load(strRelative);

			m_objectBackground.insert({ szFileName , bg });
		}

		if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
			strRelative,
			TEX_SINGLE, szFileName, nullptr, 0)))
		{
			AfxMessageBox(L"Background Texture Insert Failed");
			return;
		}		
	}

	Safe_Delete(szFilePath);
	Safe_Delete(szFileName);

	CDC* pDC = m_ListBoxMap.GetDC();
	CString		strName;
	int iWidth = 0;

	for (int i = 0; i < m_ListBoxMap.GetCount(); ++i)
	{
		m_ListBoxMap.GetText(i, strName);

		// GetTextExtent : 매개 변수에 해당하는 문자열의 길이를 픽셀 단위로 변환.
		if (pDC->GetTextExtent(strName).cx > iWidth)
		{
			iWidth = pDC->GetTextExtent(strName).cx;
		}
	}

	// 가로 스크롤 크기 조정.
	m_ListBoxMap.SetHorizontalExtent(iWidth);

	UpdateRender();

	m_ListBoxMap.ReleaseDC(pDC);

	UpdateData(FALSE);	
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RATIO_SLIDER, &CMapTool::OnSlider)
	ON_WM_DROPFILES()		// 드래그 이벤트 추가.
	ON_WM_DESTROY()			// 파괴 함수 추가.
	ON_LBN_SELCHANGE(IDC_LIST_BOX_MAP, &CMapTool::OnListBGClick)
	ON_BN_CLICKED(IDC_MAP_APPLY_BUTTON, &CMapTool::OnApplyClick)
	ON_BN_CLICKED(IDC_MAP_DELETE_BUTTON, &CMapTool::OnDeleteClick)
	ON_LBN_SELCHANGE(IDC_LIST_BOX_OBJECT, &CMapTool::OnListObjectClick)
	ON_BN_CLICKED(IDC_OBJECT_APPLY_BUTTON, &CMapTool::OnObjectApplyClick)
	ON_BN_CLICKED(IDC_OBJECT_DELETE_BUTTON, &CMapTool::OnObjectDeleteClick)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_mapKey == nullptr)
	{
		m_mapKey = new TCHAR[MAX_PATH];
	}

	if (m_objectKey == nullptr)
	{
		m_objectKey = new TCHAR[MAX_PATH];
	}

	// TODO:  여기에 추가 초기화 작업을 추가합니다.	
	m_RatioSlider.SetRange(20, 100);	
	m_RatioSlider.SetPos(100);
	
	UpdateRender();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::OnSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_Ratio = m_RatioSlider.GetPos() / 100.f;
	UpdateRender();
	*pResult = 0;	
}

void CMapTool::UpdateRender()
{
	TCHAR	szBuf[MIN_STR] = L"";
	swprintf_s(szBuf, L"현재 맵 비율 : %f", g_Ratio);
	m_RatioText.SetWindowTextW(szBuf);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CTerrain* pTerrain = pView->m_pTerrain;
	CDevice::Get_Instance()->Render_Begin();

	pTerrain->Render();

	CDevice::Get_Instance()->Render_End();
}

CString CMapTool::ConvertRelativePath(CString _path)
{
	// 상대 경로.
	TCHAR   szRelativePath[MAX_PATH] = L"";

	// 프로젝트 경로.
	TCHAR   szCurDirPath[MAX_PATH] = L"";

	// 프로젝트 경로 추출.
	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : 절대 경로를 상대 경로로 변환
	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		_path.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapBackground.begin(), m_mapBackground.end(), [](auto& pair)
		{
			pair.second->Destroy();
			Safe_Delete(pair.second);
		});

	m_mapBackground.clear();

	for_each(m_objectBackground.begin(), m_objectBackground.end(), [](auto& pair)
		{
			pair.second->Destroy();
			Safe_Delete(pair.second);
		});

	m_objectBackground.clear();

	Safe_Delete(m_mapKey);
	Safe_Delete(m_objectKey);
}

// 배경 리스트 박스 클릭.
void CMapTool::OnListBGClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_ListBoxMap.GetCurSel();

	m_ListBoxMap.GetText(iIndex, strFindName);

	auto	iter = m_mapBackground.find(strFindName);

	if (iter == m_mapBackground.end())
		return;

	CDC* dc = m_BGPicture.GetDC();		
	//m_BGPicture.SetBitmap(*(iter->second));
	iter->second->StretchBlt(*dc, 0, 0, 150, 100, SRCCOPY);

	m_BGPicture.ReleaseDC(dc);

	UpdateData(FALSE);
}

// 적용 버튼.
void CMapTool::OnApplyClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	CString strItem;
	m_ListBoxMap.GetText(m_ListBoxMap.GetCurSel(), strItem);

	// CString에서 TCHAR 배열로 복사
	_tcscpy_s(m_mapKey, MAX_PATH, strItem);

	CTextureMgr::Get_Instance()->SetBGKey(m_mapKey);	

	UpdateRender();
}

// 삭제 버튼.
void CMapTool::OnDeleteClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxMap.GetText(m_ListBoxMap.GetCurSel(), strItem);
	m_ListBoxMap.DeleteString(m_ListBoxMap.GetCurSel());	

	Safe_Delete(m_mapBackground[strItem]);
	m_mapBackground.erase(strItem);

	UpdateRender();
}

// 오브젝트 리스트 박스 클릭.
void CMapTool::OnListObjectClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	int a = 10;
}

// 오브젝트 배치 버튼.
void CMapTool::OnObjectApplyClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxObject.GetText(m_ListBoxObject.GetCurSel(), strItem);

	// CString에서 TCHAR 배열로 복사
	_tcscpy_s(m_objectKey, MAX_PATH, strItem);

	CTextureMgr::Get_Instance()->SetObjectKey(m_objectKey);
}

// 오브젝트 삭제 버튼.
void CMapTool::OnObjectDeleteClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxObject.GetText(m_ListBoxObject.GetCurSel(), strItem);
	m_ListBoxObject.DeleteString(m_ListBoxObject.GetCurSel());

	Safe_Delete(m_objectBackground[strItem]);
	m_objectBackground.erase(strItem);

	UpdateRender();
}
