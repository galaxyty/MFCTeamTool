#pragma once
#include "DH_UI.h"
class DH_Inventory;
class DH_SkillUI;

class DH_Interface :  public DH_UI
{
public:
	DH_Interface();
	virtual ~DH_Interface();

public:
	virtual void Update();
	virtual void Render(CToolView* pMainView);
	virtual void Initialize();

public:
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

	void		SetButten();

public:
	DH_Inventory* GetInventory() const { return m_Inventory; }
	void SetInventory(DH_Inventory* inventory) { m_Inventory = inventory; }
	DH_SkillUI* GetSkillUI() const { return m_SkillUI; }
	void SetSkillUI(DH_SkillUI* skillUI) {	m_SkillUI = skillUI; }

public:
	D3DXVECTOR3	m_vDragStart;
	
	DH_Inventory* m_Inventory;
	DH_SkillUI* m_SkillUI;
};

