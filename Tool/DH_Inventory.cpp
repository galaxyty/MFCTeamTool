#include "pch.h"
#include "DH_Inventory.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"

DH_Inventory::DH_Inventory()
{
}

DH_Inventory::~DH_Inventory()
{
}

void DH_Inventory::Initialize()
{
	TCHAR* Inven = L"Inven";

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Picked/Interface/Inven.png",
		TEX_SINGLE, Inven, nullptr, 0)))
	{
		AfxMessageBox(L"Background Texture Insert Failed");
		return;
	}

	SetMPos(D3DXVECTOR3{ 600.f, 300.f, 0.f });

	SetScale(D3DXVECTOR3{ 260.f, 530.f, 0.f });
}

void DH_Inventory::Update()
{
	DH_UI::Update();
}

void DH_Inventory::Render(CToolView* pMainView)
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ���������� ����


	D3DXMATRIX	matWorld, matScale, matTrans;


	//�̹��� Ű �������� �� �߽ɼ���
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Inven", nullptr);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };


	//���� ��� �����ֱ�
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	//��ũ�� �� ����
	D3DXMatrixTranslation(&matTrans,
		GetMPos().x, 
		GetMPos().y, 
		GetMPos().z);

	matWorld = matScale * matTrans;

	Set_Ratio(&matWorld, g_Ratio, g_Ratio);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	if (m_bLDown)
	{
		// ������
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
			nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
			&vTemp,			// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
			nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����
	}
	else
	{
		// ������
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
			nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
			&vTemp,			// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
			nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����
	}


	RenderChild();
}


void DH_Inventory::MouseOn()
{
	if (IsbLDown())
	{
		//���콺 ��ǥ ��������
		D3DXVECTOR3 vDiff = Get_Mouse();
		vDiff.x -= m_vDragStart.x;
		vDiff.y -= m_vDragStart.y;

		//���� �κ� ��ǥ��������
		D3DXVECTOR3 vCurPos = GetMPos();
		vCurPos = vCurPos + vDiff;
		SetMPos(vCurPos);

		m_vDragStart = Get_Mouse();
	}
}

void DH_Inventory::MouseLDown()
{
	m_vDragStart = Get_Mouse();
}
void DH_Inventory::MouseLUp()
{
}

void DH_Inventory::MouseLClicked()
{
}
