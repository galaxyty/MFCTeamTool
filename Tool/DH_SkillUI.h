#pragma once
#include "DH_PanelUI.h"
class DH_SkillUI :  public DH_PanelUI
{
public:
	DH_SkillUI();
	virtual ~DH_SkillUI();

public:
	virtual void Update();
	virtual void Render(CToolView* pMainView);
	virtual void Initialize();

public:
	virtual void MouseLDown();
	virtual void MouseOn();
	virtual void MouseLUp();
	virtual void MouseLClicked();

public:
	void	SetButten();

public:
	D3DXVECTOR3	m_vDragStart;

};

