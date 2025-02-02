#pragma once
#include "Include.h"

class CMapManager
{
	DECLARE_SINGLETON(CMapManager)

private:
	CMapManager();
	~CMapManager();

public:
	void Release();

	// �� ����.
	void CreateRoom();

	// ���� �� �ε����� ������Ʈ �߰�.
	void AddObject(CustomOBJECT* _object);

	// ���� �� �ε����� � ������Ʈ ��ġ �� ������ ����.
	void SetObjectKey(const TCHAR* _object);
	
	const TCHAR* GetObjectKey() const;

	CString* GetObjectKeyValue() const;

	friend class CTerrain;
	friend class CMapTool;
	friend class CToolView;

private:
	vector<vector<TILE*>>			m_vecTile;
	vector<CustomBACKGROUND*>		m_vecBG;
	vector<vector<CustomOBJECT*>>	m_vecObject;

	bool m_IsObjectSetting;

	int m_RoomIndex;

	const TCHAR* m_objectKey;
};