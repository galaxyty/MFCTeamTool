#include "pch.h"
#include "DH_MyState.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"
#include "DH_BtnUI.h"
#include "DH_OBJMgr.h"
#include "DH_Item.h"
#include "DH_Player.h"

DH_MyState::DH_MyState()
{
}

DH_MyState::~DH_MyState()
{
}

void DH_MyState::Initialize()
{
	TCHAR* MyState = L"MyState";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/MyState.png",
		TEX_SINGLE, MyState, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}


	SetMPos(D3DXVECTOR3{ 347.f, 270.f, 0.f });
	SetScale(D3DXVECTOR3{ 246.f, 173.f, 0.f });

}

void DH_MyState::Update()
{
	DH_UI::Update();
}

void DH_MyState::Render(CToolView* pMainView)
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 렌더링하지 않음


	D3DXMATRIX	matWorld, matScale, matTrans;


	//이미지 키 가져오기 밑 중심설정
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"MyState", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//월드 행렬 곱해주기
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//스크롤 값 제외
	D3DXMatrixTranslation(&matTrans,GetMPos().x,GetMPos().y,GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);



	// 렌더링
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&vTemp,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
		


#pragma region 정보 텍스트

	TCHAR	szBufHP[MIN_STR] = L"";
	swprintf_s(szBufHP, L"%d", DH_Player::Get_Instance()->GetPlayerData().iHP);
	RECT	RectBufHP{ -100, -55, -10, 50 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufHP,
		lstrlen(szBufHP),
		&RectBufHP,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufIQ[MIN_STR] = L"";
	swprintf_s(szBufIQ, L"%d", DH_Player::Get_Instance()->GetPlayerData().iIntelligence);
	RECT	RectBufIQ{ -100, -36, -10, 70 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufIQ,
		lstrlen(szBufIQ),
		&RectBufIQ,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufHealth[MIN_STR] = L"";
	swprintf_s(szBufHealth, L"%d", DH_Player::Get_Instance()->GetPlayerData().iStrength);
	RECT	RectBufHealth{ -100, -20, -10, 90 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufHealth,
		lstrlen(szBufHealth),
		&RectBufHealth,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufPhysics[MIN_STR] = L"";
	swprintf_s(szBufPhysics, L"%d", DH_Player::Get_Instance()->GetPlayerData().iPhysicalAtk);
	RECT	RectBufPhysics{ -100, -2, -10, 110 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufPhysics,
		lstrlen(szBufPhysics),
		&RectBufPhysics,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufMagic[MIN_STR] = L"";
	swprintf_s(szBufMagic, L"%d", DH_Player::Get_Instance()->GetPlayerData().iMagicAtk);
	RECT	RectBufMagic{ -100, 17, -10, 130 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufMagic,
		lstrlen(szBufMagic),
		&RectBufMagic,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufFire[MIN_STR] = L"";
	swprintf_s(szBufFire, L"%d", DH_Player::Get_Instance()->GetPlayerData().iFireResist);
	RECT	RectBufFire{ -100, 34, -10, 150 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufFire,
		lstrlen(szBufFire),
		&RectBufFire,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufLight[MIN_STR] = L"";
	swprintf_s(szBufLight, L"%d", DH_Player::Get_Instance()->GetPlayerData().iLightResist);
	RECT	RectBufLight{ -100, 51, -10, 170 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufLight,
		lstrlen(szBufLight),
		&RectBufLight,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufHang[MIN_STR] = L"";
	swprintf_s(szBufHang, L"%d", DH_Player::Get_Instance()->GetPlayerData().iManaResist);
	RECT	RectBufHang{ -100, 68, -10, 190 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufHang,
		lstrlen(szBufHang),
		&RectBufHang,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//////////////////////////////////////// 오른쪽 줄 ////////////////////////////////////////////////


	TCHAR	szBufMP[MIN_STR] = L"";
	swprintf_s(szBufMP, L"%d", DH_Player::Get_Instance()->GetPlayerData().iMP);
	RECT	RectBufMP{ 0, -55, 110, 50 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufMP,
		lstrlen(szBufMP),
		&RectBufMP,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufASpeed[MIN_STR] = L"";
	swprintf_s(szBufASpeed, L"%d", DH_Player::Get_Instance()->GetPlayerData().iAttackSpeed);
	RECT	RectBufASpeed{ 0, -36, 110, 70 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufASpeed,
		lstrlen(szBufASpeed),
		&RectBufASpeed,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufSpeed[MIN_STR] = L"";
	swprintf_s(szBufSpeed, L"%d", DH_Player::Get_Instance()->GetPlayerData().iAgility);
	RECT	RectBufSpeed{ 0, -20, 110, 90 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufSpeed,
		lstrlen(szBufSpeed),
		&RectBufSpeed,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufPhysicsDef[MIN_STR] = L"";
	swprintf_s(szBufPhysicsDef, L"%d", DH_Player::Get_Instance()->GetPlayerData().iPhysicalDef);
	RECT	RectBufPhysicsDef{ 0, -2, 110, 110 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufPhysicsDef,
		lstrlen(szBufPhysicsDef),
		&RectBufPhysicsDef,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufMagicDef[MIN_STR] = L"";
	swprintf_s(szBufMagicDef, L"%d", DH_Player::Get_Instance()->GetPlayerData().iMagicDef);
	RECT	RectBufMagicDef{ 0, 17, 110, 130 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufMagicDef,
		lstrlen(szBufMagicDef),
		&RectBufMagicDef,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufWater[MIN_STR] = L"";
	swprintf_s(szBufWater, L"%d", DH_Player::Get_Instance()->GetPlayerData().iWaterResist);
	RECT	RectBufWater{ 0, 34, 110, 150 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufWater,
		lstrlen(szBufWater),
		&RectBufWater,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR	szBufDark[MIN_STR] = L"";
	swprintf_s(szBufDark, L"%d", DH_Player::Get_Instance()->GetPlayerData().iDarkResist);
	RECT	RectBufDark{ 0, 51, 110, 170 };
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szBufDark,
		lstrlen(szBufDark),
		&RectBufDark,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 255, 255));


#pragma endregion

	RenderChild();
}


void DH_MyState::MouseOn()
{
	if (IsbLDown())
	{
		//마우스 좌표 가져오기
		D3DXVECTOR3 vDiff = Get_Mouse();
		vDiff.x -= m_vDragStart.x;
		vDiff.y -= m_vDragStart.y;

		//현재 인벤 좌표가져오기
		D3DXVECTOR3 vCurPos = GetMPos();
		vCurPos = vCurPos + vDiff;
		SetMPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}

	//마우스 on 일때 상세보기가 떠야함
}

void DH_MyState::MouseLDown()
{
	m_vDragStart = Get_Mouse();
}
void DH_MyState::MouseLUp()
{
}

void DH_MyState::MouseLClicked()
{
}