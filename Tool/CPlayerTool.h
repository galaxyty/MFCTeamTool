#pragma once
#include "afxdialogex.h"

#include "Include.h"


// CPlayerTool 대화 상자

class CPlayerTool : public CDialog
{
	DECLARE_DYNAMIC(CPlayerTool)

public:
	CPlayerTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPlayerTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPlayerTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPathList();
	afx_msg void OnPathSave();
	afx_msg void OnPathLoad();

public:
	void DisplayFolder(wstring FilePath);
	void FrameListUpdate();
	void UpdateTimerSpeed();

public:
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClickAnimation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFrameList();
	afx_msg void OnMoveRight();
	multimap<CString, CImage*>			m_mapPlayerImage;

	CRect				m_Rect;
	CListBox			m_PathList;
	list<IMGPATH*>		m_PathInfoList;
	CListCtrl			m_ImageInfo;
	CImageList			m_ImageList;
	TCHAR				m_PlayerState[MAX_STR];
	wstring				m_PlayerPath;
	wstring				m_AnimationPath;
	CListBox			m_FrameList;
	CStatic				m_Picture01;
	CEdit				m_Delay;
	CProgressCtrl		m_Gage;
	int					m_iOffsetX;
	int					m_iOffsetY;
	int					m_CurrentSelection;
	bool				m_Play;
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDonw();
	afx_msg void OnPlayAnimation();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStopAnimation();
};
