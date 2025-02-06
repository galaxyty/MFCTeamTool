// CMyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CMyForm.h"
#include "DH_Player.h"
#include "DH_OBJMgr.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_CMyForm), pPlayer(nullptr)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnitTool)
	ON_BN_CLICKED(IDC_MAP_BUTTON, &CMyForm::OnMapButton)
	ON_BN_CLICKED(IDC_EQUIP_BUTTON, &CMyForm::OnEquipButton)
	ON_BN_CLICKED(IDC_EQUIP_BUTTON2, &CMyForm::OnPlayer)
	ON_BN_CLICKED(IDC_EQUIP_BUTTON3, &CMyForm::OnPlay)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyForm 진단

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Font.DeleteObject(); // 기존 폰트 삭제 (필수)
	m_Font.CreateFont(
		13,                 // 글자 높이
		0,                  // 글자 너비 (0이면 자동 조정)
		0,                  // 텍스트 방향(각도)
		0,                  // 글자 베이스라인과의 각도
		FW_BOLD,            // 글자 굵기 (FW_NORMAL, FW_BOLD 등)
		FALSE,              // 이탤릭체 여부
		FALSE,              // 밑줄 여부
		FALSE,              // 취소선 여부
		DEFAULT_CHARSET,    // 문자셋
		OUT_DEFAULT_PRECIS, // 출력 정밀도
		CLIP_DEFAULT_PRECIS,// 클리핑 정밀도
		DEFAULT_QUALITY,    // 출력 품질
		DEFAULT_PITCH | FF_SWISS, // 글꼴 피치 및 패밀리
		L"고딕"           // 글꼴 이름
	);

	GetDlgItem(IDC_MAP_BUTTON)->SetFont(&m_Font);
	GetDlgItem(IDC_EQUIP_BUTTON)->SetFont(&m_Font);
	GetDlgItem(IDC_EQUIP_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_EQUIP_BUTTON3)->SetFont(&m_Font);
}


void CMyForm::OnUnitTool()
{
	//GetSafeHwnd : 현재 다이얼로그 윈도우의 핸들을 반환
	//if(nullptr == m_UnitTool.GetSafeHwnd())
	//	m_UnitTool.Create(IDD_CUnitTool);	// 해당 id에 맞는 다이얼로그 생성

	//m_UnitTool.ShowWindow(SW_SHOW);
}

// 맵 툴 버튼.
bool g_GamePlay = false;
void CMyForm::OnMapButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//GetSafeHwnd : 현재 다이얼로그 윈도우의 핸들을 반환
	if(nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_CMapTool);	// 해당 id에 맞는 다이얼로그 생성

	m_MapTool.ShowWindow(SW_SHOW);
}

// 방어구 툴 버튼.
void CMyForm::OnEquipButton()
{
	g_GamePlay = true;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	//GetSafeHwnd : 현재 다이얼로그 윈도우의 핸들을 반환
	if (nullptr == m_EquipTool.GetSafeHwnd())
		m_EquipTool.Create(IDD_CEquipTool);	// 해당 id에 맞는 다이얼로그 생성

	m_EquipTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnPlayer()
{
	g_GamePlay = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//GetSafeHwnd : 현재 다이얼로그 윈도우의 핸들을 반환
	if (nullptr == m_PlayerTool.GetSafeHwnd())
		m_PlayerTool.Create(IDD_CPlayerTool);	// 해당 id에 맞는 다이얼로그 생성

	m_PlayerTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnPlay()
{
	g_GamePlay = true;

	//플레이어 추가
	if (DH_OBJMgr::Get_Instance()->Get_Player().size() == 0)
	{
		pPlayer = DH_Player::Get_Instance();
		pPlayer->SetName(L"pPlayer");
		pPlayer->SetPlayerTool(&m_PlayerTool);
		pPlayer->Initialize();
		DH_OBJMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);
	}
}


void CMyForm::OnDestroy()
{
	CFormView::OnDestroy();
	DH_Player::Destroy_Instance();
}
