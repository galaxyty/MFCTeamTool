#include "pch.h"
#include "DH_Player.h"
#include "DH_Interface.h"
#include "DH_Inventory.h"
#include "DH_MyState.h"
#include "DH_BtnUI.h"
#include "DH_Item.h"

IMPLEMENT_SINGLETON(DH_Player);

DH_Player::DH_Player() : m_ItemUpdate(false)
{
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
}

DH_Player::~DH_Player()
{

}

void DH_Player::Update()
{
    //스위치 켜질 시
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
}

void DH_Player::LateUpdate()
{
}

void DH_Player::Initialize()
{
}

void DH_Player::Render(CToolView* pMainView)
{
}

void DH_Player::Release()
{
}

