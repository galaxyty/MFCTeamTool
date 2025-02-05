#include "pch.h"
#include "DH_Player.h"
#include "Include.h"
#include "DH_Interface.h"
#include "DH_Inventory.h"
#include "DH_MyState.h"
#include "DH_BtnUI.h"
#include "CTextureMgr.h"
#include "DH_Item.h"
#include "CDevice.h"
#include "DH_TimeMgr.h"
#include "DH_KeyMgr.h"
#include "DH_Define.h"

IMPLEMENT_SINGLETON(DH_Player);

DH_Player::DH_Player() : m_ItemUpdate(false), m_Interface(nullptr), m_Inventory(nullptr), m_MyState(nullptr),
m_iFrame(0), m_fFrameCal(0.f), m_fReverse(1.f)
{
    m_State = L"IDLE";
    SetfSpeed(250.f);
}

DH_Player::~DH_Player()
{

}

void DH_Player::Initialize()
{
#pragma region 기본 스텟

    // 기본 체력 및 마나
    m_RESETPLAYERDATA.iHP = 100;             // 기본 체력
    m_RESETPLAYERDATA.iMP = 50;              // 기본 마나

    // 기본 스탯
    m_RESETPLAYERDATA.iIntelligence = 10;    // 지능
    m_RESETPLAYERDATA.iStrength = 15;        // 체력(근력)
    m_RESETPLAYERDATA.iPhysicalAtk = 12;     // 물리 공격
    m_RESETPLAYERDATA.iMagicAtk = 8;         // 마법 공격

    // 기본 저항
    m_RESETPLAYERDATA.iFireResist = 5;       // 화염 저항
    m_RESETPLAYERDATA.iLightResist = 5;      // 명저항
    m_RESETPLAYERDATA.iManaResist = 5;       // 항마력

    // 이동 속도와 공격 속도
    m_RESETPLAYERDATA.iAgility = 10;         // 이동 속도
    m_RESETPLAYERDATA.iAttackSpeed = 10;     // 공격 속도

    // 방어력
    m_RESETPLAYERDATA.iPhysicalDef = 10;     // 물리 방어
    m_RESETPLAYERDATA.iMagicDef = 8;         // 마법 방어

    // 기타 저항
    m_RESETPLAYERDATA.iWaterResist = 5;      // 수저항
    m_RESETPLAYERDATA.iDarkResist = 5;       // 암저항

    m_PLAYERDATA = m_RESETPLAYERDATA;

#pragma endregion

    if (FAILED(CTextureMgr::Get_Instance()->Read_ImgPath(L"../Data/ImgPath.txt")))
    {
        ERR_MSG(L"못 불러온 이미지가 있습니다.");
    }

    SetMPos(D3DXVECTOR3{ 400.f, 300.f, 0.f });
    SetScale(D3DXVECTOR3{ 800.f , 260.f, 0.f });
}

void DH_Player::Update()
{
    if (!g_GamePlay)
    {
        //플레이어 스탯 업데이트
        if (m_ItemUpdate)
        {
            m_PLAYERDATA = m_RESETPLAYERDATA;
            //인벤토리의 장착 칸을 순회하며 아이템을 가져온다.
            for (auto& UI : m_Inventory->GetChildUI())
            {
                auto& BtnUI = *dynamic_cast<DH_BtnUI*>(UI);
                if (BtnUI.GetChildUI().size() > 0 && BtnUI.GetbEquip())
                {
                    auto& Item = *dynamic_cast<DH_Item*>(BtnUI.GetChildUI().front());

                    // HP와 MP를 더하기
                    m_PLAYERDATA.iHP += Item.GetpITEMDATA()->iHP;
                    m_PLAYERDATA.iMP += Item.GetpITEMDATA()->iMP;

                    // 힘(Strength)와 지능(Intelligence)를 더하기
                    m_PLAYERDATA.iStrength += Item.GetpITEMDATA()->iStrength;
                    m_PLAYERDATA.iIntelligence += Item.GetpITEMDATA()->iIntelligence;

                    // 공격력(PhysicalAtk, MagicAtk)을 더하기
                    m_PLAYERDATA.iPhysicalAtk += Item.GetpITEMDATA()->iPhysicalAtk;
                    m_PLAYERDATA.iMagicAtk += Item.GetpITEMDATA()->iMagicAtk;

                    // 방어력(PhysicalDef, MagicDef)을 더하기
                    m_PLAYERDATA.iPhysicalDef += Item.GetpITEMDATA()->iPhysicalDef;
                    m_PLAYERDATA.iMagicDef += Item.GetpITEMDATA()->iMagicDef;

                    // 속성 저항(FireResist, LightResist, ManaResist 등)을 더하기
                    m_PLAYERDATA.iFireResist += Item.GetpITEMDATA()->iFireResist;
                    m_PLAYERDATA.iLightResist += Item.GetpITEMDATA()->iLightResist;
                    m_PLAYERDATA.iManaResist += Item.GetpITEMDATA()->iManaResist;
                    m_PLAYERDATA.iWaterResist += Item.GetpITEMDATA()->iWaterResist;
                    m_PLAYERDATA.iDarkResist += Item.GetpITEMDATA()->iDarkResist;

                    // 이속(Agility)와 공격 속도(AttackSpeed)를 더하기
                    m_PLAYERDATA.iAgility += Item.GetpITEMDATA()->iAgility;
                    m_PLAYERDATA.iAttackSpeed += Item.GetpITEMDATA()->iAttackSpeed;
                }
            }

            m_ItemUpdate = false;
        }

        // 델타타임 가져오기
        float deltaTime = DH_TimeMgr::Get_Instance()->Get_TimeDelta();

        // 델타타임 기반으로 누적
        m_fFrameCal += deltaTime * 15.f;

        // 누적 값이 1 이상일 때 정수 프레임 증가
        while (m_fFrameCal >= 1.0f)
        {
            m_iFrame += 1;
            m_fFrameCal -= 1.0f; // 누적 값에서 1 제거
        }
        if (m_iFrame >= 16)
            m_iFrame = 0;
    }
    else
    {
        // 프레임 진행
        float deltaTime = DH_TimeMgr::Get_Instance()->Get_TimeDelta();
        m_fFrameCal += deltaTime * 7.f;
        while (m_fFrameCal >= 1.0f)
        {
            m_iFrame += 1;
            m_fFrameCal -= 1.0f; // 누적 값에서 1 제거
        }
        KeyInput();
        SetEndFrame(m_State);
    }
}

