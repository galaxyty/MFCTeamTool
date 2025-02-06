#pragma once
#include "DH_CObject.h"

class DH_HitBox : public DH_CObject
{
public:
    DH_HitBox();
    ~DH_HitBox();

public:
    virtual void		Update();
    virtual void		LateUpdate();
    virtual void		Initialize();
    virtual void		Render(CToolView* pMainView);
    virtual void		Release();


    void DrawRectangle(LPDIRECT3DDEVICE9 pDevice, RECT _HitRect, D3DCOLOR color);

public:
    RECT m_HitRect;
    D3DCOLOR m_color;
};

