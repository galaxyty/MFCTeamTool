#include "pch.h"
#include "DH_LoopMgr.h"
#include "DH_KeyMgr.h"
#include "DH_UIMgr.h"
#include "DH_OBJMgr.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "DH_TimeMgr.h"

IMPLEMENT_SINGLETON(DH_LoopMgr)

DH_LoopMgr::DH_LoopMgr() :m_dwTime(0), m_iFPS(0), m_wcBuff(L"")
{
}

DH_LoopMgr::~DH_LoopMgr()
{
	Release();
}

void DH_LoopMgr::Initialize()
{
	DH_UIMgr::Get_Instance()->Initialize();
	DH_KeyMgr::Get_Instance()->Initialize();
}

void DH_LoopMgr::Progress()
{
	Update();
	LateUpdate();
	Render();
}

void DH_LoopMgr::Update()
{
	DH_TimeMgr::Get_Instance()->Update();
	DH_KeyMgr::Get_Instance()->Update();
	DH_UIMgr::Get_Instance()->Update();
	DH_OBJMgr::Get_Instance()->Update();
	
}

void DH_LoopMgr::LateUpdate()
{
	DH_OBJMgr::Get_Instance()->Late_Update();
}

void DH_LoopMgr::Render()
{
#pragma region FPS 출력

	++m_iFPS;
	if (m_dwTime + 2000 < GetTickCount64())
	{
		swprintf_s(m_wcBuff, L"던전 앤 파이터 툴 (FPS : %d)", m_iFPS);
		// 창 제목 변경.
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		pMainFrm->SetWindowText(m_wcBuff);
		pMainFrm->ShowWindow(SW_SHOW);
		pMainFrm->UpdateWindow();
	
		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}

#pragma endregion
	
	if (m_pToolView)
	{
		m_pToolView->Invalidate(FALSE); // 툴 뷰의 함수 호출
	}
}

void DH_LoopMgr::Release()
{
	DH_KeyMgr::Get_Instance()->Destroy_Instance();
	DH_UIMgr::Get_Instance()->DestroyInstance();
	DH_OBJMgr::Get_Instance()->DestroyInstance();
}
