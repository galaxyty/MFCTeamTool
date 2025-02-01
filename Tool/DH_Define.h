#pragma once
#include "Include.h"

extern float g_Ratio;

//OBJ STATE
enum class STATE { IDLE, WALK, FALLING, JUMP, JUMPATTACK, ATTACK, DASH, SKILL1, HIT, DEAD, END };

enum class ITEM { WEAPON, ACCE, AMOR, POTION, END };

enum class ITEMPARTS { SHOULDER, TOP, BOTTOM, BELT, SHOES, BOOK, JEWERLY, WEAPON, SYMBOL, BRACELET, NECK, RING , POTION, ETC , END };

enum class ELEMENT { FIRE, ICE, DARK, LIGHT, POSION, NORMAL, END  };

// 벡터 2개 구조체
struct tObjInfo { D3DXVECTOR3 tPos; D3DXVECTOR3 tScale; };

typedef	struct tagItemData
{
    CString     strSpecialEffect;   // 특수 효과
    ITEMPARTS   eItemPart;          // 부위
    ELEMENT     eElement;           // 속성
    CString     strName;            // 이름
    float       fCriticalRate;      // 치명타 확률
    float       fAttackSpeed;       // 공격 속도
    float       fWeight;            // 무게
    int         iHp;                // 체력
    int         iAttack;            // 공격력
    int         iDurability;        // 내구도
    int         iRequiredLevel;     // 레벨 요구치
    int         iDefense;           // 방어력
    bool        bEnchantable;       // 강화 가능 여부

}ITEMDATA;

typedef	struct tagSkillData
{

    CString strName;            // 스킬 이름
    int iFrame = 0;             // 스킬 애니메이션 프레임
    int iAttack = 0;            // 스킬 공격력
    int iManaCost = 0;          // 마나 소비량
    float fCooldown = 0.0f;     // 쿨타임 (초 단위)
    int iRange = 0;             // 스킬 사거리
    int iDuration = 0;          // 지속 시간 (밀리초)
    CString strType;            // 스킬 유형 (예: "공격", "방어", "버프")
    bool bIsAoE = false;        // 광역 스킬 여부
    int iHitCount = 0;          // 타격 횟수

}SKILLDATA;

#define KEY_CHECK(key, state) DH_KeyMgr::Get_Instance()->GetKEY_STATE(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)