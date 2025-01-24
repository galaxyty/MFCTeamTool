// CEquipTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CEquipTool.h"


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
