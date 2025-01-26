// CEquipTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CEquipTool.h"
#include "DH_OBJMgr.h"
#include "DH_Interface.h"
#include "DH_Define.h"


// CEquipTool 대화 상자

IMPLEMENT_DYNAMIC(CEquipTool, CDialog)

CEquipTool::CEquipTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CEquipTool, pParent)
{

}

CEquipTool::~CEquipTool()
{

}

void CEquipTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEquipTool, CDialog)
END_MESSAGE_MAP()


// CEquipTool 메시지 처리기


//이니셜라이즈
BOOL CEquipTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	Interface = new DH_Interface;
	Interface->SetName(L"Interface");
	Interface->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Interface);


	return TRUE;  // return TRUE unless you set the focus to a control
}
