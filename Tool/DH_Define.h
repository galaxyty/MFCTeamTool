#pragma once
#include "Include.h"

extern float g_Ratio;

//OBJ STATE
enum class STATE { IDLE, WALK, FALLING, JUMP, JUMPATTACK, ATTACK, DASH, SKILL1, HIT, DEAD, END };

enum class ITEM { WEAPON, ACCE, AMOR, POTION, END };

enum class ITEMPARTS { SHOULDER, TOP, BOTTOM, BELT, SHOES, BOOK, JEWERLY, WEAPON, SYMBOL, BRACELET, NECK, RING , POTION, SKILL, ETC , END };

enum class ELEMENT { FIRE, ICE, DARK, LIGHT, POSION, NORMAL, END  };

// ���� 2�� ����ü
struct tObjInfo { D3DXVECTOR3 tPos; D3DXVECTOR3 tScale; };

typedef	struct tagItemData
{
    CString     strExplan;          // ����
    ITEMPARTS   eItemPart;          // ����
    ELEMENT     eElement;           // �Ӽ�
    CString     strName;            // �̸�
    float       fCriticalRate;      // ġ��Ÿ Ȯ��
    float       fWeight;            // ����
    int         iDurability;        // ������
    int         iRequiredLevel;     // ���� �䱸ġ
    bool        bEnchantable;       // ��ȭ ���� ����

    int iHP;       // ü��
    int iMP;       // ����
        
    int iIntelligence;  // ����
    int iStrength;      // ü��(����)
    int iPhysicalAtk;   // ���� ����
    int iMagicAtk;      // ���� ����
    int iFireResist;    // ȭ�� ����
    int iLightResist;   // ������
    int iManaResist;    // �׸���
        
    int iAgility;       // �̼�
    int iAttackSpeed;   // ����
    int iPhysicalDef;   // ���� ���
    int iMagicDef;      // ���� ���
    int iWaterResist;   // ������
    int iDarkResist;    // ������


}ITEMDATA;

typedef	struct tagSkillData
{

    CString strName;            // ��ų �̸�
    CString strType;            // ��ų ���� (��: "����", "���", "����")
    float fCooldown = 0.0f;     // ��Ÿ�� (�� ����)
    int iFrame = 0;             // ��ų �ִϸ��̼� ������
    int iAttack = 0;            // ��ų ���ݷ�
    int iManaCost = 0;          // ���� �Һ�
    int iRange = 0;             // ��ų ��Ÿ�
    int iDuration = 0;          // ���� �ð� (�и���)
    int iHitCount = 0;          // Ÿ�� Ƚ��
    bool bIsAoE = false;        // ���� ��ų ����

}SKILLDATA;


typedef	struct tagPlayerData
{
    int iHP;       // ü��
    int iMP;       // ����
        
    int iIntelligence;  // ����
    int iStrength;      // ü��(����)
    int iPhysicalAtk;   // ���� ����
    int iMagicAtk;      // ���� ����
    int iFireResist;    // ȭ�� ����
    int iLightResist;   // ������
    int iManaResist;    // �׸���
        
    int iAgility;       // �̼�
    int iAttackSpeed;   // ����
    int iPhysicalDef;   // ���� ���
    int iMagicDef;      // ���� ���
    int iWaterResist;   // ������
    int iDarkResist;    // ������
    
    tagPlayerData()
        : iHP(0), iMP(0), iIntelligence(0), iStrength(0), iPhysicalAtk(0), iMagicAtk(0),
        iFireResist(0), iLightResist(0), iDarkResist(0), iAgility(0), iAttackSpeed(0),
        iPhysicalDef(0), iMagicDef(0), iWaterResist(0), iManaResist(0){}
}PLAYERDATA;



#define KEY_CHECK(key, state) DH_KeyMgr::Get_Instance()->GetKEY_STATE(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)