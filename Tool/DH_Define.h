#pragma once
#include "Include.h"

//OBJ STATE
enum class STATE { IDLE, WALK, FALLING, JUMP, JUMPATTACK, ATTACK, DASH, SKILL1, HIT, DEAD, END };

// ���� 2�� ����ü
struct tObjInfo { D3DXVECTOR3 tPos; D3DXVECTOR3 tScale; };
