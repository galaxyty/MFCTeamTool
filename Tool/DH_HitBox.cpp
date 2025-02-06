#include "pch.h"
#include "DH_HitBox.h"
#include "DH_Define.h"
#include "CDevice.h"
#include "DH_Player.h"
#include "CTextureMgr.h"

DH_HitBox::DH_HitBox()
{
}

DH_HitBox::~DH_HitBox()
{
}

void DH_HitBox::Update()
{
}

void DH_HitBox::LateUpdate()
{
}

void DH_HitBox::Initialize()
{
}

void DH_HitBox::Render(CToolView* pMainView)
{
    D3DXMATRIX	matWorld, matScale, matTrans;

    const TEXINFO* pTexInfo;
    //이미지 키 가져오기 밑 중심설정

    pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Cutsin", 10);

    float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
    float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
    D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

    //월드 행렬 곱해주기
    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

    D3DXMatrixTranslation(&matTrans,
        DH_Player::Get_Instance()->GetMPos().x,
        DH_Player::Get_Instance()->GetMPos().y,
        DH_Player::Get_Instance()->GetMPos().z);

    matWorld = matScale * matTrans;

    Set_Ratio(&matWorld, g_Ratio, g_Ratio);

    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


    RECT REC = { -100, -100, 100, 100 };
    D3DCOLOR rectColor = D3DCOLOR_ARGB(255, 255, 0, 0); // 빨간색
    DrawRectangle(CDevice::Get_Instance()->Get_Device(), REC, rectColor);


    

    //CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
    //    nullptr,
    //    &vTemp,
    //    nullptr,
    //    D3DCOLOR_ARGB(255, 255, 255, 255));

}

void DH_HitBox::Release()
{
}


void DH_HitBox::DrawRectangle(LPDIRECT3DDEVICE9 pDevice, RECT rect, D3DCOLOR color)
{
    // 사각형을 정의하는 4개의 정점
    CUSTOMVERTEX vertices[] =
    {
        { (float)rect.left,  (float)rect.top,    0.0f, 1.0f, color }, // 왼쪽 위
        { (float)rect.right, (float)rect.top,    0.0f, 1.0f, color }, // 오른쪽 위
        { (float)rect.right, (float)rect.bottom, 0.0f, 1.0f, color }, // 오른쪽 아래
        { (float)rect.left,  (float)rect.bottom, 0.0f, 1.0f, color }  // 왼쪽 아래
    };

    // FVF 설정
    pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

    // 사각형 그리기
    pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(CUSTOMVERTEX));
}