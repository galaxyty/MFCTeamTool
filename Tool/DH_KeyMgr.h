#pragma once
#include "Include.h"

class DH_KeyMgr
{
private:
	DH_KeyMgr();
	~DH_KeyMgr();

public:
	bool		Key_Pressing(int _iKey);
	bool		Key_Down(int _iKey);		// µü ÇÑ ¹ø ´­·¶À» ¶§
	bool		Key_Up(int _iKey);			// ´­·¶´Ù°¡ ¶ÃÀ» ¶§
	void		Update();

public:
	static DH_KeyMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new DH_KeyMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static DH_KeyMgr* m_pInstance;
	bool	m_bKeyState[VK_MAX];

};

