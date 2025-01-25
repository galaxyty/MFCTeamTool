#include "pch.h"
#include "DH_OBJMgr.h"
#include "DH_CObject.h"

DH_OBJMgr* DH_OBJMgr::m_pInstance = nullptr;

DH_OBJMgr::DH_OBJMgr()
{
}

DH_OBJMgr::~DH_OBJMgr()
{
	Release();
}


void DH_OBJMgr::Add_Object(OBJID eID, DH_CObject* pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

int DH_OBJMgr::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void DH_OBJMgr::Late_Update()
{
}

void DH_OBJMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < OBJID::OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(hDC);
	}
}


void DH_OBJMgr::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<DH_CObject*>);
		m_ObjList[i].clear();
	}
}

void DH_OBJMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}
