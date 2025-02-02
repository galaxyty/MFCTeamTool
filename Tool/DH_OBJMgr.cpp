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
		for (auto iter = m_ObjList[i].begin();	iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Update();
		}
	}

	return 0;
}

void DH_OBJMgr::Late_Update()
{
	for (size_t i = 0; i < OBJID::OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->LateUpdate();
	}
}

void DH_OBJMgr::Render()
{
	for (size_t i = 0; i < OBJID::OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
			pObj->Render(m_pMainView);
	}
}


void DH_OBJMgr::Release()
{
	//플레이어 싱글톤이여서 다른곳에서 삭제했음
	for (size_t i = 1; i < OBJ_END; ++i)
	{
		for(auto& pObj : m_ObjList[i])
		{
			delete pObj;
			pObj = nullptr;
		}

		m_ObjList[i].clear();
	}
}

void DH_OBJMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}
