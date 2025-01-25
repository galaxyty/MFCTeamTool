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
		return; // UI�� ��Ȱ��ȭ ���¶�� ������Ʈ���� ����

	SetPinalPos(GetPos());

	if (GetParent())
	{
		//�θ��� ���̳� ��� �ҷ��ͼ� ���� �����ǰ� ������
		D3DXVECTOR3 vParentPos = GetParent()->GetPinalPos();
		SetPinalPos(GetPos() + vParentPos);
	}
	LateUpdateChild();
}
