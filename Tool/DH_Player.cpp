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
    // �⺻ ü�� �� ����
    m_RESETPLAYERDATA.iHP = 100;             // �⺻ ü��
    m_RESETPLAYERDATA.iMP = 50;              // �⺻ ����

    // �⺻ ����
    m_RESETPLAYERDATA.iIntelligence = 10;    // ����
    m_RESETPLAYERDATA.iStrength = 15;        // ü��(�ٷ�)
    m_RESETPLAYERDATA.iPhysicalAtk = 12;     // ���� ����
    m_RESETPLAYERDATA.iMagicAtk = 8;         // ���� ����

    // �⺻ ����
    m_RESETPLAYERDATA.iFireResist = 5;       // ȭ�� ����
    m_RESETPLAYERDATA.iLightResist = 5;      // ������
    m_RESETPLAYERDATA.iManaResist = 5;       // �׸���

    // �̵� �ӵ��� ���� �ӵ�
    m_RESETPLAYERDATA.iAgility = 10;         // �̵� �ӵ�
    m_RESETPLAYERDATA.iAttackSpeed = 10;     // ���� �ӵ�

    // ����
    m_RESETPLAYERDATA.iPhysicalDef = 10;     // ���� ���
    m_RESETPLAYERDATA.iMagicDef = 8;         // ���� ���

    // ��Ÿ ����
    m_RESETPLAYERDATA.iWaterResist = 5;      // ������
    m_RESETPLAYERDATA.iDarkResist = 5;       // ������

    m_PLAYERDATA = m_RESETPLAYERDATA;
}

DH_Player::~DH_Player()
{

}

void DH_Player::Update()
{
    //����ġ ���� ��
    if (m_ItemUpdate)
    {
        m_PLAYERDATA = m_RESETPLAYERDATA;
        //�κ��丮�� ���� ĭ�� ��ȸ�ϸ� �������� �����´�.
        for (auto& UI : m_Inventory->GetChildUI())
        {
            auto& BtnUI = *dynamic_cast<DH_BtnUI*>(UI);
            if (BtnUI.GetChildUI().size() > 0 && BtnUI.GetbEquip())
            {
                auto& Item = *dynamic_cast<DH_Item*>(BtnUI.GetChildUI().front());

                // HP�� MP�� ���ϱ�
                m_PLAYERDATA.iHP += Item.GetpITEMDATA()->iHP;
                m_PLAYERDATA.iMP += Item.GetpITEMDATA()->iMP;

                // ��(Strength)�� ����(Intelligence)�� ���ϱ�
                m_PLAYERDATA.iStrength += Item.GetpITEMDATA()->iStrength;
                m_PLAYERDATA.iIntelligence += Item.GetpITEMDATA()->iIntelligence;

                // ���ݷ�(PhysicalAtk, MagicAtk)�� ���ϱ�
                m_PLAYERDATA.iPhysicalAtk += Item.GetpITEMDATA()->iPhysicalAtk;
                m_PLAYERDATA.iMagicAtk += Item.GetpITEMDATA()->iMagicAtk;

                // ����(PhysicalDef, MagicDef)�� ���ϱ�
                m_PLAYERDATA.iPhysicalDef += Item.GetpITEMDATA()->iPhysicalDef;
                m_PLAYERDATA.iMagicDef += Item.GetpITEMDATA()->iMagicDef;

                // �Ӽ� ����(FireResist, LightResist, ManaResist ��)�� ���ϱ�
                m_PLAYERDATA.iFireResist += Item.GetpITEMDATA()->iFireResist;
                m_PLAYERDATA.iLightResist += Item.GetpITEMDATA()->iLightResist;
                m_PLAYERDATA.iManaResist += Item.GetpITEMDATA()->iManaResist;
                m_PLAYERDATA.iWaterResist += Item.GetpITEMDATA()->iWaterResist;
                m_PLAYERDATA.iDarkResist += Item.GetpITEMDATA()->iDarkResist;

                // �̼�(Agility)�� ���� �ӵ�(AttackSpeed)�� ���ϱ�
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

