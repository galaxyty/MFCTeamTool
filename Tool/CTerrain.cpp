#include "pch.h"
#include "CTerrain.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "CMapManager.h"
#include "DH_Define.h"

float g_Ratio = 1.f;

CTerrain::CTerrain()
{
	CMapManager::Get_Instance()->m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize()
{
	m_RoomIndex = CMapManager::Get_Instance()->m_RoomIndex;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		TEX_MULTI, L"Terrain", L"Tile", 2)))
	{
		AfxMessageBox(L"Terrain Texture Insert Failed");
		return E_FAIL;
	}

	TCHAR* mapKey = L"Pub";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Map/Pub.bmp",
		TEX_SINGLE, mapKey, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return E_FAIL;
	}

	CMapManager::Get_Instance()->CreateRoom();

	return S_OK;
}

void CTerrain::Update()
{
	
}

void CTerrain::Render()
{
	m_RoomIndex = CMapManager::Get_Instance()->m_RoomIndex;

	RECT	rc{};

	GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	// ȭ�� ��ǥ ��������
	CPoint screenPoint;
	GetCursorPos(&screenPoint);

	// ��ũ�� ��ǥ�� m_pMainView Ŭ���̾�Ʈ ��ǥ�� ��ȯ.
	ScreenToClient(m_pMainView->m_hWnd, &screenPoint);

	// ������ �°� ����Ʈ ��ǥ ����.
	screenPoint.x *= long(1.f / g_Ratio);
	screenPoint.y *= long(1.f / g_Ratio);

	// ��׶���.
	D3DXMATRIX	matWorld, matScale, matTrans;

	const TEXINFO* pBackground = CTextureMgr::Get_Instance()->Get_Texture(CMapManager::Get_Instance()->m_vecBG[m_RoomIndex]->szBGKey, nullptr, 0);
	CDevice::Get_Instance()->GetBackground()->Draw(pBackground->pTexture,
		nullptr,
		nullptr,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		CMapManager::Get_Instance()->m_vecBG[m_RoomIndex]->vPos.x - m_pMainView->GetScrollPos(0),
		CMapManager::Get_Instance()->m_vecBG[m_RoomIndex]->vPos.y - m_pMainView->GetScrollPos(1),
		CMapManager::Get_Instance()->m_vecBG[m_RoomIndex]->vPos.z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

	CDevice::Get_Instance()->GetBackground()->SetTransform(&matWorld);

	// ������Ʈ ��ġ �� �̸�����.
	const TCHAR* objectKey = CMapManager::Get_Instance()->GetObjectKey();

	bool s = CMapManager::Get_Instance()->m_IsObjectSetting;

	if (CMapManager::Get_Instance()->m_IsObjectSetting == true)
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
			(float)screenPoint.x,
			(float)screenPoint.y,
			0);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX * g_Ratio, fY * g_Ratio);

		CDevice::Get_Instance()->GetpObjectView()->SetTransform(&matWorld);
	}

	// ������Ʈ.
	for (auto pObjectData : CMapManager::Get_Instance()->m_vecObject[m_RoomIndex])
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

		CDevice::Get_Instance()->GetpObject()->Draw(pObj->pTexture, //����� �ؽ�ó �İ�ü
			nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
			&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
			nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����
	}

	// Ÿ��.
	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);	

	// ������Ʈ ��ġ ���̸� Ÿ�� ǥ�� ����.
	if (CMapManager::Get_Instance()->m_IsObjectSetting == true)
	{
		return;
	}
	if (g_GamePlay == true)
	{
		return;
	}

	for (auto pTile : CMapManager::Get_Instance()->m_vecTile[m_RoomIndex])
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

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
			nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
			&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
			nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����
			
		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// ����� ���ڿ�
			lstrlen(szBuf),  // ���ڿ� ������ ũ��
			nullptr,		// ����� ��Ʈ ��ġ
			0,			// ���� ����(�ɼ�)
			D3DCOLOR_ARGB(255, 255, 255, 255));

		iIndex++;
	}
}

void CTerrain::Release()
{
	
}

void CTerrain::Mini_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto pTile : CMapManager::Get_Instance()->m_vecTile[m_RoomIndex])
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

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
			nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
			&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
			nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����
	}
}

void CTerrain::Tile_Change(const D3DXVECTOR3& vPos)
{
	int iIndex = Get_TileIdx(vPos);

	if (-1 == iIndex)
		return;

	if (++CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->byDrawID >= 2)
	{
		// 0 �ʱ�ȭ.
		CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->byDrawID = 0;
	}

	CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->byOption = 0;
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

int CTerrain::Get_TileIdx(const D3DXVECTOR3& vPos)
{
	if (CMapManager::Get_Instance()->m_IsObjectSetting == true)
	{
		return -1;
	}

	for (size_t index = 0; index < CMapManager::Get_Instance()->m_vecTile[m_RoomIndex].size(); ++index)
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

	D3DXVec3TransformCoord(&vectorPos, &CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos, &matWorld);

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
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x, CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x + (TILECX / 2.f), CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y, 0.f },
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x, CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x - (TILECX / 2.f), CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y, 0.f },

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

	// 0 == ax + b - y // �� �� ����
	// 0 > ax + b - y // ���� ����
	// 0 < ax + b - y // ���� �Ʒ���

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


	return bCheck[m_RoomIndex] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 12 -> 3 -> 6 -> 9
	D3DXVECTOR3	vPoint[4] =
	{
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x, CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x + (TILECX / 2.f), CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y, 0.f },
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x, CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.x - (TILECX / 2.f), CMapManager::Get_Instance()->m_vecTile[m_RoomIndex][iIndex]->vPos.y, 0.f },

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
