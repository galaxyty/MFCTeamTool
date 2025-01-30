// CEquipTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CEquipTool.h"
#include "DH_OBJMgr.h"
#include "DH_Interface.h"
#include "DH_Define.h"
#include "DH_UIMgr.h"
#include "DH_SkillUI.h"
#include "DH_UI.h"
#include "DH_Inventory.h"
#include "CDH_FilePath.h"
#include "DH_Item.h"
#include "DH_UI.h"


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
	DDX_Control(pDX, IDC_LIST1, m_ListIven);
	DDX_Control(pDX, IDC_RADIO1, m_Item[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Item[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Item[2]);
	DDX_Control(pDX, IDC_RADIO4, m_Item[3]);
	DDX_Control(pDX, IDC_LIST3, m_IvenAdd);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_LIST2, m_SkillList);
	DDX_Control(pDX, IDC_LIST4, m_SkillAdd);
	DDX_Control(pDX, IDC_RADIO6, m_JobSelect[0]);
	DDX_Control(pDX, IDC_RADIO5, m_JobSelect[1]);
	DDX_Control(pDX, IDC_RADIO7, m_JobSelect[2]);
	DDX_Control(pDX, IDC_RADIO8, m_JobSelect[3]);
	DDX_Control(pDX, IDC_PICTURE2, m_PictureSkill);
}


BEGIN_MESSAGE_MAP(CEquipTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CEquipTool::OnInventory)
	ON_BN_CLICKED(IDC_BUTTON2, &CEquipTool::OnSkill)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEquipTool::OnListInven)
	ON_BN_CLICKED(IDC_RADIO1, &CEquipTool::OnWeopon)
	ON_BN_CLICKED(IDC_RADIO2, &CEquipTool::OnAmor)
	ON_BN_CLICKED(IDC_RADIO3, &CEquipTool::OnAcce)
	ON_BN_CLICKED(IDC_RADIO4, &CEquipTool::OnPotion)
	ON_LBN_SELCHANGE(IDC_LIST3, &CEquipTool::OnInvenAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &CEquipTool::OnAddIven)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON5, &CEquipTool::OnDeleteIven)
	ON_LBN_SELCHANGE(IDC_LIST2, &CEquipTool::OnSkillList)
	ON_BN_CLICKED(IDC_BUTTON4, &CEquipTool::OnAddSkill)
	ON_BN_CLICKED(IDC_BUTTON6, &CEquipTool::OnDeleteSkill)
END_MESSAGE_MAP()


// CEquipTool 메시지 처리기


//이니셜라이즈
BOOL CEquipTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	//인벤토리 추가
	Inventory = new DH_Inventory;
	Inventory->SetName(L"Inventory");
	Inventory->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Inventory);

	//스킬 추가
	Skill = new DH_SkillUI;
	Skill->SetName(L"Skill");
	Skill->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Skill);


	//인터페이스 추가
	Interface = new DH_Interface;
	Interface->SetName(L"Interface");
	Interface->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Interface);

	
	wcscpy_s(m_szFileName, L"../Texture/Picked/Item/Amor");
	OnLoadPngFiles(m_szFileName, &m_ListIven);

	wcscpy_s(m_szSkillName, L"../Texture/Picked/Skill/On");
	OnLoadPngFiles(m_szSkillName, &m_SkillList);

	m_Item[1].SetCheck(true);

	return TRUE;  // return TRUE unless you set the focus to a control
}

//UI 토글
void CEquipTool::OnInventory()
{
	UIToggle(Inventory);
}
void CEquipTool::OnSkill()
{
	UIToggle(Skill);
}
void CEquipTool::UIToggle(DH_UI* _UI)
{
	if (_UI)
	{
		bool isVisible = _UI->IsVisible();
		_UI->SetVisible(!isVisible); // 현재 상태를 반전

		vector<DH_UI*>& vecChild = _UI->GetChildUI();
		for (auto& Child : vecChild)
		{
			Child->SetVisible(!isVisible);
			vector<DH_UI*>& DoubleChilds = Child->GetChildUI();
			for (auto& DoubleChild : DoubleChilds)
			{
				DoubleChild->SetVisible(!isVisible);
			}
		}

		if (!isVisible)
		{
			DH_UIMgr::Get_Instance()->SetFocusedUI(_UI); // UI 활성화 시 포커스 설정
		}
	}
}

