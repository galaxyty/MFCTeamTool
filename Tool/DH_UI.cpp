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
		return; // UI�� ��Ȱ��ȭ ���¶�� ������Ʈ���� ����

	MouseOnCheck();
	UpdateChild();

}

void DH_UI::Render()
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ���������� ����

	D3DXVECTOR3 vPos = GetPinalPos();


	if (m_bLDown)
	{
		//Ŭ�� ���� ��
	}
	else
	{
		//Ŭ�� ������ �� 
	}

	RenderChild();
}

void DH_UI::Initialize()
{
}

void DH_UI::LateUpdate()
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ������Ʈ���� ����


	//UI �� ���� ��ǥ�� ���Ѵ�.
	m_vFinalPos = GetMPos();

	if (GetParent())
	{
		//�θ��� ���̳� ��� �ҷ��ͼ� ���� �����ǰ� ������
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
	//���콺 ��ǥ ��������
	D3DXVECTOR3 vMousePos = Get_Mouse();

	if (m_vFinalPos.x - GetScale().x / 2 <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + GetScale().x / 2
		&& m_vFinalPos.y - GetScale().y / 2 <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + GetScale().y / 2)
	{
		//���콺�� �ö�����
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
