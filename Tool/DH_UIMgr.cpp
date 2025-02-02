#include "pch.h"
#include "DH_UIMgr.h"
#include "DH_CObject.h"
#include "DH_UI.h"
#include "DH_OBJMgr.h"
#include "DH_KeyMgr.h"


DH_UIMgr* DH_UIMgr::m_pInstance = nullptr;


DH_UIMgr::DH_UIMgr() : m_FocusedUI(nullptr), m_UION(true)
{

}

DH_UIMgr::~DH_UIMgr()
{
}

void DH_UIMgr::Update()
{
	if (m_UION)
	{
		m_FocusedUI = GetFocusUI();

		if (!m_FocusedUI)
			return;

		bool bLTap = KEY_TAP(KEY::LBUTTEN);
		bool bRTap = KEY_TAP(KEY::RBUTTEN);
		bool bLAway = KEY_AWAY(KEY::LBUTTEN);
		bool bRAway = KEY_AWAY(KEY::RBUTTEN);

		DH_UI* pTarget = GetTargetedUI(m_FocusedUI);

		if (nullptr != pTarget)
		{
			pTarget->MouseOn();

			if (bLTap)
			{
				pTarget->MouseLDown();
				pTarget->m_bLDown = true;
			}
			else if (bLAway)
			{
				pTarget->MouseLUp();

				if (pTarget->m_bLDown)
				{
					pTarget->MouseLClicked();
				}

				pTarget->m_bLDown = false;
			}
			if (bRTap)
			{
				pTarget->m_bRDown = true;
			}
			else if (bRAway)
			{
				pTarget->m_bRDown = false;
			}
		}
	}
}

void DH_UIMgr::Initialize()
{

}

void DH_UIMgr::SetFocusedUI(DH_UI* _pUI)
{
	if (m_FocusedUI == _pUI || nullptr == _pUI)
	{
		m_FocusedUI = _pUI;
		return;
	}

	m_FocusedUI = _pUI;

	list<DH_CObject*>& vecUI = DH_OBJMgr::Get_Instance()->Get_UI();

	list<DH_CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_FocusedUI == *iter)
		{
			break;
		}
	}

	vecUI.erase(iter);
	vecUI.push_back(m_FocusedUI);
}

DH_UI* DH_UIMgr::GetTargetedUI(DH_UI* _pParentUI)
{
	bool bLAway = KEY_AWAY(KEY::LBUTTEN);
	bool bRAway = KEY_AWAY(KEY::RBUTTEN);
	DH_UI* pTargetUI = nullptr;

	//1. �θ� ���� , ��� �ڽĵ��� �˻��Ѵ�.
	static list<DH_UI*> queue;
	static vector<DH_UI*> vecNonTarget;
	queue.clear();
	vecNonTarget.clear();


	//ť�� �ֻ��� �θ� UI �ֱ�
	queue.push_back(_pParentUI);

	//ť�� ���� �� ���� ��ȸ
	while (!queue.empty())
	{
		//ť���� �������ϳ� ������
		DH_UI* pUI = queue.front();
		queue.pop_front();

		//ť�� �� �տ� ���콺�� �÷����� �ٷ� Ÿ��
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNonTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			vecNonTarget.push_back(pUI);
		}

		vector<DH_UI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

	}


	//���� ��ư ���� ������ üũ�� �ٽ� �����Ѵ�.
	if (bLAway)
	{
		for (size_t i = 0; i < vecNonTarget.size(); ++i)
		{
			vecNonTarget[i]->m_bLDown = false;
		}
	}

	if (bRAway)
	{
		for (size_t i = 0; i < vecNonTarget.size(); ++i)
		{
			vecNonTarget[i]->m_bRDown = false;
		}
	}

	return pTargetUI;
}

DH_UI* DH_UIMgr::GetFocusUI()
{
	list<DH_CObject*>& vecUI = DH_OBJMgr::Get_Instance()->Get_UI();

	bool bLTap = KEY_TAP(KEY::LBUTTEN);
	bool bRTap = KEY_TAP(KEY::RBUTTEN);

	DH_UI* pFocusedUI = m_FocusedUI;

	if (!bLTap && !bRTap)
	{
		return pFocusedUI;
	}

	//���ʹ�ư TAP�� �߻��ߴٴ� ����
	list<DH_CObject*>::iterator targetiter = vecUI.end();
	list<DH_CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((DH_UI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
	}

	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}

	pFocusedUI = (DH_UI*)*targetiter;

	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}
