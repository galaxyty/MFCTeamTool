﻿#pragma once
#include "afxdialogex.h"


// CMapTool 대화 상자

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMapTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnDropFiles(HDROP hDropInfo);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CSliderCtrl m_RatioSlider;
	afx_msg void OnSlider(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void UpdateRender();

	// 절대 경로를 상대 경로로 변환.
	CString ConvertRelativePath(CString _path);
public:
	CStatic m_RatioText;
	CListBox m_ListBoxMap;
	map<CString, CImage*> m_mapBackground;
	TCHAR* m_mapKey = new TCHAR[MAX_PATH];

	afx_msg void OnDestroy();
	CStatic m_BGPicture;
	afx_msg void OnListBGClick();
	afx_msg void OnApplyClick();
	afx_msg void OnDeleteClick();
};
