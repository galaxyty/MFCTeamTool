﻿#pragma once

#include "CMapTool.h"
#include "CEquipTool.h"
#include "CPlayerTool.h"

class pPlayer;

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CFont		m_Font;
	DH_Player*  pPlayer;
public:
	CMapTool	m_MapTool;
	CEquipTool	m_EquipTool;
	CPlayerTool m_PlayerTool;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnUnitTool();
	afx_msg void OnMapButton();
	afx_msg void OnEquipButton();
	afx_msg void OnPlayer();
	afx_msg void OnPlay();
	afx_msg void OnDestroy();
};


