#pragma once
#include "DH_UI.h"
class DH_Item :  public DH_UI
{
public:
	DH_Item();
	virtual ~DH_Item();

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
	void	SetItem(const ITEM& item) { m_sITEM = item; }
	void	SetpITEMDATA(ITEMDATA* _ITEMDATA) { m_pITEMDATA = _ITEMDATA; }
	ITEMDATA* GetpITEMDATA() { return m_pITEMDATA; }
	ITEM	GetItem() const {	return m_sITEM;	}
	
public:
	void MouseDrag(DH_UI* _UI);
	void RenderItemInfo(TCHAR* _Key);

private:
	D3DXVECTOR3	m_vDragStart;
	CString m_ImageKey;
	DH_UI* m_Inven;

	ITEMDATA* m_pITEMDATA;

	bool m_bDragOn;
	int m_PreviousIndex;
	ITEM m_sITEM;

};

