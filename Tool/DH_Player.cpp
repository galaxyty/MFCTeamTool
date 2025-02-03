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

IMPLEMENT_SINGLETON(DH_Player);

DH_Player::DH_Player() : m_ItemUpdate(false), m_Interface(nullptr), m_Inventory(nullptr), m_MyState(nullptr),
m_iFrame(0), m_fFrameCal(0.f)
{
    
}

DH_Player::~DH_Player()
{

}

void DH_Player::Initialize()
{
#pragma region �⺻ ����

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

#pragma endregion


    CTextureMgr::Get_Instance()->Insert_Texture(
        L"../Texture/Picked/Player/Cutsin/cutsin_%02d.png", TEX_MULTI, L"Player", L"Cutsin", 16);


    SetMPos(D3DXVECTOR3{ 400.f, 300.f, 0.f });
    SetScale(D3DXVECTOR3{ 800.f , 260.f, 0.f });
}

void DH_Player::Update()
{
    //�÷��̾� ���� ������Ʈ
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

    // ��ŸŸ�� ��������
    float deltaTime = DH_TimeMgr::Get_Instance()->Get_TimeDelta();

    // ��ŸŸ�� ������� ����
    m_fFrameCal += deltaTime * 20.f;

    // ���� ���� 1 �̻��� �� ���� ������ ����
    while (m_fFrameCal >= 1.0f)
    {
        m_iFrame += 1;
        m_fFrameCal -= 1.0f; // ���� ������ 1 ����
    }
    if (m_iFrame >= 16)
        m_iFrame = 0;
}

void DH_Player::LateUpdate()
{
}

void DH_Player::Render(CToolView* pMainView)
{
    D3DXMATRIX	matWorld, matScale, matTrans;


    //�̹��� Ű �������� �� �߽ɼ���
    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Cutsin", m_iFrame);

    float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
    float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
    D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


    //���� ��� �����ֱ�
    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

    //��ũ�� �� ����
    D3DXMatrixTranslation(&matTrans,
        GetMPos().x,
        GetMPos().y,
        GetMPos().z);

    matWorld = matScale * matTrans;

    Set_Ratio(&matWorld, g_Ratio, g_Ratio);

    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);




    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr,
        &vTemp,
        nullptr,
        D3DCOLOR_ARGB(255, 255, 255, 255));
}

void DH_Player::Release()
{
}

