#pragma once
#include "DH_Define.h"

class DH_CObject;
class DH_OBJMgr
{
private:
	DH_OBJMgr();
	DH_OBJMgr(const DH_OBJMgr& rhs) = delete;
	DH_OBJMgr operator =(DH_OBJMgr& rObj) = delete;
	~DH_OBJMgr();

public:
	DH_CObject*			Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	DH_CObject*			Get_LastMonster() { return m_ObjList[OBJ_MONSTER].back(); }
	list<DH_CObject*>	Get_UI() { return m_ObjList[OBJ_UI]; }

public:
	void		Add_Object(OBJID eID, DH_CObject* pObj);
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Delete_ID(OBJID eID);

public:
	static DH_OBJMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new DH_OBJMgr;

		return m_pInstance;
	}

	static void		DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	list<DH_CObject*>		m_ObjList[OBJ_END];

	static DH_OBJMgr* m_pInstance;


};

