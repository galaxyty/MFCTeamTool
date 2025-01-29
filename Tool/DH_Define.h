#pragma once
#include "Include.h"

extern float g_Ratio;

//OBJ STATE
enum class STATE { IDLE, WALK, FALLING, JUMP, JUMPATTACK, ATTACK, DASH, SKILL1, HIT, DEAD, END };

// 벡터 2개 구조체
struct tObjInfo { D3DXVECTOR3 tPos; D3DXVECTOR3 tScale; };

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