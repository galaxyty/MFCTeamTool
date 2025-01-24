#pragma once
#include "DH_UI.h"

class DH_UIMgr
{
private:
	DH_UIMgr();
	DH_UIMgr(const DH_UIMgr& rhs) = delete;
	DH_UIMgr operator =(DH_UIMgr& rObj) = delete;
	~DH_UIMgr();


public:
	void Update();
	void Initialize();
	void SetFocusedUI(DH_UI* _pUI);



private:
	DH_UI* GetTargetedUI(DH_UI* _pParentUI);
	DH_UI* GetFocusUI();


public:
	static DH_UIMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new DH_UIMgr;

		return m_pInstance;
	}

	static void		DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void SetUION(bool on) { m_UION = on; }
	bool GetUION() { return m_UION; }

private:
	static DH_UIMgr* m_pInstance;
	DH_UI* m_FocusedUI;
	bool	m_UION;

};
