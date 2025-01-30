#pragma once

#include "CSingleTexture.h"
#include "CMultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* Get_Texture(const TCHAR* pObjKey,
							   const TCHAR* pStateKey = L"",
							   const int& iCnt = 0);

public:
	HRESULT		Insert_Texture(const TCHAR* pFilePath,
								TEXTYPE eTexture,
								const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCnt = 0);

	void SetBGKey(const TCHAR* _key);
	const TCHAR* GetBGKey() const;

	void SetObjectKey(const TCHAR* _object);

	// call by reference.
	const TCHAR* GetObjectKey() const;

	// call by value.
	CString* GetObjectKeyValue() const;

	void		Release();


private:
	map<wstring, CTexture*>		m_mapTex;
	const TCHAR* m_bgKey;
	const TCHAR* m_objectKey;
};

