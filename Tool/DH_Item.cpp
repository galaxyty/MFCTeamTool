#include "pch.h"
#include "DH_Item.h"
#include "DH_OBJMgr.h"
#include "DH_UI.h"
#include "CTextureMgr.h"
#include "CDevice.h"

DH_Item::DH_Item() : m_Inven(nullptr), m_ImageKey()
{
	ZeroMemory(m_vDragStart, sizeof(D3DXVECTOR3));
}

DH_Item::~DH_Item()
{
}

void DH_Item::MouseOn()
{
	if (IsbLDown())
	{
		D3DXVECTOR3 vDiff;
		vDiff.x = Get_Mouse().x - m_vDragStart.x;
		vDiff.y = Get_Mouse().y + m_vDragStart.y;

		D3DXVECTOR3 vCurPos = GetMPos();
		vCurPos = vCurPos + vDiff;
		SetMPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}
}

void DH_Item::MouseLDown()
{
	m_vDragStart = Get_Mouse();
}

void DH_Item::MouseLUp()
{
	D3DXVECTOR3 fMouse = Get_Mouse();

	//원하는 최상위 부모를 m_Inven 에 담기
	auto& AllUI = DH_OBJMgr::Get_Instance()->Get_UI();
	for (auto& _UIInven : AllUI)
	{
		if (L"Inventory" == _UIInven->GetName())
		{
			m_Inven = dynamic_cast<DH_UI*>(_UIInven);
		}
	}
	//베이스 부모의 자식버튼을 순회한다.
	for (auto& Btn : m_Inven->GetChildUI())
	{
		//버튼 위에 마우스가 있으면?
		if (Btn->IsMouseOn())
		{
			//현재 부모의 자식배열에서 자신을 삭제
			auto& oldParent = *GetParent();
			auto& oldChildList = oldParent.GetChildUI();
			oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
			//새로운 부모 연결
			Btn->AddParent(this);
			SetMPos(D3DXVECTOR3{ GetParent()->GetMPos() });
		}
		else
			SetMPos(GetParent()->GetMPos());
	}
}

void DH_Item::MouseLClicked()
{
}

void DH_Item::Initialize()
{
#pragma region 아이템 이미지 로드

	//악세서리
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Acce.png", TEX_SINGLE, L"00.Acce", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Band.png", TEX_SINGLE, L"00.Band", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Heart.png", TEX_SINGLE, L"00.Heart", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Neck.png", TEX_SINGLE, L"00.Neck", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Ring1.png", TEX_SINGLE, L"00.Ring1", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Ring2.png", TEX_SINGLE, L"00.Ring2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Ring3.png", TEX_SINGLE, L"00.Ring3", nullptr, 0);

	//방어구
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/00.Muffler.png", TEX_SINGLE, L"00.Muffler", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/01.Belt2.png", TEX_SINGLE, L"01.Belt2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/01.Belt3.png", TEX_SINGLE, L"01.Belt3", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/02.Coat2.png", TEX_SINGLE, L"02.Coat2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/03.Coat3.png", TEX_SINGLE, L"03.Coat3", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/04.Neck2.png", TEX_SINGLE, L"04.Neck2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/05.Neck3.png", TEX_SINGLE, L"05.Neck3", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/06.Pant2.png", TEX_SINGLE, L"06.Pant2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/07.Pant3.png", TEX_SINGLE, L"07.Pant3", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/08.Shoes2.png", TEX_SINGLE, L"08.Shoes2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/09.Shoes3.png", TEX_SINGLE, L"09.Shoes3", nullptr, 0);

	//소비아이템
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/00.Berry.png", TEX_SINGLE, L"00.Berry", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/01.Bread.png", TEX_SINGLE, L"01.Bread", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/02.Potion.png", TEX_SINGLE, L"02.Potion", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/03.Meat.png", TEX_SINGLE, L"03.Meat", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/04.Flower.png", TEX_SINGLE, L"04.Flower", nullptr, 0);

	//무기
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Weapon01.png", TEX_SINGLE, L"Weapon01", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Weapon02.png", TEX_SINGLE, L"Weapon02", nullptr, 0);

#pragma endregion
}

void DH_Item::Render()
{
	if (!m_bVisible)
		return;

	D3DXVECTOR3 vPinalPos = GetPinalPos();

	//이미지 키 가져오기 밑 중심설정
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_ImageKey, nullptr);
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
			pTexInfo->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	RenderChild();
}


void DH_Item::Update()
{
	DH_UI::Update();
}
