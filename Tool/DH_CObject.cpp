#include "pch.h"
#include "DH_CObject.h"


DH_CObject::DH_CObject() :
	m_bArrive(true), m_rRect(RECT{}), m_iDirection(0), m_eCurState(STATE::IDLE), m_ePreState(STATE::IDLE), m_tFrame(FRAME{}),
	m_bFalling(false), m_bJump(false), m_fJumpSpeed(16.f), m_fTime(0.f), m_fGravity(3.6f), m_bDoubleJump(false), m_iHP(100),
	m_fSpeed(3.0f), m_bJumpSwitch(false), m_bMoveFrame(true)
{
	ZeroMemory(&m_tObjInfo, sizeof(tObjInfo));
}

DH_CObject::~DH_CObject()
{
	Release();
}

//������Ʈ �׸���
void DH_CObject::Render(HDC hDC)
{
}

void DH_CObject::Release()
{
}



void DH_CObject::LateUpdate()
{
}

// ���� �̻�� �Լ���
int DH_CObject::Update() 
{
	return 0;
}
void DH_CObject::Initialize() {}
