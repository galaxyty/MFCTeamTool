#pragma once
#include "Include.h"

//OBJ STATE
enum class STATE { IDLE, WALK, FALLING, JUMP, JUMPATTACK, ATTACK, DASH, SKILL1, HIT, DEAD, END };

// 벡터 2개 구조체
struct tObjInfo { D3DXVECTOR3 tPos; D3DXVECTOR3 tScale; };
