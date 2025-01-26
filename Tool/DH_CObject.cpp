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

//오브젝트 그리기
void DH_CObject::Render(CToolView* pMainView)
{
}

void DH_CObject::Release()
{
}



void DH_CObject::LateUpdate()
{
}

void DH_CObject::Set_Ratio(D3DXMATRIX* pOut, float _fX, float _fY)
{
	pOut->_11 *= _fX;
	pOut->_21 *= _fX;
	pOut->_31 *= _fX;
	pOut->_41 *= _fX;

	pOut->_12 *= _fY;
	pOut->_22 *= _fY;
	pOut->_32 *= _fY;
	pOut->_42 *= _fY;

}


// 아직 미사용 함수들
void DH_CObject::Update() {}
void DH_CObject::Initialize() {}
