#include "pch.h"
#include "DH_Inventory.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"
#include "DH_BtnUI.h"
#include "DH_OBJMgr.h"
#include "DH_Item.h"

DH_Inventory::DH_Inventory() : Interface(nullptr)
{
}

DH_Inventory::~DH_Inventory() 
{
	for (auto& ChildUI : GetChildUI())
	{
		delete ChildUI;
		ChildUI = nullptr;
	}
}

void DH_Inventory::Initialize()
{
	TCHAR* Inven = L"Inven";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/Inven.png",
		TEX_SINGLE, Inven, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}


	SetMPos(D3DXVECTOR3{ 600.f, 300.f, 0.f });
	SetScale(D3DXVECTOR3{ 260.f, 530.f, 0.f });

	SetButten();
}

void DH_Inventory::Update()
{
	DH_UI::Update();
}

void DH_Inventory::Render(CToolView* pMainView)
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 렌더링하지 않음


	D3DXMATRIX	matWorld, matScale, matTrans;


	//이미지 키 가져오기 밑 중심설정
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Inven", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//월드 행렬 곱해주기
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//스크롤 값 제외
	D3DXMatrixTranslation(&matTrans,
		GetMPos().x, 
		GetMPos().y, 
		GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	if (m_bLDown)
	{
		// 렌더링
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, 
			nullptr,		
			&vTemp,			
			nullptr,		
			D3DCOLOR_ARGB(255, 255, 255, 255)); 
	}
	else
	{
		// 렌더링
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, 
			nullptr,		
			&vTemp,			
			nullptr,		
			D3DCOLOR_ARGB(255, 255, 255, 255)); 
	}


	RenderChild();
}


void DH_Inventory::MouseOn()
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
}

void DH_Inventory::MouseLDown()
{
	m_vDragStart = Get_Mouse();
}
void DH_Inventory::MouseLUp()
{
}

void DH_Inventory::MouseLClicked()
{
}

void DH_Inventory::SetButten()
{
	//하위버튼 추가
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			DH_BtnUI* Btn = new DH_BtnUI;

			wstring name = L"Btn_" + std::to_wstring(i) + L"_" + std::to_wstring(j);
			Btn->SetName(name);

			Btn->SetMPos(D3DXVECTOR3{ -106.f + (30 * j), -20.f + (30 * i),0.f });
			Btn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
			Btn->SeteItemParts(ITEMPARTS::ETC);
			Btn->Initialize();
			AddParent(Btn);
		}
	}

#pragma region 장비창 버튼

	DH_BtnUI* AmorShoulderBtn = new DH_BtnUI;
	wstring name = L"AmorShoulderBtn";
	AmorShoulderBtn->SetName(name);
	AmorShoulderBtn->SetMPos(D3DXVECTOR3{ -103.f, -180.f ,0.f });
	AmorShoulderBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	AmorShoulderBtn->SeteItemParts(ITEMPARTS::SHOULDER);
	AmorShoulderBtn->SetbEquip(true);
	AmorShoulderBtn->Initialize();
	AddParent(AmorShoulderBtn);

	DH_BtnUI* AmorTopBtn = new DH_BtnUI;
	name = L"AmorTopBtn";
	AmorTopBtn->SetName(name);
	AmorTopBtn->SetMPos(D3DXVECTOR3{ -71.f, -180.f ,0.f });
	AmorTopBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	AmorTopBtn->SeteItemParts(ITEMPARTS::TOP);
	AmorTopBtn->SetbEquip(true);
	AmorTopBtn->Initialize();
	AddParent(AmorTopBtn);

	DH_BtnUI* AmorBottomBtn = new DH_BtnUI;
	name = L"AmorBottomBtn";
	AmorBottomBtn->SetName(name);
	AmorBottomBtn->SetMPos(D3DXVECTOR3{ -103.f, -148.f ,0.f });
	AmorBottomBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	AmorBottomBtn->SeteItemParts(ITEMPARTS::BOTTOM);
	AmorBottomBtn->SetbEquip(true);
	AmorBottomBtn->Initialize();
	AddParent(AmorBottomBtn);

	DH_BtnUI* AmorBeltBtn = new DH_BtnUI;
	name = L"AmorBeltBtn";
	AmorBeltBtn->SetName(name);
	AmorBeltBtn->SetMPos(D3DXVECTOR3{ -71.f, -148.f ,0.f });
	AmorBeltBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	AmorBeltBtn->SeteItemParts(ITEMPARTS::BELT);
	AmorBeltBtn->SetbEquip(true);
	AmorBeltBtn->Initialize();
	AddParent(AmorBeltBtn);

	DH_BtnUI* AmorShoesBtn = new DH_BtnUI;
	name = L"AmorShoesBtn";
	AmorShoesBtn->SetName(name);
	AmorShoesBtn->SetMPos(D3DXVECTOR3{ -103.f, -116.f ,0.f });
	AmorShoesBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	AmorShoesBtn->SeteItemParts(ITEMPARTS::SHOES);
	AmorShoesBtn->SetbEquip(true);
	AmorShoesBtn->Initialize();
	AddParent(AmorShoesBtn);

