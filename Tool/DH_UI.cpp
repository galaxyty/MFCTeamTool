#include "pch.h"
#include "DH_UI.h"

DH_UI::DH_UI() : m_vFinalPos(D3DXVECTOR3{}), m_pParnetUI(nullptr), m_bMouseOn(false), m_bLDown(false), m_bVisible(true)
{
}

DH_UI::~DH_UI()
{
}

void DH_UI::Update()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 업데이트하지 않음

	MouseOnCheck();
	UpdateChild();

}

void DH_UI::Render()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 렌더링하지 않음

	D3DXVECTOR3 vPos = GetPinalPos();


	if (m_bLDown)
	{
		//클릭 했을 때
	}
	else
	{
		//클릭 안했을 때 
	}

	RenderChild();
}

void DH_UI::Initialize()
{
}

void DH_UI::LateUpdate()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 업데이트하지 않음


	//UI 의 최종 좌표를 구한다.
	m_vFinalPos = GetMPos();

	if (GetParent())
	{
		//부모의 파이널 포즈를 불러와서 현재 포지션과 더해줌
		D3DXVECTOR3 vParentPos = GetParent()->GetPinalPos();
		m_vFinalPos = m_vFinalPos + vParentPos;
	}
	LateUpdateChild();
}

void DH_UI::MouseOn()
{

}

void DH_UI::MouseLDown()
{

}

void DH_UI::MouseLUp()
{

}

void DH_UI::MouseLClicked()
{

}


void DH_UI::MouseOnCheck()
{
	//마우스 좌표 가져오기
	D3DXVECTOR3 vMousePos = Get_Mouse();

	if (m_vFinalPos.x - GetScale().x / 2 <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + GetScale().x / 2
		&& m_vFinalPos.y - GetScale().y / 2 <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + GetScale().y / 2)
	{
		//마우스가 올라가있음
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void DH_UI::UpdateChild()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void DH_UI::RenderChild()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render();
	}
}

void DH_UI::LateUpdateChild()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->LateUpdate();
	}
}
