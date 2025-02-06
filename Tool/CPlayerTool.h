#pragma once
#include "afxdialogex.h"

#include "Include.h"
#include "DH_Define.h"


// CPlayerTool 대화 상자
class DH_HitBox;

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
	void LoadHitBox(HITBOX _HITBOX);

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
	int					m_iPrev = -1;
	int					m_iOffsetX;
	int					m_iOffsetY;
	int					m_CurrentSelection;
	bool				m_Play;
	bool				m_bDragging = false;   // 드래그 상태 확인
	bool				m_bCheckHit = false;
	CPoint				m_StartPoint;        // 마우스 시작 좌표
	CPoint				m_EndPoint;          // 현재 마우스 좌표
	CRect				m_HitRect;               // 드래그 박스 영역
	DH_HitBox*			m_HitBox;
	wstring				m_SkillKey;
	map<HITBOX*, RECT>	m_SaveHitMap;

	afx_msg void OnMoveLeft();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDonw();
	afx_msg void OnPlayAnimation();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStopAnimation();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	void DrawOnPictureControl();

	afx_msg void OnCheckHit();
	CButton m_CheckHit;
	afx_msg void OnSaveBtn();
};