//인벤리스트
void CEquipTool::OnListInven()
{
	UpdateData(TRUE);

	//클릭한 아이템
	int	iIndex = m_ListIven.GetCurSel();
	m_ListIven.GetText(iIndex, PickedName);

	//아이템 미리보기 만들자
	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";

	//상대경로와 이미지 합치기
	swprintf_s(szFilePath, MAX_PATH, L"%s/%s.png", m_szFileName, PickedName.GetString());

	//맵에서 찾기
	auto	iter = m_mapPngImage.find(PickedName);

	//못찾을 시
	if (iter == m_mapPngImage.end())
	{
		//이미지 추가(상대경로)
		CImage* pPngImage = new CImage;
		pPngImage->Load(szFilePath);

		//이미지를 맵에 추가
		iter = m_mapPngImage.insert({ PickedName, pPngImage });
		m_Picture.SetBitmap(*(iter->second));
	}
	else
	{
		m_Picture.SetBitmap(*(iter->second));
	}
	
	UpdateData(FALSE);
}
void CEquipTool::OnInvenAdd()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

//스킬리스트
void CEquipTool::OnSkillList()
{
	UpdateData(TRUE);

	//클릭한 스킬
	int	iIndex = m_SkillList.GetCurSel();
	m_SkillList.GetText(iIndex, PickedSkill);

	//아이템 미리보기 만들자
	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";

	//상대경로와 이미지 합치기
	swprintf_s(szFilePath, MAX_PATH, L"%s/%s.png", m_szSkillName, PickedSkill.GetString());

	//맵에서 찾기
	auto	iter = m_mapPngImage.find(PickedSkill);

	//못찾을 시
	if (iter == m_mapPngImage.end())
	{
		//이미지 추가(상대경로)
		CImage* pPngImage = new CImage;
		pPngImage->Load(szFilePath);

		//이미지를 맵에 추가
		iter = m_mapPngImage.insert({ PickedSkill, pPngImage });
		m_PictureSkill.SetBitmap(*(iter->second));
	}
	else
	{
		m_PictureSkill.SetBitmap(*(iter->second));
	}

	UpdateData(FALSE);
}



//아이템 최신화
void CEquipTool::LoadPngFiles(const CString& folderPath, CListBox* _ListBox)
{
	// 경로 끝에 '\' 추가 (필요 시)
	CString searchPath = folderPath;
	if (folderPath.Right(1) != _T("\\"))
		searchPath += _T("\\");
	searchPath += _T("*.png"); // .png 파일만 검색

	// CFileFind로 파일 검색
	CFileFind finder;
	BOOL isFileFound = finder.FindFile(searchPath); // 첫 번째 파일 검색

	while (isFileFound)
	{
		isFileFound = finder.FindNextFile(); // 다음 파일 검색

		CString filePath = finder.GetFilePath(); // 파일 경로 가져오기
		CString RelativePath = CDH_FilePath::Convert_RelativePath(filePath);
		CString fileName = PathFindFileName(RelativePath);

		// 확장자 명을 제거하는 함수
		TCHAR	szFileName[MAX_STR] = L"";
		lstrcpy(szFileName, fileName.GetString());
		PathRemoveExtension(szFileName);

		_ListBox->AddString(szFileName); // 리스트 박스에 추가
	}

	finder.Close(); // 검색 종료
}
void CEquipTool::OnLoadPngFiles(CString _Path, CListBox* _ListBox)
{
	// 초기 폴더 경로 지정
	CString initialFolderPath = _Path; // 원하는 폴더 경로
	_ListBox->ResetContent(); // 리스트 박스 초기화
	LoadPngFiles(initialFolderPath, _ListBox); // 재귀 호출로 모든 PNG 파일 읽기
}


