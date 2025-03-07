#pragma once
#include "DH_BtnUI.h"
class DH_Skill :  public DH_BtnUI
{
public:
	DH_Skill();
	virtual ~DH_Skill();

public:
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

public:
	virtual void Initialize();
	virtual void Render();
	virtual void Update();

public:
	void	SetImageKey(CString _Key) { m_ImageKey = _Key; }
	void	MouseDrag(DH_UI* _UI);

public:
	void		SetSkillData(SKILLDATA* _SKILLDATA) { m_pSkillDATA = _SKILLDATA; }
	SKILLDATA*	GetSkillData() { return m_pSkillDATA; }

private:
	D3DXVECTOR3	m_vDragStart;
	CString m_ImageKey;
	DH_UI* m_Skill;
	DH_UI* m_Interface;

	SKILLDATA* m_pSkillDATA;

	bool m_bDragOn;
	int m_PreviousIndex;
};

