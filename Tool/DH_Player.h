#pragma once
#include "DH_CObject.h"
#include "Include.h"
#include "DH_Define.h"

class DH_Inventory;
class DH_MyState;
class DH_Interface;
class CPlayerTool;
class DH_HITBOX;



class DH_Player :  public DH_CObject
{
	DECLARE_SINGLETON(DH_Player)

public:
	DH_Player();
	virtual ~DH_Player();

public:
	virtual void		Update();
	virtual void		LateUpdate();
	virtual void		Initialize();
	virtual void		Render(CToolView* pMainView);
	virtual void		Release();

public:
	PLAYERDATA GetPlayerData() const{	return m_PLAYERDATA; }
	DH_Inventory* GetInventory() const{	return m_Inventory;	}
	DH_MyState* GetMyState() const{	return m_MyState; }
	DH_Interface* GetInterface() const{	return m_Interface;	}
	CPlayerTool* GetPlayerTool() const{	return m_PlayerTool;}
	bool GetItemUpdate() const { return m_ItemUpdate; }
	
public:
	void SetPlayerData(PLAYERDATA* pData){	m_PLAYERDATA = *pData; }
	void SetInventory(DH_Inventory* inventory){	m_Inventory = inventory; }
	void SetPlayerTool(CPlayerTool* PlayerTool){ m_PlayerTool = PlayerTool; }
	void SetMyState(DH_MyState* myState){ m_MyState = myState; }
	void SetInterface(DH_Interface* interfaceObj){	m_Interface = interfaceObj;	}
	void SetItemUpdate(bool update) {m_ItemUpdate = update;	}
	void SetEndFrame(wstring _state);
	void KeyInput();

	wstring SetSkillName(wstring _Name);
	void SpawnHitBox();


private:
	DH_Inventory*	m_Inventory;
	DH_MyState*		m_MyState;
	DH_Interface*	m_Interface;
	CPlayerTool*	m_PlayerTool;

private:
	PLAYERDATA		m_PLAYERDATA;
	PLAYERDATA		m_RESETPLAYERDATA;

private:
	bool			m_ItemUpdate;
	int				m_previousFrame = -1;
	float			m_fFrameCal;
	float			m_fReverse;
	int				m_iFrame;

	wstring			m_State;
	vector<HITBOX*> m_SpawnedHitBoxes;
	vector<DH_HITBOX*> m_vecHitBox;
};

