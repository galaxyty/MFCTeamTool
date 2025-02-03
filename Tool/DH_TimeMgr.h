#pragma once

#include "Include.h"

class DH_TimeMgr
{
	DECLARE_SINGLETON(DH_TimeMgr)

private:
	DH_TimeMgr();
	~DH_TimeMgr();

public:
	float			Get_TimeDelta() { return m_fTimeDelta; }

public:
	void			Initialize();
	void			Update();

private:
	LARGE_INTEGER			m_CurTime;
	LARGE_INTEGER			m_OldTime;
	LARGE_INTEGER			m_OriginTime;
	LARGE_INTEGER			m_CpuTick;

	float					m_fTimeDelta;
};