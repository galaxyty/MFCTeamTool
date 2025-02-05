#include "pch.h"
#include "CMapManager.h"

IMPLEMENT_SINGLETON(CMapManager)

CMapManager::CMapManager() : m_IsObjectSetting(false), m_RoomIndex(-1), m_objectKey(nullptr)
{
}

CMapManager::~CMapManager()
{
	Release();
}

void CMapManager::Release()
{
	// 타일 제거.
	for_each(m_vecTile.begin(), m_vecTile.end(), [](auto& pTILE)
		{
			for_each(pTILE.begin(), pTILE.end(), [](auto& p)
				{
					if (p)
					{
						delete p;
						p = nullptr;
					}
				});

			pTILE.clear();
			pTILE.shrink_to_fit();
		});
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	// 백그라운드 제거.
	for_each(m_vecBG.begin(), m_vecBG.end(), [](auto& p)
		{
			if (p)
			{
				delete p;
				p = nullptr;
			}
		});

	m_vecBG.clear();
	m_vecBG.shrink_to_fit();

	// 오브젝트 제거.
	for_each(m_vecObject.begin(), m_vecObject.end(), [](auto& pOBJECT)
		{
			for_each(pOBJECT.begin(), pOBJECT.end(), [](auto& p)
				{
					if (p)
					{
						delete p;
						p = nullptr;
					}
				});

			pOBJECT.clear();
			pOBJECT.shrink_to_fit();
		});
	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
}

void CMapManager::CreateRoom()
{
	m_RoomIndex = m_vecBG.size();
	//m_RoomIndex++;

	// 타일 생성.
	vector<TILE*> tileVec;

	m_vecTile.push_back(tileVec);

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float	fY = TILECY * i;
			float	fX = TILECX * j;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY };
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile[m_RoomIndex].push_back(pTile);
		}
	}

	// 백그라운드 생성.
	// 기본 맵 키는 주점.
	TCHAR* mapKey = new TCHAR[MAX_PATH];

	_tcscpy_s(mapKey, MAX_PATH, L"Pub");

	CustomBACKGROUND* bg = new CustomBACKGROUND;
	bg->vPos = { 0.f, 0.f, 0.f };
	bg->vSize = { 1344.f, 600.f };
	bg->szBGKey = mapKey;

	m_vecBG.push_back(bg);

	// 오브젝트 생성.
	vector<CustomOBJECT*> objectVec;

	m_vecObject.push_back(objectVec);
}

void CMapManager::AddObject(CustomOBJECT* _object)
{
	m_vecObject[m_RoomIndex].push_back(_object);
}

void CMapManager::SetObjectKey(const TCHAR* _object)
{
	m_objectKey = _object;
}

const TCHAR* CMapManager::GetObjectKey() const
{
	return m_objectKey;
}

CString* CMapManager::GetObjectKeyValue() const
{
	return new CString(m_objectKey);
}
