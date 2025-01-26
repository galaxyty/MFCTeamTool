#include "pch.h"
#include "DH_KeyMgr.h"

IMPLEMENT_SINGLETON(DH_KeyMgr);


int g_arrVK[static_cast<int>(KEY::LAST)]
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'Z', 'X', 'C', 'V', 'B',
	VK_MENU, VK_CONTROL, VK_LSHIFT, VK_SPACE, VK_RETURN, VK_TAB, VK_F1, VK_ESCAPE,
	VK_LBUTTON, VK_RBUTTON
	//LAST
};


DH_KeyMgr::DH_KeyMgr()
{

}
DH_KeyMgr::~DH_KeyMgr()
{

}

void DH_KeyMgr::Initialize()
{
	for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
	{
		m_veckey.push_back(tKey_Info{ KEY_STATE::NONE, false });
	}
}

void DH_KeyMgr::Update()
{
	HWND hWnd = GetFocus();
	if (hWnd != nullptr)
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// �� ������ �����־���?
				if (m_veckey[i].bPrev)
				{
					m_veckey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					//�� ���� ���������ʾҴ�?
					m_veckey[i].eState = KEY_STATE::TAP;
				}
				m_veckey[i].bPrev = true;
			}
			else
			{
				//�ƹ��͵� �ȴ�������
				if (m_veckey[i].bPrev)
				{
					//������ ���Ⱦ���? ���� �� ��
					m_veckey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					//�ƹ��͵� �ȴ����� �� ������ �ȴ���
					m_veckey[i].eState = KEY_STATE::NONE;
				}
				m_veckey[i].bPrev = false;
			}
		}
	}
	else
	{
		//�� �����츦 ���� �������� ��
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			m_veckey[i].bPrev = false;
			if (m_veckey[i].eState == KEY_STATE::TAP || m_veckey[i].eState == KEY_STATE::AWAY)
			{
				m_veckey[i].eState = KEY_STATE::AWAY;
			}
			else if (m_veckey[i].eState == KEY_STATE::AWAY)
			{
				m_veckey[i].eState = KEY_STATE::NONE;
			}
		}
	}
}
