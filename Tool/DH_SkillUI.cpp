#include "pch.h"
#include "DH_SkillUI.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"
#include "DH_BtnUI.h"


DH_SkillUI::DH_SkillUI() : Interface(nullptr)
{
}

DH_SkillUI::~DH_SkillUI()
{
	for (auto& ChildUI : GetChildUI())
	{
		delete ChildUI;
		ChildUI = nullptr;
	}
}

void DH_SkillUI::Initialize()
{
	TCHAR* Skill = L"Skill";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/Skill.png",
		TEX_SINGLE, Skill, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}

	SetMPos(D3DXVECTOR3{ 347.f, 460.f, 0.f });
	SetScale(D3DXVECTOR3{ 245.f, 200.f, 0.f });

	SetButten();
}

void DH_SkillUI::Update()
{
	DH_UI::Update();
}

void DH_SkillUI::Render(CToolView* pMainView)
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 렌더링하지 않음


	D3DXMATRIX	matWorld, matScale, matTrans;


	//이미지 키 가져오기 밑 중심설정
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Skill", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//월드 행렬 곱해주기
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//스크롤 값 제외
	D3DXMatrixTranslation(&matTrans,
		GetMPos().x,
		GetMPos().y,
		GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	if (m_bLDown)
	{
		// 렌더링
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		// 렌더링
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&vTemp,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255)); 
	}


	RenderChild();
}


void DH_SkillUI::MouseOn()
{
	if (IsbLDown())
	{
		//마우스 좌표 가져오기
		D3DXVECTOR3 vDiff = Get_Mouse();
		vDiff.x -= m_vDragStart.x;
		vDiff.y -= m_vDragStart.y;

		//현재 인벤 좌표가져오기
		D3DXVECTOR3 vCurPos = GetMPos();
		vCurPos = vCurPos + vDiff;
		SetMPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}
}

void DH_SkillUI::MouseLDown()
{
	m_vDragStart = Get_Mouse();
}
void DH_SkillUI::MouseLUp()
{
}

void DH_SkillUI::MouseLClicked()
{
}

void DH_SkillUI::SetButten()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			DH_BtnUI* Btn = new DH_BtnUI;

			wstring name = L"Btn_" + std::to_wstring(i) + L"_" + std::to_wstring(j);
			Btn->SetName(name);

			Btn->SetMPos(D3DXVECTOR3{ -94.f + (30 * j), -49.f + (30 * i), 0.f });
			Btn->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
			Btn->SeteItemParts(ITEMPARTS::SKILL);
			Btn->Initialize();
			AddParent(Btn);
		}
	}
}

