#include "pch.h"
#include "DH_Item.h"
#include "DH_OBJMgr.h"
#include "DH_BtnUI.h"
#include "DH_UI.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "DH_Inventory.h"
#include "DH_Interface.h"
#include "DH_Player.h"

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
		if (dynamic_cast<DH_BtnUI*>(GetParent())->GetbEquip())
		{
			//��Ŭ���ߴµ� ���ĭ�̶��?

			//���� �θ��� �ڽĹ迭���� �ڽ��� ����
			auto& oldParent = *GetParent();
			auto& oldChildList = oldParent.GetChildUI();
			oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
			//���ο� �θ� ����
			auto& Inventory = *GetParent()->GetParent();
			for (auto& BtnUI : Inventory.GetChildUI())
			{
				if (BtnUI->GetChildUI().size() == 0)
				{
					dynamic_cast<DH_BtnUI*>(BtnUI)->AddParent(this);
					//������ ����
					DH_Player::Get_Instance()->SetItemUpdate(true);
					SetMPos({ 0.f, 0.f, 0.f });

					m_bRDown = false;
					return;
				}
			}
		}
		// �ٲ㳢�� �ؾ���
		auto& Inventory = *GetParent()->GetParent();
		for (auto& BtnUI : Inventory.GetChildUI())
		{
			if (m_pITEMDATA->eItemPart == dynamic_cast<DH_BtnUI*>(BtnUI)->GeteItemParts())
			{
				//�ش� UI �� �̵�
				//���� �θ��� �ڽĹ迭���� �ڽ��� ����
				auto& oldParent = *GetParent();
				auto& oldChildList = oldParent.GetChildUI();
				oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
				//���ο� �θ� ����
				dynamic_cast<DH_BtnUI*>(BtnUI)->AddParent(this);
				//������ ����
				DH_Player::Get_Instance()->SetItemUpdate(true);
				SetMPos({ 0.f, 0.f, 0.f });

				//���� �ڽ��� ���� �� ����
				if (dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().size() > 1)
				{
					//Btn->GetChildUI().begin = ���� �ִ� ������
					oldParent.AddParent(dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().front());
					dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().erase(dynamic_cast<DH_BtnUI*>(BtnUI)->GetChildUI().begin());
				}
			}
		}

		m_bRDown = false;
	}
}

void DH_Item::MouseLDown()
{
	m_bDragOn = true;
	m_vDragStart = Get_Mouse();

	//���� �ǵڷ� ������
	auto& Btn = *GetParent();
	auto& Inven = *GetParent()->GetParent();
	auto& childUI = Inven.GetChildUI();

	for (size_t i = 0; i < childUI.size(); ++i)
	{
		if (childUI[i] == &Btn)
		{
			m_PreviousIndex = i;
			auto temp = childUI[i];   // ��� ����
			childUI.erase(childUI.begin() + i); // ���� ��ġ���� ����
			childUI.push_back(temp);  // ���� �߰�
			break;
		}
	}
}

void DH_Item::MouseLUp()
{
	m_bDragOn = false;

	//������ ���� ����ġ
	auto& Btn = *GetParent();
	auto& Inven = *GetParent()->GetParent();
	auto& childUI = Inven.GetChildUI();

	auto temp = childUI.back(); // �� �� ������ ����
	childUI.pop_back(); // �� �ڿ��� ����
	childUI.insert(childUI.begin() + m_PreviousIndex, temp); // ���� ��ġ�� ����


	D3DXVECTOR3 fMouse = Get_Mouse();
	
	//m_Inven ����
	m_Inven = dynamic_cast<DH_UI*>(GetParent()->GetParent());

	//���� �θ� ��ųâ�϶�
	if (GetParent()->GetParent()->GetName() == L"Inventory")
	{
		//���̽� �θ��� �ڽĹ�ư�� ��ȸ�Ѵ�.
		for (auto& Btn : m_Inven->GetChildUI())
		{
			//��ưUI ���� ���콺�� ������? && ��ư�� ETC ���?
			if (Btn->IsMouseOn() && dynamic_cast<DH_BtnUI*>(Btn)->GeteItemParts() == ITEMPARTS::ETC)
			{
				//���� �θ��� �ڽĹ迭���� �ڽ��� ����
				auto& oldParent = *GetParent();
				auto& oldChildList = oldParent.GetChildUI();
				oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
				//���ο� �θ� ����
				Btn->AddParent(this);
				//������ ����
				DH_Player::Get_Instance()->SetItemUpdate(true);
				SetMPos({ 0.f, 0.f, 0.f });

				//���� �ڽ��� ���� �� ����
				if (Btn->GetChildUI().size() > 1)
				{
					//Btn->GetChildUI().begin = ���� �ִ� ������
					oldParent.AddParent(Btn->GetChildUI().front());
					Btn->GetChildUI().erase(Btn->GetChildUI().begin());
				}
			}
			//���� ���콺�� �÷����µ� ���ĭ�̴�?
			else if (Btn->IsMouseOn() && dynamic_cast<DH_BtnUI*>(Btn)->GetbEquip())
			{
				//���ĭ�� �������� ������ ����?
				if (dynamic_cast<DH_BtnUI*>(Btn)->GeteItemParts() == m_pITEMDATA->eItemPart)
				{
					//���� �θ��� �ڽĹ迭���� �ڽ��� ����
					auto& oldParent = *GetParent();
					auto& oldChildList = oldParent.GetChildUI();
					oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
					//���ο� �θ� ����
					Btn->AddParent(this);
					//������ ����
					DH_Player::Get_Instance()->SetItemUpdate(true);
					SetMPos({ 0.f, 0.f, 0.f });

					//���� �ڽ��� ���� �� ����
					if (Btn->GetChildUI().size() > 1)
					{
						//Btn->GetChildUI().begin = ���� �ִ� ������
						oldParent.AddParent(Btn->GetChildUI().front());
						Btn->GetChildUI().erase(Btn->GetChildUI().begin());
					}
				}
				else
				{
					SetMPos({ 0.f, 0.f, 0.f });
				}
			}
			else
			{
				SetMPos({ 0.f, 0.f, 0.f });
			}
		}

		//�������̽� â ��������
		DH_Interface* InterfaceUI = dynamic_cast<DH_Inventory*>(m_Inven)->GetInterface();
		MouseDrag(InterfaceUI);

	}
	//�θ� �������̽�â�� ��
	else if (GetParent()->GetParent()->GetName() == L"Interface")
	{
		m_Inven = GetParent()->GetParent();
		DH_Inventory* InvenUI = dynamic_cast<DH_Interface*>(m_Inven)->GetInventory();
	
		MouseDrag(m_Inven);
		MouseDrag(InvenUI);
	}
}

