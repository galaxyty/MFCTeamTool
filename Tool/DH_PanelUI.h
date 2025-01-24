#pragma once
#include "DH_UI.h"
class DH_PanelUI : public DH_UI
{
public:
	DH_PanelUI();
	virtual ~DH_PanelUI();

public:
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

public:
	D3DXVECTOR3	m_vDragStart;

};