#pragma endregion

#pragma region 문양, 보석

	DH_BtnUI* AmorBookBtn = new DH_BtnUI;
	name = L"AmorBookBtn";
	AmorBookBtn->SetName(name);
	AmorBookBtn->SetMPos(D3DXVECTOR3{ -64.f, -111.f ,0.f });
	AmorBookBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	AmorBookBtn->SeteItemParts(ITEMPARTS::BOOK);
	AmorBookBtn->SetbEquip(true);
	AmorBookBtn->Initialize();
	AddParent(AmorBookBtn);

	DH_BtnUI* JewerlyBtn = new DH_BtnUI;
	name = L"JewerlyBtn";
	JewerlyBtn->SetName(name);
	JewerlyBtn->SetMPos(D3DXVECTOR3{ 64.f, -111.f ,0.f });
	JewerlyBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	JewerlyBtn->SeteItemParts(ITEMPARTS::JEWERLY);
	JewerlyBtn->SetbEquip(true);
	JewerlyBtn->Initialize();
	AddParent(JewerlyBtn);

#pragma endregion

#pragma region 무기 버튼

	DH_BtnUI* WeaponBtn = new DH_BtnUI;
	name = L"WeaponBtn";
	WeaponBtn->SetName(name);
	WeaponBtn->SetMPos(D3DXVECTOR3{ 71.f, -180.f ,0.f });
	WeaponBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	WeaponBtn->SeteItemParts(ITEMPARTS::WEAPON);
	WeaponBtn->SetbEquip(true);
	WeaponBtn->Initialize();
	AddParent(WeaponBtn);

	DH_BtnUI* SymbolBtn = new DH_BtnUI;
	name = L"SymbolBtn";
	SymbolBtn->SetName(name);
	SymbolBtn->SetMPos(D3DXVECTOR3{ 103.f, -180.f, 0.f });
	SymbolBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	SymbolBtn->SeteItemParts(ITEMPARTS::SYMBOL);
	SymbolBtn->SetbEquip(true);
	SymbolBtn->Initialize();
	AddParent(SymbolBtn);

	DH_BtnUI* BraceletBtn = new DH_BtnUI;
	name = L"BraceletBtn";
	BraceletBtn->SetName(name);
	BraceletBtn->SetMPos(D3DXVECTOR3{ 71.f, -148.f ,0.f });
	BraceletBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	BraceletBtn->SeteItemParts(ITEMPARTS::BRACELET);
	BraceletBtn->SetbEquip(true);
	BraceletBtn->Initialize();
	AddParent(BraceletBtn);

	DH_BtnUI* NeckBtn = new DH_BtnUI;
	name = L"NeckBtn";
	NeckBtn->SetName(name);
	NeckBtn->SetMPos(D3DXVECTOR3{ 103.f, -148.f, 0.f });
	NeckBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	NeckBtn->SeteItemParts(ITEMPARTS::NECK);
	NeckBtn->SetbEquip(true);
	NeckBtn->Initialize();
	AddParent(NeckBtn);

	DH_BtnUI* RingBtn = new DH_BtnUI;
	name = L"RingBtn";
	RingBtn->SetName(name);
	RingBtn->SetMPos(D3DXVECTOR3{ 103.f, -116.f ,0.f });
	RingBtn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
	RingBtn->SeteItemParts(ITEMPARTS::RING);
	RingBtn->SetbEquip(true);
	RingBtn->Initialize();
	AddParent(RingBtn);

#pragma endregion

}


