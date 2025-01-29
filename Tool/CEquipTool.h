#pragma once
#include "afxdialogex.h"
#include "Include.h"
#include "DH_Define.h"

// 전방선언
class DH_Inventory;
class DH_Interface;
class DH_SkillUI;
class DH_UI;


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
public:
	virtual BOOL OnInitDialog();

public:
	DH_Interface*	Interface;
	DH_Inventory*	Inventory;
	DH_SkillUI*		Skill;



	afx_msg void OnInventory();


public:
	void LoadPngFiles(const CString& folderPath, CListBox* _ListBox);
	void OnLoadPngFiles(CString _Path, CListBox* _ListBox);
	void AddItem(wstring _Item, CString _CString);

public:
	void UIToggle(DH_UI* _UI);
	afx_msg void OnSkill();
	afx_msg void OnListInven();
	CString	 PickedName;
	CString	 PickedSkill;
	CListBox m_ListIven;
	CListBox m_IvenAdd;
	CListBox m_SkillList;
	CListBox m_SkillAdd;
	CStatic m_Picture;
	CButton m_Item[4];
	CButton m_JobSelect[4];
	CStatic m_PictureSkill;
	TCHAR	m_szFileName[MAX_STR];
	TCHAR	m_szSkillName[MAX_STR];

	multimap<CString, CImage*>			m_mapPngImage;
	multimap<CString, UNITDATA*>		m_mapUnitData;
	multimap<CString, SKILLDATA*>		m_mapSkillData;
	afx_msg void OnWeopon();
	afx_msg void OnAmor();
	afx_msg void OnAcce();
	afx_msg void OnPotion();
	afx_msg void OnInvenAdd();
	afx_msg void OnAddIven();
	afx_msg void OnDestroy();
	afx_msg void OnDeleteIven();
	afx_msg void OnSkillList();
	afx_msg void OnAddSkill();
	afx_msg void OnDeleteSkill();
};
