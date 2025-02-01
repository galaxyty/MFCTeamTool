#include "pch.h"
#include "CTerrain.h"
#include "CTextureMgr.h"
#include "CDevice.h"

float g_Ratio = 1.f;

CTerrain::CTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		TEX_MULTI, L"Terrain", L"Tile", 2)))
	{
		AfxMessageBox(L"Terrain Texture Insert Failed");
		return E_FAIL;
	}

	TCHAR* c = L"Pub";
	CTextureMgr::Get_Instance()->SetBGKey(c);

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Map/Pub.bmp",
		TEX_SINGLE, c, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return E_FAIL;
	}

	m_imgBackground = new TILE;
	m_imgBackground->vPos = {0.f, 0.f, 0.f};
	m_imgBackground->vSize = { 1344.f, 600.f};

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float	fY = TILECY * i;
			float	fX = TILECX * j;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY};
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CTerrain::Update()
{
	
}

void CTerrain::Render()
{
	RECT	rc{};

	GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	// 화면 좌표 가져오기
	CPoint screenPoint;
	GetCursorPos(&screenPoint);

	// 스크린 좌표를 클라이언트 좌표로 변환.
	ScreenToClient(g_hWnd, &screenPoint);

	// 비율에 맞게 포인트 좌표 갱신.
	screenPoint.x *= 1.f / g_Ratio;
	screenPoint.y *= 1.f / g_Ratio;

	// 백그라운드.
	const TCHAR* bgKey = CTextureMgr::Get_Instance()->GetBGKey();
	
	D3DXMATRIX	matWorld, matScale, matTrans;

	const TEXINFO* pBackground = CTextureMgr::Get_Instance()->Get_Texture(bgKey, nullptr, 0);
	CDevice::Get_Instance()->GetBackground()->Draw(pBackground->pTexture,
		nullptr,
		nullptr,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_imgBackground->vPos.x - m_pMainView->GetScrollPos(0),
		m_imgBackground->vPos.y - m_pMainView->GetScrollPos(1),
		m_imgBackground->vPos.z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

	CDevice::Get_Instance()->GetBackground()->SetTransform(&matWorld);

	// 오브젝트 설치 시 미리보기.
	const TCHAR* objectKey = CTextureMgr::Get_Instance()->GetObjectKey();

	if (objectKey != nullptr)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);

		const TEXINFO* pObject = CTextureMgr::Get_Instance()->Get_Texture(objectKey, nullptr, 0);

		float	fCenterX = pObject->tImgInfo.Width / 2.f;
		float	fCenterY = pObject->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->GetpObjectView()->Draw(pObject->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(100, 255, 255, 255));

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			screenPoint.x,
			screenPoint.y,
			0);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		CDevice::Get_Instance()->GetpObjectView()->SetTransform(&matWorld);
	}

	// 오브젝트.
	for (auto pObjectData : m_vecObject)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pObjectData->vPos.x - m_pMainView->GetScrollPos(0),
			pObjectData->vPos.y - m_pMainView->GetScrollPos(1),
			pObjectData->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		CDevice::Get_Instance()->GetpObject()->SetTransform(&matWorld);

		const TEXINFO* pObj = CTextureMgr::Get_Instance()->Get_Texture(*pObjectData->szName, nullptr, 0);

		float	fCenterX = pObj->tImgInfo.Width / 2.f;
		float	fCenterY = pObj->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->GetpObject()->Draw(pObj->pTexture, //출력할 텍스처 컴객체
			nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
			&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
			nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지
	}

	// 타일.
	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);	

	// 오브젝트 배치 중이면 타일 표시 안함.
	if (CTextureMgr::Get_Instance()->GetObjectKey() != nullptr)
	{
		return;
	}

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, 
			pTile->vPos.x - m_pMainView->GetScrollPos(0),
			pTile->vPos.y - m_pMainView->GetScrollPos(1),
			pTile->vPos.z);

		matWorld = matScale * matTrans;		

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);		

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
			nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
			&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
			nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지
			
		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// 출력할 문자열
			lstrlen(szBuf),  // 문자열 버퍼의 크기
			nullptr,		// 출력할 렉트 위치
			0,			// 정렬 기준(옵션)
			D3DCOLOR_ARGB(255, 255, 255, 255));

		iIndex++;
	}
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), [](auto& p) 
		{ 
			if (p)
			{ delete p; p = nullptr; }
		});
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	for_each(m_vecObject.begin(), m_vecObject.end(), [](auto& p)
		{
			if (p)
			{
				delete p;
				p = nullptr;
			}
		});
	m_vecObject.clear();
	m_vecObject.shrink_to_fit();

	Safe_Delete(m_imgBackground);
}

