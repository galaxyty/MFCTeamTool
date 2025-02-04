#include "pch.h"
#include "CTextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::Get_Texture(const TCHAR* pObjKey, 
										const TCHAR* pStateKey, 
										const int& iCnt)
{
	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), 
		[&](auto& MyPair)->bool
		{
			if (MyPair.first == pObjKey)
				return true;

			return false;
		});

	if (iter == m_mapTex.end())
		return nullptr;


	return iter->second->Get_Texture(pStateKey, iCnt);
}
HRESULT CTextureMgr::Insert_Texture(const TCHAR* pFilePath, TEXTYPE eTexture, const TCHAR* pObjKey, const TCHAR* pStateKey, const int& iCnt)
{
	auto    iter = find_if(m_mapTex.begin(), m_mapTex.end(),
		[&](auto& MyPair)->bool
		{
			if (MyPair.first == pObjKey)
				return true;

			return false;
		});

	//없던 오브젝트면 새로 만든다.
	if (iter == m_mapTex.end())
	{
		CTexture* pTexture = nullptr;

		switch (eTexture)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCnt)))
		{
			ERR_MSG(pFilePath);
			return E_FAIL;
		}

		m_mapTex.insert({ pObjKey, pTexture });

	}
	//있던 오브젝트인데 멀티 텍스쳐다
	else if (CMultiTexture* pMultiTex = dynamic_cast<CMultiTexture*>(iter->second))
	{
		pMultiTex->Insert_Texture(pFilePath, pStateKey, iCnt);
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_mapTex.begin(), m_mapTex.end(), [](auto& MyPair)
		{
			Safe_Delete<CTexture*>(MyPair.second);
		});

	m_mapTex.clear();	
}

HRESULT CTextureMgr::Read_ImgPath(const wstring& wstrPath)
{
	wifstream		fin;

	fin.open(wstrPath, ios::in);

	if (!fin.fail())
	{
		TCHAR		szObjKey[MAX_STR] = L"";
		TCHAR		szStateKey[MAX_STR] = L"";
		TCHAR		szCount[MAX_STR] = L"";
		TCHAR		szPath[MAX_PATH] = L"";

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			int	iCount = _ttoi(szCount);

			if (FAILED(Insert_Texture(szPath, TEX_MULTI, szObjKey, szStateKey, iCount)))
				return E_FAIL;
		}

		fin.close();
	}

	return S_OK;
}