void DH_Item::MouseLClicked()
{
}

void DH_Item::Initialize()
{
#pragma region ������ �̹��� �ε�

	//����
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot3.png", TEX_SINGLE, L"ItemSlot3", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot4.png", TEX_SINGLE, L"ItemSlot4", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot5.png", TEX_SINGLE, L"ItemSlot5", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemSlot6.png", TEX_SINGLE, L"ItemSlot6", nullptr, 0);

	//�Ǽ�����
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

	//��
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

	//�Һ������
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

	//����
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Weapon01.png", TEX_SINGLE, L"Weapon01", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Weapon02.png", TEX_SINGLE, L"Weapon02", nullptr, 0);

	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Clicked/Weapon01.png", TEX_SINGLE, L"Weapon01_Clicked", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Item/Weapon/Clicked/Weapon02.png", TEX_SINGLE, L"Weapon02_Clicked", nullptr, 0);

#pragma endregion

	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Interface/ItemINFO.png", TEX_SINGLE, L"ItemINFO", nullptr, 0);
}

void DH_Item::Render()
{
	if (!m_bVisible)
		return;

	D3DXVECTOR3 vPinalPos = GetPinalPos();

	//�̹��� Ű �������� �� �߽ɼ���
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_ImageKey, nullptr);
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

	if(m_bMouseOn)
		RenderItemInfo(L"ItemINFO");

	RenderChild();
}


void DH_Item::Update()
{
	if (m_bDragOn)
	{
		//���콺�� �ö� ���¿��� ��ư�� ������ ������
		D3DXVECTOR3 vDiff = { 0.f, 0.f, 0.f };
		vDiff.x = Get_Mouse().x - m_vDragStart.x;
		vDiff.y = Get_Mouse().y - m_vDragStart.y;

		D3DXVECTOR3 vCurPos = GetMPos();
		vCurPos = vCurPos + vDiff;
		SetMPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}

	DH_UI::Update();
}

void DH_Item::MouseDrag(DH_UI* _UI)
{
	//���̽� �θ��� �ڽĹ�ư�� ��ȸ�Ѵ�.
	for (auto& Btn : _UI->GetChildUI())
	{
		//��ưUI ���� ���콺�� ������? ��ư�� �����̶��?
		if (Btn->IsMouseOn() && dynamic_cast<DH_BtnUI*>(Btn)->GeteItemParts() == m_pITEMDATA->eItemPart)
		{
			//���� �θ��� �ڽĹ迭���� �ڽ��� ����
			auto& oldParent = *GetParent();
			auto& oldChildList = oldParent.GetChildUI();
			oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
			//���ο� �θ� ����
			Btn->AddParent(this);
			//������ ����
			SetMPos({ 0.f, 0.f, 0.f });

			//���� �ڽ��� ���� �� ����
			if (Btn->GetChildUI().size() > 1)
			{
				//Btn->GetChildUI().begin = ���� �ִ� ������
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

void DH_Item::RenderItemInfo(TCHAR* _Key)
{
	//�̹��� Ű �������� �� �߽ɼ���
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(_Key, nullptr);
	float	fCenterX = (float)pTexInfo->tImgInfo.Width;
	float	fCenterY = (float)pTexInfo->tImgInfo.Height;
	D3DXVECTOR3	vTemp{ fCenterX + 2.f, +1.f, 0.f };
	D3DXVECTOR3	vMouse = Get_Mouse();

	//���� ��� �����ֱ�
	D3DXMATRIX	matWorld, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, vMouse.x, vMouse.y, vMouse.z);
	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(
		pTexInfo->pTexture,
		nullptr,
		&vTemp,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));


#pragma region �󼼺��� �������

	TCHAR	szBufName[MIN_STR] = L"";
	swprintf_s(szBufName, MIN_STR, L"%s", m_pITEMDATA->strExplan.GetString());

	// �ؽ�Ʈ�� ������ ���� �簢�� ����
	RECT	RectBufName{ -150,0,0,60 };
	int lineHeight = 30; // �� ���� ����

	// �ؽ�Ʈ ���
	const TCHAR* pText = szBufName;
	RECT currentRect = RectBufName;

	
	int charsPrinted = CDevice::Get_Instance()->Get_Font2()->DrawTextW(
		CDevice::Get_Instance()->Get_Sprite(),
		pText,
		-1, // ��ü ���� ���
		&currentRect,
		DT_WORDBREAK | DT_CENTER, // �ڵ� �ٹٲ�
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

#pragma endregion

}