void DH_Player::LateUpdate()
{
}

void DH_Player::Render(CToolView* pMainView)
{
    D3DXMATRIX	matWorld, matScale, matTrans;

    const TEXINFO* pTexInfo;
    //이미지 키 가져오기 밑 중심설정
    if(!g_GamePlay)
        pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Cutsin", m_iFrame);
    else
    {
        pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", m_State.c_str(), m_iFrame);
    }

    float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
    float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
    D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


    //월드 행렬 곱해주기
    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixScaling(&matScale, m_fReverse, 1.f, 1.f);

    //스크롤 값 제외
    D3DXMatrixTranslation(&matTrans,
        GetMPos().x,
        GetMPos().y,
        GetMPos().z);

    matWorld = matScale * matTrans;

    Set_Ratio(&matWorld, g_Ratio, g_Ratio);

    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

    if (g_GamePlay)
    {
        CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
            nullptr,
            &vTemp,
            nullptr,
            D3DCOLOR_ARGB(255, 255, 255, 255));
    }
    
    if (m_Inventory && m_Inventory->IsVisible() && !g_GamePlay)
    {
        CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
            nullptr,
            &vTemp,
            nullptr,
            D3DCOLOR_ARGB(255, 255, 255, 255));
    }
}

void DH_Player::Release()
{
}

void DH_Player::SetEndFrame(wstring _state)
{
    if (_state == L"ATTACK")
    {
        if (m_iFrame >= 15)
            m_iFrame = 0;
    }
    else if (_state == L"ATTACK2")
    {
        if (m_iFrame >= 15)
            m_iFrame = 0;
    }
    else if (_state == L"BUF")
    {
        if (m_iFrame >= 3)
            m_iFrame = 0;
    }
    else if (_state == L"CROSS")
    {
        if (m_iFrame >= 14)
            m_iFrame = 0;
    }
    else if (_state == L"HIT")
    {
        if (m_iFrame >= 4)
            m_iFrame = 0;
    }
    else if (_state == L"IDLE")
    {
        if (m_iFrame >= 22)
            m_iFrame = 0;
    }
    else if (_state == L"CROSS")
    {
        if (m_iFrame >= 14)
            m_iFrame = 0;
    }
    else if (_state == L"JACK")
    {
        if (m_iFrame >= 3)
            m_iFrame = 0;
    }
    else if (_state == L"JUMP")
    {
        if (m_iFrame >= 6)
            m_iFrame = 0;
    }
    else if (_state == L"JUMPATTACK")
    {
        if (m_iFrame >= 5)
            m_iFrame = 0;
    }
    else if (_state == L"MAHA")
    {
        if (m_iFrame >= 4)
            m_iFrame = 0;
    }
    else if (_state == L"MOVEFIRE")
    {
        if (m_iFrame >= 26)
            m_iFrame = 0;
    }
    else if (_state == L"PUNISH")
    {
        if (m_iFrame >= 13)
            m_iFrame = 0;
    }
    else if (_state == L"QUICKS")
    {
        if (m_iFrame >= 0)
            m_iFrame = 0;
    }
    else if (_state == L"RUN")
    {
        if (m_iFrame >= 7)
            m_iFrame = 0;
    }
    else if (_state == L"SLIDE")
    {
        if (m_iFrame >= 3)
            m_iFrame = 0;
    }
    else if (_state == L"THROW")
    {
        if (m_iFrame >= 4)
            m_iFrame = 0;
    }
    else if (_state == L"WALK")
    {
        if (m_iFrame >= 7)
            m_iFrame = 0;
    }
    else if (_state == L"WINDMIL")
    {
        if (m_iFrame >= 5)
            m_iFrame = 0;
    }
    else
    {
        return;
    }
}

void DH_Player::KeyInput()
{
    m_State = L"IDLE";
    if (KEY_HOLD(KEY::LEFT))
    {
        m_fReverse = -1.f;
        m_State = L"WALK";
        AddMPos(D3DXVECTOR3{ -GetfSpeed() * DH_TimeMgr::Get_Instance()->Get_TimeDelta(), 0.f, 0.f });
    }
    if (KEY_HOLD(KEY::RIGHT))
    {
        m_fReverse = 1.f;
        m_State = L"WALK";
        AddMPos(D3DXVECTOR3{ GetfSpeed() * DH_TimeMgr::Get_Instance()->Get_TimeDelta(), 0.f, 0.f });
    }
    if (KEY_HOLD(KEY::UP))
    {
        m_State = L"WALK";
        AddMPos(D3DXVECTOR3{ 0.f, -GetfSpeed() * DH_TimeMgr::Get_Instance()->Get_TimeDelta(), 0.f });
    }
    if (KEY_HOLD(KEY::DOWN))
    {
        m_State = L"WALK";
        AddMPos(D3DXVECTOR3{ 0.f, GetfSpeed() * DH_TimeMgr::Get_Instance()->Get_TimeDelta(), 0.f });
    }

}

