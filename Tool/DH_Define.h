#pragma once
#include "Include.h"

extern float g_Ratio;

//OBJ STATE
enum class STATE { IDLE, WALK, FALLING, JUMP, JUMPATTACK, ATTACK, DASH, SKILL1, HIT, DEAD, END };

// ���� 2�� ����ü
struct tObjInfo { D3DXVECTOR3 tPos; D3DXVECTOR3 tScale; };

typedef	struct tagSkillData
{

    CString strName;            // ��ų �̸�
    int iFrame = 0;             // ��ų �ִϸ��̼� ������
    int iAttack = 0;            // ��ų ���ݷ�
    int iManaCost = 0;          // ���� �Һ�
    float fCooldown = 0.0f;     // ��Ÿ�� (�� ����)
    int iRange = 0;             // ��ų ��Ÿ�
    int iDuration = 0;          // ���� �ð� (�и���)
    CString strType;            // ��ų ���� (��: "����", "���", "����")
    bool bIsAoE = false;        // ���� ��ų ����
    int iHitCount = 0;          // Ÿ�� Ƚ��

}SKILLDATA;

#define KEY_CHECK(key, state) DH_KeyMgr::Get_Instance()->GetKEY_STATE(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)