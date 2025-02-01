#include "pch.h"
#include "DH_Interface.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"

DH_Interface::DH_Interface()
{
}

DH_Interface::~DH_Interface()
{
}

void DH_Interface::Initialize()
{
	TCHAR* UI = L"Interface";
	
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/Interface.png",
		TEX_SINGLE, UI, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}

	//행렬 포지션
	SetMPos(D3DXVECTOR3{ 0.f, 0.f, 0.f });

	//이미지 크기
	SetScale(D3DXVECTOR3{ 800.f, 600.f, 0.f });
}

void DH_Interface::Update()
{
}

void DH_Interface::Render(CToolView* pMainView)
{

	D3DXMATRIX	matWorld, matScale, matTrans;


	//이미지 키 가져오기 밑 중심설정
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Interface", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//월드 행렬 곱해주기
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//스크롤 값 제외
	D3DXMatrixTranslation(&matTrans,
		GetMPos().x + GetScale().x / 2, 
		GetMPos().y + GetScale().y / 2, 
		GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


	// 렌더링
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
		nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
		&vTemp,			// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
		nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지

}

void DH_Interface::MouseOn()
{
}

void DH_Interface::MouseLDown()
{
}

void DH_Interface::MouseLUp()
{
}

void DH_Interface::MouseLClicked()
{
}
