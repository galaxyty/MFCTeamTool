#pragma once
#include "afxdialogex.h"
#include "Include.h"
#include "DH_Define.h"

// 전방선언
class DH_Inventory;
class DH_Interface;
class DH_SkillUI;
class DH_MyState;
class DH_Player;
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
	DH_MyState*		MyState;
	DH_SkillUI*		Skill;
	DH_Player*		pPlayer;


	afx_msg void OnInventory();


public:
	void LoadPngFiles(const CString& folderPath, CListBox* _ListBox);
	void OnLoadPngFiles(CString _Path, CListBox* _ListBox);
	void AddItem(wstring _Item, CString _CString, ITEMDATA* pItemData, DH_UI* pParrent = nullptr);
	void DeleteItem(CString _CString);
	void AddSkill(wstring _Item, CString _CString, SKILLDATA* pItemData, DH_UI* pParrent = nullptr);
	void DeleteSkill(CString _CString);
	void SetItemInfo(CString _CString, ITEMDATA* pItemData);

public:
	void OnEquipSave();
	void OnEquipLoad();
	void OnSkillSave();
	void OnSkillLoad();

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
	multimap<CString, ITEMDATA*>		m_mapItemData;
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
	afx_msg void OnMyState();
	afx_msg void OnItemSave();
	afx_msg void OnItemLoad();
	afx_msg void OnSkillSaveBtn();
	afx_msg void OnSkillLoadBtn();
};
