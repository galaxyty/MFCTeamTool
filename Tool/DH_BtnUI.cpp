#include "pch.h"
#include "DH_BtnUI.h"
#include "CDevice.h"
#include "CTextureMgr.h"

DH_BtnUI::DH_BtnUI() : m_bClicked(false)
{
}

DH_BtnUI::~DH_BtnUI()
{
	for (auto& ChildUI : GetChildUI())
	{
		delete ChildUI;
		ChildUI = nullptr;
	}
}

void DH_BtnUI::MouseOn()
{
}

void DH_BtnUI::MouseLDown()
{
}

void DH_BtnUI::MouseLUp()
{
}

void DH_BtnUI::MouseLClicked()
{
	if (IsVisible())
		m_bClicked = true;
}

void DH_BtnUI::Initialize()
{
	TCHAR* BtnUI = L"BtnUI";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/ItemSlot.png",
		TEX_SINGLE, BtnUI, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}

	TCHAR* BtnUI2 = L"BtnUI2";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/ItemSlot2.png",
		TEX_SINGLE, BtnUI2, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}
}


void DH_BtnUI::Render()
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ���������� ����



	D3DXVECTOR3 vPinalPos = GetPinalPos();

	//�̹��� Ű �������� �� �߽ɼ���
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BtnUI", nullptr);
	const TEXINFO* pTexInfo2 = CTextureMgr::Get_Instance()->Get_Texture(L"BtnUI2", nullptr);
	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//���� ��� �����ֱ�
	D3DXMATRIX	matWorld, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, vPinalPos.x, vPinalPos.y, vPinalPos.z);
	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	//Ŭ�� , ��Ŭ��
	if (m_bLDown)
	{
		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo2->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	RenderChild();
}
