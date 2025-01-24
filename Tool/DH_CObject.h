#pragma once
#include "Include.h"
#include "DH_Define.h"

class DH_CObject
{
public:
	DH_CObject();
	virtual  ~DH_CObject();

public:
	virtual int			Update();
	virtual void		LateUpdate();
	virtual void		Initialize();
	virtual void		Render(HDC hDC);
	virtual void		Release();

public:
	//GET 함수들
	tObjInfo* Get_Info_Pointer() { return &m_tObjInfo; }
	D3DXVECTOR3			GetPos() { return m_tObjInfo.tPos; }
	D3DXVECTOR3			GetScale() { return m_tObjInfo.tScale; }
	bool				GetbArrive() { return m_bArrive; }
	bool				GetbFalling() { return m_bFalling; }
	const wstring		GetName() { return m_strName; }
	bool				GetbJump() { return m_bJump; }
	bool				GetbDoubleJump() { return m_bDoubleJump; }
	float				GetfJumpSpeed() { return m_fJumpSpeed; }
	float				GetfTime() { return m_fTime; }
	float				GetfGravity() { return m_fGravity; }
	int					GetiDirection() { return m_iDirection; }
	float				GetiHP() { return m_iHP; }
	float				GetfSpeed() { return m_fSpeed; }
	STATE				GeteState() { return m_eCurState; }
	bool				GetbJumpSwitch() { return m_bJumpSwitch; }
	bool				GetbMoveFrame() { return m_bMoveFrame; }

public:
	//SET 함수들
	void		SetPos(D3DXVECTOR3 _Set) { m_tObjInfo.tPos = _Set; }
	void		SetScale(D3DXVECTOR3 _Set) { m_tObjInfo.tScale = _Set; }
	void		SetName(const wstring& _strName) { m_strName = _strName; }
	void		SetbFalling(bool _bFall) { m_bFalling = _bFall; }
	void		SetbJump(bool _bjump) { m_bJump = _bjump; }
	void		SetbDoubleJump(bool _bDjump) { m_bDoubleJump = _bDjump; }
	void		SetfJumpSpeed(float _fJSpeed) { m_fJumpSpeed = _fJSpeed; }
	void		SetfTime(float _fTime) { m_fTime = _fTime; }
	void		SeteState(STATE _state) { m_eCurState = _state; }
	void		SetfGravity(float _fVel) { m_fGravity = _fVel; }
	void		SetiDirection(int _iDir) { m_iDirection = _iDir; }
	void		SetbArrive(bool _arrive) { m_bArrive = _arrive; }
	void		SetiHP(float _iHP) { m_iHP = _iHP; }
	void		SetfSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void		SetbJumpSwitch(bool _Switch) { m_bJumpSwitch = _Switch; }
	void		SetbMoveFrame(bool _Frame) { m_bMoveFrame = _Frame; }

public:
	//더하기 함수들
	void		AddPos(D3DXVECTOR3 _Set) { m_tObjInfo.tPos + _Set; }
	void		AddScale(D3DXVECTOR3 _Set) { m_tObjInfo.tScale + _Set; }
	void		AddfGravity(float _fVel) { m_fGravity += _fVel; }
	void		AddfJumpSpeed(float _fJump) { m_fJumpSpeed += _fJump; }
	void		AddfTime(float _fTime) { m_fTime += _fTime; }
	void		AddiHP(float _iHP) { m_iHP += _iHP; }
	void		AddfSpeed(float _fSpeed) { m_fSpeed += _fSpeed; }


private:
	wstring			m_strName;
	tObjInfo		m_tObjInfo;
	RECT			m_rRect;
	bool			m_bArrive;
	bool			m_bFalling;
	bool			m_bMoveFrame;
	bool			m_bJump;
	bool			m_bDoubleJump;
	bool			m_bJumpSwitch;
	float			m_fJumpSpeed;
	float			m_fTime;
	float			m_fSpeed;
	float			m_fGravity;
	float			m_iHP;
	int				m_iDirection;


	//애니메이션
	STATE				m_eCurState;
	STATE				m_ePreState;
	FRAME				m_tFrame;

};

