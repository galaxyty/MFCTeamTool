#include "pch.h"
#include "DH_Skill.h"
#include "DH_OBJMgr.h"
#include "DH_UI.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "DH_Interface.h"
#include "DH_SkillUI.h"
#include "DH_UIMgr.h"

DH_Skill::DH_Skill() : m_Skill(nullptr), m_ImageKey(), m_bDragOn(false), m_PreviousIndex(0), m_pSkillDATA(nullptr)
,m_Interface(nullptr)
{
	ZeroMemory(m_vDragStart, sizeof(D3DXVECTOR3));
}

DH_Skill::~DH_Skill()
{
}

void DH_Skill::MouseOn()
{
	
}

void DH_Skill::MouseLDown()
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

void DH_Skill::MouseLUp()
{
	m_bDragOn = false;

	//������ ���� ����ġ
	auto& Btn = *GetParent();
	auto& Inven = *GetParent()->GetParent();
	auto& childUI = Inven.GetChildUI();

	auto temp = childUI.back(); // �� �� ������ ����
	childUI.pop_back(); // �� �ڿ��� ����
	childUI.insert(childUI.begin() + m_PreviousIndex, temp); // ���� ��ġ�� ����

	//���� �θ� ��ųâ�϶�
	if (GetParent()->GetParent()->GetName() == L"Skill")
	{
		m_Skill = GetParent()->GetParent();
		DH_Interface* InterfaceUI = dynamic_cast<DH_SkillUI*>(m_Skill)->GetInterface();

		MouseDrag(m_Skill);
		MouseDrag(InterfaceUI);
	}
	//�θ� �������̽�â�� ��
	else if (GetParent()->GetParent()->GetName() == L"Interface")
	{
		m_Interface = GetParent()->GetParent();
		DH_SkillUI* SkillUI = dynamic_cast<DH_Interface*>(m_Interface)->GetSkillUI();

		MouseDrag(m_Interface);
		MouseDrag(SkillUI);
	}
}

void DH_Skill::MouseLClicked()
{
}

void DH_Skill::Initialize()
{
#pragma region ��ų �̹��� �ε�

	//ON
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/Gun_Hock.png", TEX_SINGLE, L"Gun_Hock", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/HeadShoot.png", TEX_SINGLE, L"HeadShoot", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/MahaKick.png", TEX_SINGLE, L"MahaKick", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/Moving_Shoot.png", TEX_SINGLE, L"Moving_Shoot", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/Random_Shoot.png", TEX_SINGLE, L"Random_Shoot", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/Rizing.png", TEX_SINGLE, L"Rizing", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/Seven'sFlow.png", TEX_SINGLE, L"Seven'sFlow", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/On/WindMill.png", TEX_SINGLE, L"WindMill", nullptr, 0);

	//OFF
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/Rizing_Off.png", TEX_SINGLE, L"Rizing_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/Gun_Hock_Off.png", TEX_SINGLE, L"Gun_Hock_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/HeadShoot_Off.png", TEX_SINGLE, L"HeadShoot_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/MahaKick_Off.png", TEX_SINGLE, L"MahaKick_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/Moving_Shoot_Off.png", TEX_SINGLE, L"Moving_Shoot_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/Random_Shoot_Off.png", TEX_SINGLE, L"Random_Shoot_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/Seven'sFlow_Off.png", TEX_SINGLE, L"Seven'sFlow_Off", nullptr, 0);
	CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Picked/Skill/Off/WindMill_Off.png", TEX_SINGLE, L"WindMill_Off", nullptr, 0);

#pragma endregion
}

void DH_Skill::Render()
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
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_ImageKey + L"_Off", nullptr);
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


		const TEXINFO* pUIInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BtnUI2", nullptr);
		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pUIInfo->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	RenderChild();
}


void DH_Skill::Update()
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

void DH_Skill::MouseDrag(DH_UI* _UI)
{
	//���̽� �θ��� �ڽĹ�ư�� ��ȸ�Ѵ�.
	for (auto& Btn : _UI->GetChildUI())
	{
		//��ưUI ���� ���콺�� ������?
		if (Btn->IsMouseOn() && dynamic_cast<DH_BtnUI*>(Btn)->GeteItemParts() == ITEMPARTS::SKILL)
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
			//DH_UIMgr::Get_Instance()->SetFocusedUI(_UI);
		}
		else
		{
			SetMPos({ 0.f, 0.f, 0.f });
		}
	}
}
