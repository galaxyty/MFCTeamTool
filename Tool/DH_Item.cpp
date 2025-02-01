#include "pch.h"
#include "DH_Item.h"
#include "DH_OBJMgr.h"
#include "DH_BtnUI.h"
#include "DH_UI.h"
#include "CTextureMgr.h"
#include "CDevice.h"

DH_Item::DH_Item() : m_Inven(nullptr), m_ImageKey(), m_bDragOn(false), m_pITEMDATA(nullptr), m_PreviousIndex(0)
{
	ZeroMemory(m_vDragStart, sizeof(D3DXVECTOR3));
}

DH_Item::~DH_Item()
{
}

void DH_Item::MouseOn()
{
	if (m_bRDown)
	{
		// 바꿔끼기 해야함
		auto& Inventory = *GetParent()->GetParent();
		for (auto& BtnUI : Inventory.GetChildUI())
		{
			if (m_pITEMDATA->eItemPart == dynamic_cast<DH_BtnUI*>(BtnUI)->GeteItemParts())
			{
				//해당 UI 로 이동
				//현재 부모의 자식배열에서 자신을 삭제
				auto& oldParent = *GetParent();
				auto& oldChildList = oldParent.GetChildUI();
				oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
				//새로운 부모 연결
				dynamic_cast<DH_BtnUI*>(BtnUI)->AddParent(this);
				//포지션 셋팅
				SetMPos({ 0.f, 0.f, 0.f });

				//기존 자식을 삭제 후 스왑
				if (dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().size() > 1)
				{
					//Btn->GetChildUI().begin = 원래 있던 아이템
					oldParent.AddParent(dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().front());
					dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().erase(dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().begin());
				}
			}
		}
	}


	if (m_bDragOn)
	{
		//마우스가 올라간 상태에서 버튼이 눌리면 움직임
		D3DXVECTOR3 vDiff = {0.f, 0.f, 0.f};
		vDiff.x = Get_Mouse().x - m_vDragStart.x;
		vDiff.y = Get_Mouse().y - m_vDragStart.y;

		D3DXVECTOR3 vCurPos = GetMPos();
		vCurPos = vCurPos + vDiff;
		SetMPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}
}

void DH_Item::MouseLDown()
{
	m_bDragOn = true;
	m_vDragStart = Get_Mouse();

	//벡터 맨뒤로 보내기
	auto& Btn = *GetParent();
	auto& Inven = *GetParent()->GetParent();
	auto& childUI = Inven.GetChildUI();

	for (size_t i = 0; i < childUI.size(); ++i)
	{
		if (childUI[i] == &Btn)
		{
			m_PreviousIndex = i;
			auto temp = childUI[i];   // 요소 복사
			childUI.erase(childUI.begin() + i); // 기존 위치에서 제거
			childUI.push_back(temp);  // 끝에 추가
			break;
		}
	}
}

void DH_Item::MouseLUp()
{
	m_bDragOn = false;

	//아이템 순서 원위치
	auto& Btn = *GetParent();
	auto& Inven = *GetParent()->GetParent();
	auto& childUI = Inven.GetChildUI();

	auto temp = childUI.back(); // 맨 뒤 아이템 복사
	childUI.pop_back(); // 맨 뒤에서 제거
	childUI.insert(childUI.begin() + m_PreviousIndex, temp); // 이전 위치로 복귀


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
			//포지션 셋팅
			SetMPos({0.f, 0.f, 0.f});

			//기존 자식을 삭제 후 스왑
			if (Btn->GetChildUI().size() > 1)
			{
				//Btn->GetChildUI().begin = 원래 있던 아이템
				oldParent.AddParent(Btn->GetChildUI().front());
				Btn->GetChildUI().erase(Btn->GetChildUI().begin());
			}
		}
		else
		{
			SetMPos({ 0.f, 0.f, 0.f });
		}
	}
}

void DH_Item::MouseLClicked()
{
}

