#pragma once
#include "include.h"

class CToolView;

class DH_LoopMgr
{
	DECLARE_SINGLETON(DH_LoopMgr)

private:
	DH_LoopMgr();
	~DH_LoopMgr();


public:
	void		Progress();

public:
	void		Update();
	void		LateUpdate();
	void		Render();
	void		Initialize();
	void		Release();

public:
	void		SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }

private:
	//FPS ¿ë
	ULONG64		m_dwTime;
	int			m_iFPS;
	TCHAR		m_wcBuff[32];


private:
	CToolView* m_pToolView;


};
