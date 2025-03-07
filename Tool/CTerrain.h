#pragma once

#include "Include.h"
#include "ToolView.h"

extern float g_Ratio;

class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	HRESULT		Initialize();
	void		Update();
	void		Render();
	void		Release();
	void		Mini_Render();

public:
	void	Tile_Change(const D3DXVECTOR3& vPos);
	void	Set_MainView(CToolView* pMainView) { m_pMainView = pMainView;  }
	void	Set_Ratio(D3DXMATRIX* pOut, float _fX, float _fY);

private:
	bool	Picking_Rect(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	int		Get_TileIdx(const D3DXVECTOR3& vPos);

	friend class CToolView;

private:
	CToolView*					m_pMainView;
	int m_RoomIndex;
};

