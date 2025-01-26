#pragma once
#include "DH_UI.h"

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

public:
	D3DXVECTOR3	m_vDragStart;
	

};

