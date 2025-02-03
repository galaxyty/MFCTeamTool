#include "pch.h"
#include "DH_TimeMgr.h"

IMPLEMENT_SINGLETON(DH_TimeMgr)

DH_TimeMgr::DH_TimeMgr()
	: m_fTimeDelta(0.f)
{
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OriginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}


DH_TimeMgr::~DH_TimeMgr()
{
}

void DH_TimeMgr::Initialize()
{
	//QueryPerformanceCounter : ���κ��尡 ���� ���ػ� Ÿ�̸��� ���� ���� ������ �Լ�

	QueryPerformanceCounter(&m_CurTime);		// 1000
	QueryPerformanceCounter(&m_OldTime);		// 1024

	QueryPerformanceCounter(&m_OriginTime);		// 1030
	// QueryPerformanceFrequency : ���ػ� Ÿ�̸��� ���ļ��� ������ �Լ�, ���⼭ ���ļ��� cpu�� �ʴ� Ŭ������ �ǹ�
	QueryPerformanceFrequency(&m_CpuTick);
}

void DH_TimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurTime);	// 1100

	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_OriginTime = m_CurTime;
	}

	m_fTimeDelta = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime = m_CurTime;
}