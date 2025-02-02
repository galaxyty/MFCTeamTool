#pragma once
#include "DH_UI.h"
class DH_MyState :  public DH_UI
{
public:
	DH_MyState();
	virtual ~DH_MyState();

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
	D3DXVECTOR3	m_vDragStart;
};

