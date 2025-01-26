#pragma once
#include "DH_CObject.h"
#include "Include.h"

class DH_UI :   public DH_CObject
{
public:
	DH_UI();
	virtual ~DH_UI();

public:

	virtual void Update();
	virtual void Render();
	virtual void Initialize();
	virtual void LateUpdate();

public:
	//���콺 ���� �Լ�
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

private:
	//�ڽ� UI ���� ��� ����
	vector<DH_UI*>	m_vecChildUI;

	//�θ� ��� ������
	DH_UI* m_pParnetUI;

	//���� UI ��ġ
	D3DXVECTOR3		m_vFinalPos;

public:
	//�θ� �ּ� ���Լ�
	DH_UI* GetParent() { return m_pParnetUI; }

	//�� ��ü�� �θ�� �����ִ� �Լ� 
	void	AddParent(DH_UI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParnetUI = this; }
	void	SetPinalPos(D3DXVECTOR3 _tFin) { m_vFinalPos = _tFin; }

	D3DXVECTOR3	GetPinalPos() { return m_vFinalPos; }
	bool		IsMouseOn() { return m_bMouseOn; }
	bool		IsbLDown() { return m_bLDown; }

	vector<DH_UI*>& GetChildUI() { return m_vecChildUI; }

public:
	void MouseOnCheck();
	void UpdateChild();
	void RenderChild();
	void LateUpdateChild();

public:
	void SetVisible(bool visible) { m_bVisible = visible; }
	bool IsVisible() const { return m_bVisible; }

protected:
	bool m_bMouseOn;
	bool m_bLDown;
	bool m_bVisible;


	friend class DH_UIMgr;
};

