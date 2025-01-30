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
		return; // UI가 비활성화 상태라면 렌더링하지 않음



	D3DXVECTOR3 vPinalPos = GetPinalPos();

	//이미지 키 가져오기 밑 중심설정
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BtnUI", nullptr);
	const TEXINFO* pTexInfo2 = CTextureMgr::Get_Instance()->Get_Texture(L"BtnUI2", nullptr);
	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//월드 행렬 곱해주기
	D3DXMATRIX	matWorld, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, vPinalPos.x, vPinalPos.y, vPinalPos.z);
	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	//클릭 , 비클릭
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
