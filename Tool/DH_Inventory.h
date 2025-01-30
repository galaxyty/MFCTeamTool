#pragma once
#include "DH_UI.h"
class DH_Inventory :  public DH_UI
{
public:
	DH_Inventory();
	virtual ~DH_Inventory();

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
	void SetButten();

public:
	D3DXVECTOR3	m_vDragStart;
};

