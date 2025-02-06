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
#include "CPlayerTool.h"
#include "DH_HitBox.h"
#include "DH_OBJMgr.h"

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

    if (FAILED(CTextureMgr::Get_Instance()->Read_ImgPath(L"../Data/ImgPath.txt")))
    {
        ERR_MSG(L"�� �ҷ��� �̹����� �ֽ��ϴ�.");
    }

    SetPos(D3DXVECTOR3{ 400.f, 300.f, 0.f });
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

    if (m_State == L"Cutsin")
    {       
        // ��ŸŸ�� ��������
        float deltaTime = DH_TimeMgr::Get_Instance()->Get_TimeDelta();

        // ��ŸŸ�� ������� ����
        m_fFrameCal += deltaTime * 15.f;

        // ���� ���� 1 �̻��� �� ���� ������ ����
        while (m_fFrameCal >= 1.0f)
        {
            m_iFrame += 1;
            m_fFrameCal -= 1.0f; // ���� ������ 1 ����
        }
        if (m_iFrame >= 16)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else
    {
        // ������ ����
        float deltaTime = DH_TimeMgr::Get_Instance()->Get_TimeDelta();
        m_fFrameCal += deltaTime * 8.5f;
        while (m_fFrameCal >= 1.0f)
        {
            m_iFrame += 1;
            m_fFrameCal -= 1.0f; // ���� ������ 1 ����

            // �������� �������� ���� �Լ� ȣ��
            if (m_iFrame != m_previousFrame && m_State != L"IDLE" && m_State != L"WALK")
            {
                m_previousFrame = m_iFrame; // ���� ������ �� ����
                SpawnHitBox();          // ȣ���� �Լ�
            }
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
    //�̹��� Ű �������� �� �߽ɼ���

    pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", m_State.c_str(), m_iFrame);

    float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
    float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
    D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


    //���� ��� �����ֱ�
    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixScaling(&matScale, m_fReverse, 1.f, 1.f);

    

    if (m_State == L"Cutsin")
    {
        D3DXMatrixTranslation(&matTrans,
            GetPos().x,
            GetPos().y,
            GetPos().z);

        matWorld = matScale * matTrans;

        Set_Ratio(&matWorld, g_Ratio, g_Ratio);

        CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

        CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
            nullptr,
            &vTemp,
            nullptr,
            D3DCOLOR_ARGB(255, 255, 255, 255));
    }
    
    else
    {
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
}

void DH_Player::Release()
{
}

void DH_Player::SetEndFrame(wstring _state)
{
    if (_state == L"ATTACK")
    {
        if (m_iFrame >= 15)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"ATTACK2")
    {
        if (m_iFrame >= 15)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"BUF")
    {
        if (m_iFrame >= 3)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"CROSS")
    {
        if (m_iFrame >= 14)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"HIT")
    {
        if (m_iFrame >= 4)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"IDLE")
    {
        if (m_iFrame >= 22)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"JACK")
    {
        if (m_iFrame >= 3)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"JUMP")
    {
        if (m_iFrame >= 6)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"JUMPATTACK")
    {
        if (m_iFrame >= 5)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"MAHA")
    {
        if (m_iFrame >= 4)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"MOVEFIRE")
    {
        if (m_iFrame >= 26)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"PUNISH")
    {
        if (m_iFrame >= 13)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"QUICKS")
    {
        if (m_iFrame >= 0)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"RUN")
    {
        if (m_iFrame >= 7)
            m_iFrame = 0;
    }
    else if (_state == L"SLIDE")
    {
        if (m_iFrame >= 3)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"THROW")
    {
        if (m_iFrame >= 4)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else if (_state == L"WALK")
    {
        if (m_iFrame >= 7)
            m_iFrame = 0;
    }
    else if (_state == L"WINDMIL")
    {
        if (m_iFrame >= 5)
        {
            m_iFrame = 0;
            m_State = L"IDLE";
        }
    }
    else
    {
        return;
    }
}

void DH_Player::KeyInput()
{
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
    if (KEY_AWAY(KEY::LEFT) || KEY_AWAY(KEY::RIGHT) || KEY_AWAY(KEY::UP) || KEY_AWAY(KEY::DOWN))
    {
        m_State = L"IDLE";
    }
    if (KEY_TAP(KEY::A) && m_Interface)
    {
        for (auto SkillBtn : m_Interface->GetChildUI())
        {
            if (SkillBtn->GetName() == L"SkillBtn_0")
            {
                m_iFrame = 0;
                m_State = SetSkillName(SkillBtn->GetChildUI().front()->GetName());
            }
        }
    }
    if (KEY_TAP(KEY::S) && m_Interface)
    {
        for (auto SkillBtn : m_Interface->GetChildUI())
        {
            if (SkillBtn->GetName() == L"SkillBtn_1")
            {
                m_iFrame = 0;
                m_State = SetSkillName(SkillBtn->GetChildUI().front()->GetName());
            }
        }
        
    }
    if (KEY_TAP(KEY::D) && m_Interface)
    {
        for (auto SkillBtn : m_Interface->GetChildUI())
        {
            if (SkillBtn->GetName() == L"SkillBtn_2")
            {
                m_iFrame = 0;
                m_State = SetSkillName(SkillBtn->GetChildUI().front()->GetName());
            }
        }
        
    }
    if (KEY_TAP(KEY::F) && m_Interface)
    {
        for (auto SkillBtn : m_Interface->GetChildUI())
        {
            if (SkillBtn->GetName() == L"SkillBtn_3")
            {
                m_iFrame = 0;
                m_State = SetSkillName(SkillBtn->GetChildUI().front()->GetName());
            }
        }
        
    }
    if (KEY_TAP(KEY::G) && m_Interface)
    {
        for (auto SkillBtn : m_Interface->GetChildUI())
        {
            if (SkillBtn->GetName() == L"SkillBtn_4")
            {
                m_iFrame = 0;
                m_State = SetSkillName(SkillBtn->GetChildUI().front()->GetName());
            }
        }
        
    }
    if (KEY_TAP(KEY::X))
    {
        m_iFrame = 0;
        m_State = L"ATTACK";
    }
    if (KEY_TAP(KEY::C))
    {
        m_iFrame = 0;
        m_State = L"BUF";
    }
    if (KEY_TAP(KEY::V))
    {
        m_iFrame = 0;
        m_State = L"JACK";
    }
    if (KEY_TAP(KEY::Z))
    {
        m_iFrame = 0;
        m_State = L"PUNISH";
    }
}

wstring DH_Player::SetSkillName(wstring _Name)
{
    if (_Name == L"MahaKick")
        return L"MAHA";
    else if (_Name == L"Moving_Shoot")
        return L"MOVEFIRE";
    else if (_Name == L"Random_Shoot")
        return L"CROSS";
    else if (_Name == L"WindMill")
        return L"WINDMIL";
    else if (_Name == L"Seven'sFlow")
        return L"Cutsin";
    else
        return _Name;
}

void DH_Player::SpawnHitBox()
{
    //���̺�� ��Ʈ�ڽ��� ����
    m_PlayerTool->m_SaveHitMap;
    for (auto& Pair : m_PlayerTool->m_SaveHitMap)
    {
        auto it = find_if( m_SpawnedHitBoxes.begin(), m_SpawnedHitBoxes.end(),
            [&Pair](const HITBOX* pHitBox) 
            {
                return pHitBox->HitKey == Pair.first->HitKey; 
            });

        if (it != m_SpawnedHitBoxes.end())
            continue;

        if (Pair.first->HitKey == m_State && Pair.first->HitFrame == m_iFrame)
        {
            DH_HitBox* pHitBox = new DH_HitBox;
            pHitBox->m_HitRect = Pair.second;
            pHitBox->Initialize();
            DH_OBJMgr::Get_Instance()->Add_Object(OBJ_SHIELD, pHitBox);

            // ��ȯ�� ��Ʈ�ڽ� ���
            m_SpawnedHitBoxes.push_back(Pair.first);
        }
    }
}


