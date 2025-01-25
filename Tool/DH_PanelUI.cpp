#include "pch.h"
#include "DH_PanelUI.h"

DH_PanelUI::DH_PanelUI() : m_vDragStart(D3DXVECTOR3{0.f, 0.f, 0.f})
{
}

DH_PanelUI::~DH_PanelUI()
{
}

void DH_PanelUI::MouseOn()
{
	if (IsbLDown())
	{
		//마우스 좌표 가져오기
		D3DXVECTOR3 vDiff = Get_Mouse();

		D3DXVECTOR3 vCurPos = GetPos();
		vCurPos = vCurPos + vDiff;
		SetPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}
}

void DH_PanelUI::MouseLDown()
{
	m_vDragStart = Get_Mouse();
}

void DH_PanelUI::MouseLUp()
{
}

void DH_PanelUI::MouseLClicked()
{
}
