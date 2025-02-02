#include "pch.h"
#include "DH_Interface.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"
#include "DH_BtnUI.h"


DH_Interface::DH_Interface()
{
}

DH_Interface::~DH_Interface()
{
	for (auto& ChildUI : GetChildUI())
	{
		delete ChildUI;
		ChildUI = nullptr;
	}
}

void DH_Interface::Initialize()
{
	TCHAR* UI = L"Interface";
	
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/Interface.png",
		TEX_SINGLE, UI, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}

	SetMPos(D3DXVECTOR3{ 400.f, 556.f, 0.f });
	SetScale(D3DXVECTOR3{ 800.f, 600.f, 0.f });


	SetButten();
}

void DH_Interface::Update()
{
	DH_UI::Update();
}

void DH_Interface::Render(CToolView* pMainView)
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ���������� ����

	D3DXMATRIX	matWorld, matScale, matTrans;


	//�̹��� Ű �������� �� �߽ɼ���
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Interface", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//���� ��� �����ֱ�
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//��ũ�� �� ����
	D3DXMatrixTranslation(&matTrans,GetMPos().x, GetMPos().y, GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


	// ������
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&vTemp,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	RenderChild();
}

void DH_Interface::MouseOn()
{
}

void DH_Interface::MouseLDown()
{
}

void DH_Interface::MouseLUp()
{
}

void DH_Interface::MouseLClicked()
{
}

void DH_Interface::SetButten()
{
	//������ư �߰�
	for (int i = 0; i < 6; ++i)
	{
		DH_BtnUI* Btn = new DH_BtnUI;

		wstring name = L"PotionBtn_" + std::to_wstring(i);
		Btn->SetName(name);

		Btn->SetMPos(D3DXVECTOR3{ -301.f + (30 * i), 272 - 256.f, 0.f });
		Btn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
		Btn->SeteItemParts(ITEMPARTS::POTION);
		Btn->SetVisible(true);
		Btn->Initialize();
		AddParent(Btn);
	}

	for (int i = 0; i < 6; ++i)
	{
		DH_BtnUI* Btn = new DH_BtnUI;

		wstring name = L"SkillBtn_" + std::to_wstring(i);
		Btn->SetName(name);

		Btn->SetMPos(D3DXVECTOR3{ 152.f + (30 * i), 272.f - 256, 0.f });
		Btn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
		Btn->SeteItemParts(ITEMPARTS::SKILL);
		Btn->SetVisible(true);
		Btn->Initialize();
		AddParent(Btn);
	}
}