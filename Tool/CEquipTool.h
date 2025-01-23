#pragma once
#include "afxdialogex.h"


// CEquipTool 대화 상자

class CEquipTool : public CDialog
{
	DECLARE_DYNAMIC(CEquipTool)

public:
	CEquipTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEquipTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CEquipTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
