// CMapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CMapTool.h"
#include "CDevice.h"
#include "CTerrain.h"
#include "MainFrm.h"


// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent)
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
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	// 드래그해서 가져온 파일 갯수.
	UINT dragCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (int i = 0; i < dragCount; i++)
	{
		// 파일 경로.
		TCHAR szFilePath[MAX_PATH];

		// 파일 이름.
		TCHAR szFileName[MAX_PATH];

		// 드래그 정보에서 파일 절대 경로 추출.
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		// 파일 절대 경로를 상대 경로로 변환.
		CString strRelative = ConvertRelativePath(szFilePath);

		// 경로 중 파일 이름만 추출.
		strRelative = PathFindFileName(strRelative);

		lstrcpy(szFileName, strRelative.GetString());

		// 확장자 명 제거.
		PathRemoveExtension(szFileName);

		// 파일 이름을 콤보 박스에 추가
		m_ListBoxMap.AddString(szFileName);
	}

	// 가로 스크롤 크기 조정.
	m_ListBoxMap.SetHorizontalExtent(500);

	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RATIO_SLIDER, &CMapTool::OnSlider)
	ON_WM_DROPFILES()		// 드래그 이벤트 추가.
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

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
