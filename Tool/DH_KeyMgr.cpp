#include "pch.h"
#include "DH_KeyMgr.h"
DH_KeyMgr* DH_KeyMgr::m_pInstance = nullptr;

DH_KeyMgr::DH_KeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

DH_KeyMgr::~DH_KeyMgr()
{
}

bool DH_KeyMgr::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}
// 이전에 눌림이 없고, 현재 눌렸을 경우
bool DH_KeyMgr::Key_Down(int _iKey)
{
	if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

// 이전에 눌림이 있고, 현재 누르지 않을 경우
bool DH_KeyMgr::Key_Up(int _iKey)
{
	if ((m_bKeyState[_iKey]) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

void DH_KeyMgr::Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if ((!m_bKeyState[i]) && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];

		if ((m_bKeyState[i]) && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}