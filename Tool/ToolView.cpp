﻿
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "MainFrm.h"
#include "CTerrain.h"
#include "DH_OBJMgr.h"
#include "DH_LoopMgr.h"
#include "CMapManager.h"
#include "DH_TimeMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
	: m_pDevice(CDevice::Get_Instance())
	, m_pTerrain(nullptr)

{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	DH_TimeMgr::Get_Instance()->Initialize();

	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY / 2));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rcWnd{};
	pMainFrm->GetWindowRect(&rcWnd);


	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);


	RECT rcMainView{};

	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);
	pMainFrm->SetWindowPos(nullptr,		// 순서 변경하지 않음
							0, 
							0,
							int(WINCX + fRowFrm), 
							int(WINCY + fColFrm), 
							SWP_NOZORDER); // 현재 순서를 유지함


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Init_Device Create Failed");
		return;
	}
	
	m_pTerrain = new CTerrain;
	if (FAILED(m_pTerrain->Initialize()))
	{
		AfxMessageBox(L"m_pTerrain Create Failed");
		return;
	}

	m_pTerrain->Set_MainView(this);
	DH_OBJMgr::Get_Instance()->Set_MainView(this);
	DH_LoopMgr::Get_Instance()->SetToolView(this);
	//UI 추가

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);

	m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0) * g_Ratio,
										float(point.y) + GetScrollPos(1) * g_Ratio,
										0.f));

	if (CMapManager::Get_Instance()->m_IsObjectSetting == true)
	{
		// 화면 좌표 가져오기
		// point 매개변수쓰면 안맞아서 안씀.
		CPoint screenPoint;
		GetCursorPos(&screenPoint);

		// 스크린 좌표를 클라이언트 좌표로 변환.
		// MFC 관련 클래스를 상속 받은 상태라 ScreenToClient 함수는 MFC 네임스페이스쪽 함수임
		// MFC에서 지원하는게 아닌 기존에 있는 ScreenToClient 함수를 써야하니 :: 를 붙여 기존 네임스페이스를 가리킴.
		// :: 왼쪽에 비어 있는 이유는 아마 C++에서 ::만 쓰면 디폴트 네임스페이스로 잡아줘서 그런 거 같다.
		::ScreenToClient(m_pTerrain->m_pMainView->m_hWnd, &screenPoint);		

		// 비율에 맞게 포인트 좌표 갱신.
		screenPoint.x *= 1.f / g_Ratio;
		screenPoint.y *= 1.f / g_Ratio;

		CustomOBJECT* _object = new CustomOBJECT();

		_object->vPos = {(float)screenPoint.x + GetScrollPos(0), (float)screenPoint.y + GetScrollPos(1), 0.f};
		_object->vSize = {0.f, 0.f};
		_object->szName = CMapManager::Get_Instance()->GetObjectKeyValue();
		_object->szShowName = CMapManager::Get_Instance()->GetObjectKeyValue();

		CMapManager::Get_Instance()->AddObject(_object);

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CMyForm* pForm = dynamic_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

		pForm->m_MapTool.m_ListBoxObjectList.AddString(_object->szName->GetString());
	}

	Invalidate(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMapManager::Get_Instance()->m_IsObjectSetting = false;
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pDevice->Render_Begin();

	m_pTerrain->Render();
	DH_OBJMgr::Get_Instance()->Render();

	m_pDevice->Render_End();


}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);

	CTextureMgr::Destroy_Instance();
	CMapManager::Destroy_Instance();
	DH_TimeMgr::Destroy_Instance();
	m_pDevice->Destroy_Instance();

}


#pragma region 생략
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion








