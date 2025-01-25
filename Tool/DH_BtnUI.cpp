#include "pch.h"
#include "DH_BtnUI.h"

DH_BtnUI::DH_BtnUI() : m_bClicked(false)
{
}

DH_BtnUI::~DH_BtnUI()
{
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

void DH_BtnUI::LateUpdate()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 업데이트하지 않음

	SetPinalPos(GetPos());

	if (GetParent())
	{
		//부모의 파이널 포즈를 불러와서 현재 포지션과 더해줌
		D3DXVECTOR3 vParentPos = GetParent()->GetPinalPos();
		SetPinalPos(GetPos() + vParentPos);
	}
	LateUpdateChild();
}
