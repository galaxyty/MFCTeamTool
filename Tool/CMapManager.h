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

	// 룸 생성.
	void CreateRoom();

	// 현재 룸 인덱스에 오브젝트 추가.
	void AddObject(CustomOBJECT* _object);

	// 현재 룸 인덱스에 어떤 오브젝트 배치 할 것인지 셋팅.
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