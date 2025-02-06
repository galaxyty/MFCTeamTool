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

void DH_Skill::MouseLUp()
{
	m_bDragOn = false;

	//아이템 순서 원위치
	auto& Btn = *GetParent();
	auto& Inven = *GetParent()->GetParent();
	auto& childUI = Inven.GetChildUI();

	auto temp = childUI.back(); // 맨 뒤 아이템 복사
	childUI.pop_back(); // 맨 뒤에서 제거
	childUI.insert(childUI.begin() + m_PreviousIndex, temp); // 이전 위치로 복귀

	//지금 부모가 스킬창일때
	if (GetParent()->GetParent()->GetName() == L"Skill")
	{
		m_Skill = GetParent()->GetParent();
		DH_Interface* InterfaceUI = dynamic_cast<DH_SkillUI*>(m_Skill)->GetInterface();

		MouseDrag(m_Skill);
		MouseDrag(InterfaceUI);
	}
	//부모가 인터페이스창일 때
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
#pragma region 스킬 이미지 로드

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
		//마우스가 올라간 상태에서 버튼이 눌리면 움직임
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
	//베이스 부모의 자식버튼을 순회한다.
	for (auto& Btn : _UI->GetChildUI())
	{
		//버튼UI 위에 마우스가 있으면?
		if (Btn->IsMouseOn() && dynamic_cast<DH_BtnUI*>(Btn)->GeteItemParts() == ITEMPARTS::SKILL)
		{
			//현재 부모의 자식배열에서 자신을 삭제
			auto& oldParent = *GetParent();
			auto& oldChildList = oldParent.GetChildUI();
			oldChildList.erase(remove(oldChildList.begin(), oldChildList.end(), this), oldChildList.end());
			//새로운 부모 연결
			Btn->AddParent(this);
			//포지션 셋팅
			SetMPos({ 0.f, 0.f, 0.f });

			//기존 자식을 삭제 후 스왑
			if (Btn->GetChildUI().size() > 1)
			{
				//Btn->GetChildUI().begin = 원래 있던 아이템
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