//아이템 띄우기
void CEquipTool::OnWeopon()
{
	wcscpy_s(m_szFileName, L"../Texture/Picked/Item/Weapon");
	OnLoadPngFiles(m_szFileName, &m_ListIven);
}
void CEquipTool::OnAmor()
{
	wcscpy_s(m_szFileName, L"../Texture/Picked/Item/Amor");
	OnLoadPngFiles(m_szFileName, &m_ListIven);
}
void CEquipTool::OnAcce()
{
	wcscpy_s(m_szFileName, L"../Texture/Picked/Item/Acce");
	OnLoadPngFiles(m_szFileName, &m_ListIven);
}
void CEquipTool::OnPotion()
{
	wcscpy_s(m_szFileName, L"../Texture/Picked/Item/Potion");
	OnLoadPngFiles(m_szFileName, &m_ListIven);
}



//인벤추가, 삭제 버튼
void CEquipTool::OnAddIven()
{
	UpdateData(TRUE);

	//선택한 파일의 이름
	CString	strFindName;
	int	iIndex = m_ListIven.GetCurSel();
	m_ListIven.GetText(iIndex, strFindName);

	UNITDATA* pUnit = new UNITDATA;

	pUnit->strName = strFindName;
	pUnit->iAttack = 0;
	pUnit->iHp = 0;
	pUnit->byItem = 0;
	pUnit->byJobIndex = 0;

	m_mapUnitData.insert(make_pair(strFindName, pUnit));

	// AddString : 리스트 박스에 문자열을 삽입
	m_IvenAdd.AddString(pUnit->strName);
	AddItem(strFindName.GetString(), strFindName);

	UpdateData(FALSE);
}
void CEquipTool::OnDeleteIven()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_IvenAdd.GetCurSel();

	m_IvenAdd.GetText(iIndex, strFindName);

	auto it = m_mapUnitData.find(strFindName);
	if (it != m_mapUnitData.end())
	{
		delete it->second;
		m_mapUnitData.erase(it);
	}

	m_IvenAdd.DeleteString(iIndex);

	UpdateData(FALSE);
}

//스킬추가, 삭제 버튼
void CEquipTool::OnAddSkill()
{
	UpdateData(TRUE);

	//선택한 파일의 이름
	CString	strFindName;
	int	iIndex = m_SkillList.GetCurSel();
	m_SkillList.GetText(iIndex, strFindName);

	SKILLDATA* pSkill = new SKILLDATA;
	pSkill->strName = strFindName;

	m_mapSkillData.insert(make_pair(strFindName, pSkill));

	// AddString : 리스트 박스에 문자열을 삽입
	// 확장자 명을 제거하는 함수
	TCHAR	szFileName[MAX_STR] = L"";
	lstrcpy(szFileName, strFindName.GetString());
	PathRemoveExtension(szFileName);
	m_SkillAdd.AddString(szFileName);


	UpdateData(FALSE);
}
void CEquipTool::OnDeleteSkill()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_SkillAdd.GetCurSel();

	m_SkillAdd.GetText(iIndex, strFindName);

	auto it = m_mapSkillData.find(strFindName);
	if (it != m_mapSkillData.end())
	{
		delete it->second;
		m_mapSkillData.erase(it);
	}


	m_SkillAdd.DeleteString(iIndex);

	UpdateData(FALSE);
}



//소멸자
void CEquipTool::OnDestroy()
{
	CDialog::OnDestroy();

	multimap<CString, UNITDATA*>::iterator iter = m_mapUnitData.begin();
	for (; iter != m_mapUnitData.end(); ++iter)
	{
		delete iter->second;
	}
	multimap<CString, SKILLDATA*>::iterator iter2 = m_mapSkillData.begin();
	for (; iter2 != m_mapSkillData.end(); ++iter2)
	{
		delete iter2->second;
	}

	for_each(m_mapPngImage.begin(), m_mapPngImage.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});

	m_mapPngImage.clear();

}




void CEquipTool::AddItem(wstring _Item, CString _CString)
{
	DH_Item* Item = new DH_Item;
	Item->SetName(_Item);

	//for 문을 돌리면서 자식의 자식이 있는지 확인
	for (size_t i = 0; i < Inventory->GetChildUI().size(); ++i)
	{
		if (Inventory->GetChildUI()[i]->GetChildUI().size() > 0)
		{
			continue;
		}
		else
		{
			Item->SetMPos(D3DXVECTOR3{ 0.f, 0.f, 0.f });
			Item->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
			Item->SetImageKey(_CString);
			Item->Initialize();
			Inventory->GetChildUI()[i]->AddParent(Item);
			break;
		}
	}
}