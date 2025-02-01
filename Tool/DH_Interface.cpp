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

	//��� ������
	SetMPos(D3DXVECTOR3{ 0.f, 0.f, 0.f });

	//�̹��� ũ��
	SetScale(D3DXVECTOR3{ 800.f, 600.f, 0.f });
}

void DH_Interface::Update()
{
}

void DH_Interface::Render(CToolView* pMainView)
{

	D3DXMATRIX	matWorld, matScale, matTrans;


	//�̹��� Ű �������� �� �߽ɼ���
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Interface", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//���� ��� �����ֱ�
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//��ũ�� �� ����
	D3DXMatrixTranslation(&matTrans,
		GetMPos().x + GetScale().x / 2, 
		GetMPos().y + GetScale().y / 2, 
		GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


	// ������
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
		nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
		&vTemp,			// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
		nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����

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