void DH_Item::Initialize()
{
#pragma region 아이템 이미지 로드

	//슬룻
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot3.png", TEX_SINGLE, L"ItemSlot3", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot4.png", TEX_SINGLE, L"ItemSlot4", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot5.png", TEX_SINGLE, L"ItemSlot5", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot6.png", TEX_SINGLE, L"ItemSlot6", nullptr, 0);

	//악세서리
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Band.png", TEX_SINGLE, L"00.Band", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Heart.png", TEX_SINGLE, L"00.Heart", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Neck.png", TEX_SINGLE, L"00.Neck", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Ring1.png", TEX_SINGLE, L"00.Ring1", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Ring2.png", TEX_SINGLE, L"00.Ring2", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/00.Ring3.png", TEX_SINGLE, L"00.Ring3", nullptr, 0);

	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/Clicked/00.Band.png", TEX_SINGLE, L"00.Band_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/Clicked/00.Heart.png", TEX_SINGLE, L"00.Heart_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/Clicked/00.Neck.png", TEX_SINGLE, L"00.Neck_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/Clicked/00.Ring1.png", TEX_SINGLE, L"00.Ring1_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/Clicked/00.Ring2.png", TEX_SINGLE, L"00.Ring2_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Acce/Clicked/00.Ring3.png", TEX_SINGLE, L"00.Ring3_Clicked", nullptr, 0);

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

	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/00.Muffler.png", TEX_SINGLE, L"00.Muffler_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/01.Belt2.png", TEX_SINGLE, L"01.Belt2_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/01.Belt3.png", TEX_SINGLE, L"01.Belt3_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/02.Coat2.png", TEX_SINGLE, L"02.Coat2_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/03.Coat3.png", TEX_SINGLE, L"03.Coat3_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/04.Neck2.png", TEX_SINGLE, L"04.Neck2_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/05.Neck3.png", TEX_SINGLE, L"05.Neck3_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/06.Pant2.png", TEX_SINGLE, L"06.Pant2_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/07.Pant3.png", TEX_SINGLE, L"07.Pant3_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/08.Shoes2.png", TEX_SINGLE, L"08.Shoes2_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Amor/Clicked/09.Shoes3.png", TEX_SINGLE, L"09.Shoes3_Clicked", nullptr, 0);

	//소비아이템
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/00.Berry.png", TEX_SINGLE, L"00.Berry", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/01.Bread.png", TEX_SINGLE, L"01.Bread", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/02.Potion.png", TEX_SINGLE, L"02.Potion", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/03.Meat.png", TEX_SINGLE, L"03.Meat", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/04.Flower.png", TEX_SINGLE, L"04.Flower", nullptr, 0);

	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/Clicked/00.Berry.png", TEX_SINGLE, L"00.Berry_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/Clicked/01.Bread.png", TEX_SINGLE, L"01.Bread_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/Clicked/02.Potion.png", TEX_SINGLE, L"02.Potion_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/Clicked/03.Meat.png", TEX_SINGLE, L"03.Meat_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Potion/Clicked/04.Flower.png", TEX_SINGLE, L"04.Flower_Clicked", nullptr, 0);

	//무기
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Weapon01.png", TEX_SINGLE, L"Weapon01", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Weapon02.png", TEX_SINGLE, L"Weapon02", nullptr, 0);

	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Clicked/Weapon01.png", TEX_SINGLE, L"Weapon01_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Clicked/Weapon02.png", TEX_SINGLE, L"Weapon02_Clicked", nullptr, 0);

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
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_ImageKey + L"_Clicked", nullptr);

		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		const TEXINFO* pUIInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BtnUI", nullptr);
		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pUIInfo->pTexture,
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

		if (m_sITEM == ITEM::WEAPON)
		{
			const TEXINFO* pUIInfo = CTextureMgr::Get_Instance()->Get_Texture(L"ItemSlot4", nullptr);
			CDevice::Get_Instance()->Get_Sprite()->Draw(
				pUIInfo->pTexture,
				nullptr,
				&vTemp,
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (m_sITEM == ITEM::AMOR)
		{
			const TEXINFO* pUIInfo = CTextureMgr::Get_Instance()->Get_Texture(L"ItemSlot3", nullptr);
			CDevice::Get_Instance()->Get_Sprite()->Draw(
				pUIInfo->pTexture,
				nullptr,
				&vTemp,
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (m_sITEM == ITEM::POTION)
		{
			const TEXINFO* pUIInfo = CTextureMgr::Get_Instance()->Get_Texture(L"ItemSlot5", nullptr);
			CDevice::Get_Instance()->Get_Sprite()->Draw(
				pUIInfo->pTexture,
				nullptr,
				&vTemp,
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (m_sITEM == ITEM::ACCE)
		{
			const TEXINFO* pUIInfo = CTextureMgr::Get_Instance()->Get_Texture(L"ItemSlot6", nullptr);
			CDevice::Get_Instance()->Get_Sprite()->Draw(
				pUIInfo->pTexture,
				nullptr,
				&vTemp,
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	RenderChild();
}


void DH_Item::Update()
{
	DH_UI::Update();
}
