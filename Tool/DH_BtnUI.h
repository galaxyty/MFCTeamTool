#pragma once
#include "DH_UI.h"

class DH_BtnUI : public DH_UI
{
public:
	DH_BtnUI();
	virtual ~DH_BtnUI();

public:
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

public:
	virtual void LateUpdate();

public:
	void		SetbClicked(bool _Clicked) { m_bClicked = _Clicked; }
	bool		GetbClicked() { return m_bClicked; }

private:
	bool		m_bClicked;
};