void CTerrain::Mini_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x,
			pTile->vPos.y,
			pTile->vPos.z);

		matWorld = matScale * matTrans;
				
		Set_Ratio(&matWorld, g_Ratio, g_Ratio);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
			nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
			&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
			nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지
	}
}

void CTerrain::Tile_Change(const D3DXVECTOR3& vPos)
{
	int iIndex = Get_TileIdx(vPos);

	if (-1 == iIndex)
		return;

	if (++m_vecTile[iIndex]->byDrawID >= 2)
	{
		// 0 초기화.
		m_vecTile[iIndex]->byDrawID = 0;
	}

	m_vecTile[iIndex]->byOption = 0;
}

void CTerrain::Set_Ratio(D3DXMATRIX* pOut, float _fX, float _fY)
{
	pOut->_11 *= _fX;
	pOut->_21 *= _fX;
	pOut->_31 *= _fX;
	pOut->_41 *= _fX;

	pOut->_12 *= _fY;
	pOut->_22 *= _fY;
	pOut->_32 *= _fY;
	pOut->_42 *= _fY;

}

void CTerrain::AddObject(CustomOBJECT* _object)
{
	m_vecObject.push_back(_object);
}


int CTerrain::Get_TileIdx(const D3DXVECTOR3& vPos)
{
	if (CTextureMgr::Get_Instance()->GetObjectKey() != nullptr)
	{
		return -1;
	}

	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking_Rect(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

bool CTerrain::Picking_Rect(const D3DXVECTOR3& vPos, const int& iIndex)
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXVECTOR3 vectorPos;	

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matScale);

	matWorld = matScale;
	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	D3DXVec3TransformCoord(&vectorPos, &m_vecTile[iIndex]->vPos, &matWorld);

	return (vPos.x >= vectorPos.x - (TILECX * (0.5f * g_Ratio)) &&
		vPos.x < vectorPos.x + (TILECX * (0.5f * g_Ratio))) &&
		(vPos.y >= vectorPos.y - (TILECY * (0.5f * g_Ratio)) &&
			vPos.y < vectorPos.y + (TILECY * (0.5f * g_Ratio)));
}

bool CTerrain::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 12 -> 3 -> 6 -> 9
	float	fGradient[4] =
	{
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)
	};

	// 12 -> 3 -> 6 -> 9
	D3DXVECTOR3	vPoint[4] =
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },

	};

	// y = ax + b;
	// -b = ax - y
	// b = y - ax

	float	fB[4] =
	{
		vPoint[0].y - fGradient[0] * vPoint[0].x,
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x
	};

	// 0 == ax + b - y // 선 상에 있음
	// 0 > ax + b - y // 선분 위쪽
	// 0 < ax + b - y // 선분 아래쪽

	bool	bCheck[4]{ false };

	// 12 -> 3
	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	// 3 -> 6
	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	// 6 -> 9
	if (0 >= fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	// 9 -> 12
	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;


	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 12 -> 3 -> 6 -> 9
	D3DXVECTOR3	vPoint[4] =
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },

	};

	D3DXVECTOR3	vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4] =
	{
		{ -vDir[0].y , vDir[0].x, 0.f },
		{ -vDir[1].y , vDir[1].x, 0.f },
		{ -vDir[2].y , vDir[2].x, 0.f },
		{ -vDir[3].y , vDir[3].x, 0.f },
	};

	D3DXVECTOR3		vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}
