#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

public:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9			Get_Device() { return m_pDevice; }
	LPD3DXSPRITE				Get_Sprite() { return m_pSprite; }
	LPD3DXSPRITE				GetBackground() { return m_pBackground; }
	LPD3DXSPRITE				GetpObjectView() { return m_pObjectView; }
	LPD3DXSPRITE				GetpObject() { return m_pObject; }
	LPD3DXFONT					Get_Font()	 { return m_pFont;  }
	LPD3DXFONT					Get_Font2()	 { return m_pFont2;  }
	LPD3DXFONT					Get_Font3()	 { return m_pFont3;  }

public:
	HRESULT		SetFont01();
	HRESULT		SetFont02();
	HRESULT		SetFont03();

public:
	HRESULT		Init_Device();

	void		Render_Begin();
	void		Render_End(HWND hWnd = nullptr);

	void		Release();

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// 1번에 해당하는 객체
	LPDIRECT3DDEVICE9	m_pDevice;	// 3번에 해당하는 객체

	LPD3DXSPRITE		m_pSprite;		// 2d 이미지를 출력하기 위한 용도
	LPD3DXSPRITE		m_pBackground;	// 백그라운드
	LPD3DXSPRITE		m_pObjectView;		// 오브젝트 미리보기.
	LPD3DXSPRITE		m_pObject;			// 오브젝트 배치.
	LPD3DXFONT			m_pFont;
	LPD3DXFONT			m_pFont2;
	LPD3DXFONT			m_pFont3;
};

