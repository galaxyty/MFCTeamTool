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
#include "DH_BtnUI.h"
#include "DH_Item.h"
#include "DH_Skill.h"
#include "DH_MyState.h"
#include "DH_UI.h"
#include "DH_Player.h"


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
	ON_BN_CLICKED(IDC_BUTTON7, &CEquipTool::OnMyState)
	ON_BN_CLICKED(IDC_BUTTON8, &CEquipTool::OnItemSave)
	ON_BN_CLICKED(IDC_BUTTON9, &CEquipTool::OnItemLoad)
	ON_BN_CLICKED(IDC_BUTTON10, &CEquipTool::OnSkillSaveBtn)
	ON_BN_CLICKED(IDC_BUTTON11, &CEquipTool::OnSkillLoadBtn)
END_MESSAGE_MAP()


// CEquipTool 메시지 처리기


//이니셜라이즈
BOOL CEquipTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	//인터페이스 추가
	Interface = new DH_Interface;
	Interface->SetName(L"Interface");
	Interface->SetVisible(true);
	Interface->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Interface);

	//인벤토리 추가
	Inventory = new DH_Inventory;
	Inventory->SetName(L"Inventory");
	Inventory->SetInterface(Interface);
	Inventory->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Inventory);

	//스킬 추가
	Skill = new DH_SkillUI;
	Skill->SetName(L"Skill");
	Skill->SetInterface(Interface);
	Skill->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, Skill);

	//스테이트 추가
	MyState = new DH_MyState;
	MyState->SetName(L"MyState");
	MyState->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_UI, MyState);

	Interface->SetInventory(Inventory);
	Interface->SetSkillUI(Skill);

	//플레이어 추가
	pPlayer = DH_Player::Get_Instance();
	pPlayer->SetName(L"pPlayer");
	pPlayer->SetInventory(Inventory);
	pPlayer->SetInterface(Interface);
	pPlayer->SetMyState(MyState);
	pPlayer->Initialize();
	DH_OBJMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);

	
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
void CEquipTool::OnMyState()
{
	UIToggle(MyState);
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


	//실제 추가 로직
	ITEMDATA* pItemData = new ITEMDATA;
	SetItemInfo(strFindName, pItemData);
	m_IvenAdd.AddString(strFindName);
	AddItem(strFindName.GetString(), strFindName, pItemData);

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

	auto it = m_mapItemData.find(strFindName);
	if (it != m_mapItemData.end())
	{
		delete it->second;
		m_mapItemData.erase(it);
	}

	m_IvenAdd.DeleteString(iIndex);
	DeleteItem(strFindName);

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
	AddSkill(strFindName.GetString(), strFindName, pSkill);

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
	DeleteSkill(strFindName);

	UpdateData(FALSE);
}



//소멸자
void CEquipTool::OnDestroy()
{
	CDialog::OnDestroy();

	multimap<CString, ITEMDATA*>::iterator iter = m_mapItemData.begin();
	for (; iter != m_mapItemData.end(); ++iter)
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

	DH_Player::Destroy_Instance();
}



//아이템 추가, 삭제 로직
void CEquipTool::AddItem(wstring _Item, CString _CString, ITEMDATA* pItemData, DH_UI* pParrent)
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

			//아이템 구분을 해주고 그에 맞춰 셋팅해줘야함
			if(m_Item[0].GetCheck())
				Item->SetItem(ITEM::WEAPON);
			else if (m_Item[1].GetCheck())
				Item->SetItem(ITEM::AMOR);
			else if (m_Item[2].GetCheck())
				Item->SetItem(ITEM::ACCE); 
			else if (m_Item[3].GetCheck())
				Item->SetItem(ITEM::POTION);

			Item->SetImageKey(_CString);
			Item->SetpITEMDATA(pItemData);
			Item->SetVisible(true);
			Item->Initialize();

			if (!pParrent)
				Inventory->GetChildUI()[i]->AddParent(Item);
			else if (pParrent)
				pParrent->AddParent(Item);

			break;
		}
	}
}
void CEquipTool::DeleteItem(CString _CString)
{
	//for 문을 돌려가면서 해당이름이 있는 ui 를 탐색
	

	// 인벤토리 버튼 이터레이터 반복
	for (auto it = Inventory->GetChildUI().begin(); it != Inventory->GetChildUI().end(); ++it)
	{
		// 버튼의 자식이 해당 네임인지 확인
		if ((*it)->GetChildUI().size() > 0 && (*it)->GetChildUI().front()->GetName() == _CString.GetString())
		{
			// 버튼의 자식의 네이밍이 일치하면 삭제 후 탈출
			delete (*it)->GetChildUI().front();
			(*it)->GetChildUI().erase((*it)->GetChildUI().begin());
			break;
		}
	}
}

//스킬 추가, 삭제 로직
void CEquipTool::AddSkill(wstring _Item, CString _CString, SKILLDATA* pItemData, DH_UI* pParrent)
{
	DH_Skill* SkillIcon = new DH_Skill;
	SkillIcon->SetName(_Item);

	//for 문을 돌리면서 자식의 자식이 있는지 확인
	for (size_t i = 0; i < Skill->GetChildUI().size(); ++i)
	{
		if (Skill->GetChildUI()[i]->GetChildUI().size() > 0)
		{
			continue;
		}
		else
		{
			SkillIcon->SetMPos(D3DXVECTOR3{ 0.f, 0.f, 0.f });
			SkillIcon->SetScale(D3DXVECTOR3{ 28.f,28.f,0.f });
			SkillIcon->SetImageKey(_CString);
			SkillIcon->SetVisible(true);
			SkillIcon->SetSkillData(pItemData);
			SkillIcon->Initialize();

			if (!pParrent)
				Skill->GetChildUI()[i]->AddParent(SkillIcon);
			else if (pParrent)
				pParrent->AddParent(SkillIcon);

			
			break;
		}
	}
}
void CEquipTool::DeleteSkill(CString _CString)
{
	//for 문을 돌려가면서 해당이름이 있는 ui 를 탐색


	// 인벤토리 버튼 이터레이터 반복
	for (auto it = Skill->GetChildUI().begin(); it != Skill->GetChildUI().end(); ++it)
	{
		// 버튼의 자식이 해당 네임인지 확인
		if ((*it)->GetChildUI().size() > 0 && (*it)->GetChildUI().front()->GetName() == _CString.GetString())
		{
			// 버튼의 자식의 네이밍이 일치하면 삭제 후 탈출
			delete (*it)->GetChildUI().front();
			(*it)->GetChildUI().erase((*it)->GetChildUI().begin());
			break;
		}
	}
}

//아이템 인포
void CEquipTool::SetItemInfo(CString _CString, ITEMDATA* pItemData)
{
	//악세서리
	if (_CString == L"00.Band")
	{
		pItemData->strName = L"00.Band";
		pItemData->strExplan = L"손가락에 착용 시 방어력을 약간 증가시키는 단순한 금속 밴드입니다.";
		pItemData->ItemName = L"은은한 방어반";
		pItemData->eItemPart = ITEMPARTS::BRACELET;
		pItemData->eElement = ELEMENT::NORMAL;


		pItemData->fCriticalRate = 0.05f;       // 치명타 확률: 5%
		pItemData->fWeight = 0.1f;              // 무게: 가벼움
		pItemData->iDurability = 100;           // 내구도: 100
		pItemData->iRequiredLevel = 5;          // 요구 레벨: 5
		pItemData->bEnchantable = true;         // 강화 가능 여부: 가능

		pItemData->iHP = 50;                    // 체력: +50
		pItemData->iMP = 30;                    // 마나: +30
		pItemData->iIntelligence = 10;          // 지능: +10
		pItemData->iStrength = 5;               // 체력(스탯): +5
		pItemData->iPhysicalAtk = 15;           // 물리 공격: +15
		pItemData->iMagicAtk = 10;              // 마법 공격: +10
		pItemData->iFireResist = 3;             // 화염 저항: +3
		pItemData->iLightResist = 2;            // 명저항: +2
		pItemData->iManaResist = 5;             // 마나 저항: +5
		pItemData->iAgility = 8;                // 이속: +8
		pItemData->iAttackSpeed = 5;            // 공격 속도: +5
		pItemData->iPhysicalDef = 10;           // 물리 방어: +10
		pItemData->iMagicDef = 8;               // 마법 방어: +8
		pItemData->iWaterResist = 2;            // 수저항: +2
		pItemData->iDarkResist = 4;             // 암저항: +4

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Heart")
	{
		pItemData->strName = L"00.Heart";
		pItemData->strExplan = L"사용 시 랜덤한 효과를 발휘하는 신비로운 하트 형태의 아이템입니다.";
		pItemData->ItemName = L"신비의 심장석";
		pItemData->eItemPart = ITEMPARTS::JEWERLY;
		pItemData->eElement = ELEMENT::NORMAL;

		pItemData->fCriticalRate = 0.1f;           // 치명타 확률: 10%
		pItemData->fWeight = 0.05f;                // 무게: 매우 가벼움
		pItemData->iDurability = 80;               // 내구도: 80
		pItemData->iRequiredLevel = 10;            // 요구 레벨: 10
		pItemData->bEnchantable = true;            // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 100;                      // 체력: +100
		pItemData->iMP = 50;                       // 마나: +50
		pItemData->iIntelligence = 15;             // 지능: +15
		pItemData->iStrength = 8;                  // 힘: +8
		pItemData->iPhysicalAtk = 5;               // 물리 공격: +5
		pItemData->iMagicAtk = 20;                 // 마법 공격: +20
		pItemData->iFireResist = 5;                // 화염 저항: +5
		pItemData->iLightResist = 8;               // 명저항: +8
		pItemData->iManaResist = 10;               // 마나 저항: +10
		pItemData->iAgility = 10;                  // 이속: +10
		pItemData->iAttackSpeed = 5;               // 공격 속도: +5
		pItemData->iPhysicalDef = 5;               // 물리 방어: +5
		pItemData->iMagicDef = 10;                 // 마법 방어: +10
		pItemData->iWaterResist = 7;               // 수저항: +7
		pItemData->iDarkResist = 12;               // 암저항: +12

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Neck")
	{
		pItemData->strName = L"00.Neck";
		pItemData->strExplan = L"착용 시 마법 방어력을 강화하는 신비로운 빛을 내뿜는 목걸이입니다.";
		pItemData->ItemName = L"에테리얼 펜던트";
		pItemData->eItemPart = ITEMPARTS::NECK;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.05f;           // 치명타 확률: 5%
		pItemData->fWeight = 0.15f;                 // 무게: 약간 무거움
		pItemData->iDurability = 90;                // 내구도: 90
		pItemData->iRequiredLevel = 15;             // 요구 레벨: 15
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 50;                        // 체력: +50
		pItemData->iMP = 70;                        // 마나: +70
		pItemData->iIntelligence = 20;              // 지능: +20
		pItemData->iStrength = 5;                   // 힘: +5
		pItemData->iPhysicalAtk = 8;                // 물리 공격: +8
		pItemData->iMagicAtk = 25;                  // 마법 공격: +25
		pItemData->iFireResist = 8;                 // 화염 저항: +8
		pItemData->iLightResist = 10;               // 명저항: +10
		pItemData->iManaResist = 15;                // 마나 저항: +15
		pItemData->iAgility = 6;                    // 이속: +6
		pItemData->iAttackSpeed = 5;                // 공격 속도: +5
		pItemData->iPhysicalDef = 5;                // 물리 방어: +5
		pItemData->iMagicDef = 20;                  // 마법 방어: +20
		pItemData->iWaterResist = 10;               // 수저항: +10
		pItemData->iDarkResist = 12;                // 암저항: +12

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Ring1")
	{
		pItemData->strName = L"00.Ring1";
		pItemData->strExplan = L"마법의 힘을 담고 있어 착용자의 능력을 소폭 강화시키는 반지입니다.";
		pItemData->ItemName = L"아르카나 링";
		pItemData->eItemPart = ITEMPARTS::RING;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.08f;           // 치명타 확률: 8%
		pItemData->fWeight = 0.02f;                 // 무게: 매우 가벼움
		pItemData->iDurability = 50;                // 내구도: 50
		pItemData->iRequiredLevel = 8;              // 요구 레벨: 8
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 30;                        // 체력: +30
		pItemData->iMP = 60;                        // 마나: +60
		pItemData->iIntelligence = 15;              // 지능: +15
		pItemData->iStrength = 5;                   // 힘: +5
		pItemData->iPhysicalAtk = 10;               // 물리 공격: +10
		pItemData->iMagicAtk = 20;                  // 마법 공격: +20
		pItemData->iFireResist = 5;                 // 화염 저항: +5
		pItemData->iLightResist = 5;                // 명저항: +5
		pItemData->iManaResist = 8;                 // 마나 저항: +8
		pItemData->iAgility = 7;                    // 이속: +7
		pItemData->iAttackSpeed = 4;                // 공격 속도: +4
		pItemData->iPhysicalDef = 3;                // 물리 방어: +3
		pItemData->iMagicDef = 7;                   // 마법 방어: +7
		pItemData->iWaterResist = 4;                // 수저항: +4
		pItemData->iDarkResist = 6;                 // 암저항: +6

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Ring2")
	{
		pItemData->strName = L"00.Ring2";
		pItemData->strExplan = L"착용자에게 공격력을 증폭시키는 어둠의 기운이 깃든 반지입니다.";
		pItemData->ItemName = L"데스브링어 링";
		pItemData->eItemPart = ITEMPARTS::RING;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.12f;           // 치명타 확률: 12%
		pItemData->fWeight = 0.03f;                 // 무게: 가벼움
		pItemData->iDurability = 60;                // 내구도: 60
		pItemData->iRequiredLevel = 12;             // 요구 레벨: 12
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 40;                        // 체력: +40
		pItemData->iMP = 50;                        // 마나: +50
		pItemData->iIntelligence = 10;              // 지능: +10
		pItemData->iStrength = 12;                  // 힘: +12
		pItemData->iPhysicalAtk = 30;               // 물리 공격: +30
		pItemData->iMagicAtk = 15;                  // 마법 공격: +15
		pItemData->iFireResist = 6;                 // 화염 저항: +6
		pItemData->iLightResist = 4;                // 명저항: +4
		pItemData->iManaResist = 7;                 // 마나 저항: +7
		pItemData->iAgility = 9;                    // 이속: +9
		pItemData->iAttackSpeed = 6;                // 공격 속도: +6
		pItemData->iPhysicalDef = 5;                // 물리 방어: +5
		pItemData->iMagicDef = 6;                   // 마법 방어: +6
		pItemData->iWaterResist = 5;                // 수저항: +5
		pItemData->iDarkResist = 10;                // 암저항: +10

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Ring3")
	{
		pItemData->strName = L"00.Ring3";
		pItemData->strExplan = L"착용 시 마력을 증대시키며 푸른 에너지를 발산하는 신비로운 목걸이입니다.";
		pItemData->ItemName = L"블루 플레어 펜던트";
		pItemData->eItemPart = ITEMPARTS::NECK;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.1f;            // 치명타 확률: 10%
		pItemData->fWeight = 0.05f;                 // 무게: 가벼움
		pItemData->iDurability = 70;                // 내구도: 70
		pItemData->iRequiredLevel = 10;             // 요구 레벨: 10
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 20;                        // 체력: +20
		pItemData->iMP = 100;                       // 마나: +100
		pItemData->iIntelligence = 25;              // 지능: +25
		pItemData->iStrength = 5;                   // 힘: +5
		pItemData->iPhysicalAtk = 5;                // 물리 공격: +5
		pItemData->iMagicAtk = 30;                  // 마법 공격: +30
		pItemData->iFireResist = 8;                 // 화염 저항: +8
		pItemData->iLightResist = 10;               // 명저항: +10
		pItemData->iManaResist = 12;                // 마나 저항: +12
		pItemData->iAgility = 6;                    // 이속: +6
		pItemData->iAttackSpeed = 4;                // 공격 속도: +4
		pItemData->iPhysicalDef = 3;                // 물리 방어: +3
		pItemData->iMagicDef = 15;                  // 마법 방어: +15
		pItemData->iWaterResist = 12;               // 수저항: +12
		pItemData->iDarkResist = 7;                 // 암저항: +7


		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}

	//방어구
	else if (_CString == L"00.Muffler")
	{
		pItemData->strName = L"00.Muffler";
		pItemData->strExplan = L"착용 시 따뜻함과 함께 방어력을 소폭 증가시키는 황금빛 머플러입니다.";
		pItemData->ItemName = L"레디언트 웜스카프";
		pItemData->eItemPart = ITEMPARTS::NECK;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.05f;           // 치명타 확률: 5%
		pItemData->fWeight = 0.2f;                  // 무게: 약간 무거움
		pItemData->iDurability = 80;                // 내구도: 80
		pItemData->iRequiredLevel = 10;             // 요구 레벨: 10
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 60;                        // 체력: +60
		pItemData->iMP = 40;                        // 마나: +40
		pItemData->iIntelligence = 10;              // 지능: +10
		pItemData->iStrength = 12;                  // 힘: +12
		pItemData->iPhysicalAtk = 8;                // 물리 공격: +8
		pItemData->iMagicAtk = 5;                   // 마법 공격: +5
		pItemData->iFireResist = 10;                // 화염 저항: +10
		pItemData->iLightResist = 8;                // 명저항: +8
		pItemData->iManaResist = 6;                 // 마나 저항: +6
		pItemData->iAgility = 5;                    // 이속: +5
		pItemData->iAttackSpeed = 3;                // 공격 속도: +3
		pItemData->iPhysicalDef = 15;               // 물리 방어: +15
		pItemData->iMagicDef = 12;                  // 마법 방어: +12
		pItemData->iWaterResist = 10;               // 수저항: +10
		pItemData->iDarkResist = 7;                 // 암저항: +7

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"01.Belt2")
	{
		pItemData->strName = L"01.Belt2";
		pItemData->strExplan = L"공격 속도를 증가시키며 전투 능력을 향상시키는 강철 벨트입니다.";
		pItemData->ItemName = L"템페스트 벨트";
		pItemData->eItemPart = ITEMPARTS::BELT;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.1f;            // 치명타 확률: 10%
		pItemData->fWeight = 0.25f;                 // 무게: 약간 무거움
		pItemData->iDurability = 90;                // 내구도: 90
		pItemData->iRequiredLevel = 15;             // 요구 레벨: 15
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 50;                        // 체력: +50
		pItemData->iMP = 30;                        // 마나: +30
		pItemData->iIntelligence = 12;              // 지능: +12
		pItemData->iStrength = 20;                  // 힘: +20
		pItemData->iPhysicalAtk = 25;               // 물리 공격: +25
		pItemData->iMagicAtk = 10;                  // 마법 공격: +10
		pItemData->iFireResist = 7;                 // 화염 저항: +7
		pItemData->iLightResist = 5;                // 명저항: +5
		pItemData->iManaResist = 8;                 // 마나 저항: +8
		pItemData->iAgility = 15;                   // 이속: +15
		pItemData->iAttackSpeed = 10;               // 공격 속도: +10
		pItemData->iPhysicalDef = 10;               // 물리 방어: +10
		pItemData->iMagicDef = 8;                   // 마법 방어: +8
		pItemData->iWaterResist = 6;                // 수저항: +6
		pItemData->iDarkResist = 7;                 // 암저항: +7

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"01.Belt3")
	{
		pItemData->strName = L"01.Belt3";
		pItemData->strExplan = L"마법 에너지가 깃들어 착용자의 마력과 방어력을 증대시키는 신비로운 벨트입니다.";
		pItemData->ItemName = L"미스틱 오브 벨트";
		pItemData->eItemPart = ITEMPARTS::BELT;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.08f;           // 치명타 확률: 8%
		pItemData->fWeight = 0.2f;                  // 무게: 중간 정도
		pItemData->iDurability = 85;                // 내구도: 85
		pItemData->iRequiredLevel = 15;             // 요구 레벨: 15
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 60;                        // 체력: +60
		pItemData->iMP = 90;                        // 마나: +90
		pItemData->iIntelligence = 20;              // 지능: +20
		pItemData->iStrength = 10;                  // 힘: +10
		pItemData->iPhysicalAtk = 15;               // 물리 공격: +15
		pItemData->iMagicAtk = 25;                  // 마법 공격: +25
		pItemData->iFireResist = 10;                // 화염 저항: +10
		pItemData->iLightResist = 12;               // 명저항: +12
		pItemData->iManaResist = 15;                // 마나 저항: +15
		pItemData->iAgility = 8;                    // 이속: +8
		pItemData->iAttackSpeed = 6;                // 공격 속도: +6
		pItemData->iPhysicalDef = 12;               // 물리 방어: +12
		pItemData->iMagicDef = 18;                  // 마법 방어: +18
		pItemData->iWaterResist = 8;                // 수저항: +8
		pItemData->iDarkResist = 10;                // 암저항: +10


		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"02.Coat2")
	{
		pItemData->strName = L"02.Coat2";
		pItemData->strExplan = L"불꽃의 힘을 품고 있어 방어력과 화염 저항을 강화하는 붉은 코트입니다.";
		pItemData->ItemName = L"이그니스 코트";
		pItemData->eItemPart = ITEMPARTS::TOP;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.05f;           // 치명타 확률: 5%
		pItemData->fWeight = 1.0f;                  // 무게: 무거움
		pItemData->iDurability = 100;               // 내구도: 100
		pItemData->iRequiredLevel = 20;             // 요구 레벨: 20
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 100;                       // 체력: +100
		pItemData->iMP = 50;                        // 마나: +50
		pItemData->iIntelligence = 15;              // 지능: +15
		pItemData->iStrength = 20;                  // 힘: +20
		pItemData->iPhysicalAtk = 10;               // 물리 공격: +10
		pItemData->iMagicAtk = 10;                  // 마법 공격: +10
		pItemData->iFireResist = 25;                // 화염 저항: +25
		pItemData->iLightResist = 10;               // 명저항: +10
		pItemData->iManaResist = 12;                // 마나 저항: +12
		pItemData->iAgility = 5;                    // 이속: +5
		pItemData->iAttackSpeed = 3;                // 공격 속도: +3
		pItemData->iPhysicalDef = 30;               // 물리 방어: +30
		pItemData->iMagicDef = 20;                  // 마법 방어: +20
		pItemData->iWaterResist = 8;                // 수저항: +8
		pItemData->iDarkResist = 10;                // 암저항: +10

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"03.Coat3")
	{
		pItemData->strName = L"03.Coat3";
		pItemData->strExplan = L"마법 보호막을 생성하여 방어력과 마법 저항을 크게 증가시키는 고급 코트입니다.";
		pItemData->ItemName = L"인챈티드 워드 로브";
		pItemData->eItemPart = ITEMPARTS::TOP;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.04f;           // 치명타 확률: 4%
		pItemData->fWeight = 1.2f;                  // 무게: 무거움
		pItemData->iDurability = 120;               // 내구도: 120
		pItemData->iRequiredLevel = 25;             // 요구 레벨: 25
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 120;                       // 체력: +120
		pItemData->iMP = 100;                       // 마나: +100
		pItemData->iIntelligence = 25;              // 지능: +25
		pItemData->iStrength = 15;                  // 힘: +15
		pItemData->iPhysicalAtk = 10;               // 물리 공격: +10
		pItemData->iMagicAtk = 15;                  // 마법 공격: +15
		pItemData->iFireResist = 12;                // 화염 저항: +12
		pItemData->iLightResist = 20;               // 명저항: +20
		pItemData->iManaResist = 25;                // 마나 저항: +25
		pItemData->iAgility = 6;                    // 이속: +6
		pItemData->iAttackSpeed = 4;                // 공격 속도: +4
		pItemData->iPhysicalDef = 40;               // 물리 방어: +40
		pItemData->iMagicDef = 35;                  // 마법 방어: +35
		pItemData->iWaterResist = 10;               // 수저항: +10
		pItemData->iDarkResist = 15;                // 암저항: +15

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"04.Neck2")
	{
		pItemData->strName = L"04.Neck2";
		pItemData->strExplan = L"고대의 힘이 깃들어 착용자의 체력과 방어력을 증가시키는 견갑입니다.";
		pItemData->ItemName = L"타이탄의 견갑";
		pItemData->eItemPart = ITEMPARTS::SHOULDER;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.03f;           // 치명타 확률: 3%
		pItemData->fWeight = 1.5f;                  // 무게: 무거움
		pItemData->iDurability = 110;               // 내구도: 110
		pItemData->iRequiredLevel = 18;             // 요구 레벨: 18
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 150;                       // 체력: +150
		pItemData->iMP = 30;                        // 마나: +30
		pItemData->iIntelligence = 10;              // 지능: +10
		pItemData->iStrength = 25;                  // 힘: +25
		pItemData->iPhysicalAtk = 15;               // 물리 공격: +15
		pItemData->iMagicAtk = 5;                   // 마법 공격: +5
		pItemData->iFireResist = 8;                 // 화염 저항: +8
		pItemData->iLightResist = 12;               // 명저항: +12
		pItemData->iManaResist = 10;                // 마나 저항: +10
		pItemData->iAgility = 5;                    // 이속: +5
		pItemData->iAttackSpeed = 3;                // 공격 속도: +3
		pItemData->iPhysicalDef = 35;               // 물리 방어: +35
		pItemData->iMagicDef = 20;                  // 마법 방어: +20
		pItemData->iWaterResist = 7;                // 수저항: +7
		pItemData->iDarkResist = 10;                // 암저항: +10

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"05.Neck3")
	{
		pItemData->strName = L"05.Neck3";
		pItemData->strExplan = L"마력을 증폭시켜 강력한 마법 효과를 부여하는 신비로운 견갑입니다.";
		pItemData->ItemName = L"엘드리치 숄더";
		pItemData->eItemPart = ITEMPARTS::SHOULDER;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.06f;           // 치명타 확률: 6%
		pItemData->fWeight = 1.2f;                  // 무게: 중간 정도
		pItemData->iDurability = 100;               // 내구도: 100
		pItemData->iRequiredLevel = 20;             // 요구 레벨: 20
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 80;                        // 체력: +80
		pItemData->iMP = 120;                       // 마나: +120
		pItemData->iIntelligence = 30;              // 지능: +30
		pItemData->iStrength = 10;                  // 힘: +10
		pItemData->iPhysicalAtk = 8;                // 물리 공격: +8
		pItemData->iMagicAtk = 35;                  // 마법 공격: +35
		pItemData->iFireResist = 10;                // 화염 저항: +10
		pItemData->iLightResist = 15;               // 명저항: +15
		pItemData->iManaResist = 20;                // 마나 저항: +20
		pItemData->iAgility = 6;                    // 이속: +6
		pItemData->iAttackSpeed = 4;                // 공격 속도: +4
		pItemData->iPhysicalDef = 20;               // 물리 방어: +20
		pItemData->iMagicDef = 30;                  // 마법 방어: +30
		pItemData->iWaterResist = 12;               // 수저항: +12
		pItemData->iDarkResist = 15;                // 암저항: +15

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"06.Pant2")
	{
		pItemData->strName = L"06.Pant2";
		pItemData->strExplan = L"화염 속성의 보호 효과를 제공하며 방어력을 강화하는 붉은 하의입니다.";
		pItemData->ItemName = L"인페르노 그리브";
		pItemData->eItemPart = ITEMPARTS::BOTTOM;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.05f;           // 치명타 확률: 5%
		pItemData->fWeight = 1.3f;                  // 무게: 중간 정도
		pItemData->iDurability = 110;               // 내구도: 110
		pItemData->iRequiredLevel = 20;             // 요구 레벨: 20
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 120;                       // 체력: +120
		pItemData->iMP = 40;                        // 마나: +40
		pItemData->iIntelligence = 15;              // 지능: +15
		pItemData->iStrength = 20;                  // 힘: +20
		pItemData->iPhysicalAtk = 10;               // 물리 공격: +10
		pItemData->iMagicAtk = 8;                   // 마법 공격: +8
		pItemData->iFireResist = 25;                // 화염 저항: +25
		pItemData->iLightResist = 10;               // 명저항: +10
		pItemData->iManaResist = 12;                // 마나 저항: +12
		pItemData->iAgility = 5;                    // 이속: +5
		pItemData->iAttackSpeed = 3;                // 공격 속도: +3
		pItemData->iPhysicalDef = 30;               // 물리 방어: +30
		pItemData->iMagicDef = 25;                  // 마법 방어: +25
		pItemData->iWaterResist = 8;                // 수저항: +8
		pItemData->iDarkResist = 10;                // 암저항: +10

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"07.Pant3")
	{
		pItemData->strName = L"07.Pant3";
		pItemData->strExplan = L"착용자의 이동 속도와 방어력을 크게 향상시키는 마법이 깃든 하의입니다.";
		pItemData->ItemName = L"스톰체이서 하의";
		pItemData->eItemPart = ITEMPARTS::BOTTOM;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.05f;           // 치명타 확률: 5%
		pItemData->fWeight = 1.1f;                  // 무게: 중간 정도
		pItemData->iDurability = 105;               // 내구도: 105
		pItemData->iRequiredLevel = 22;             // 요구 레벨: 22
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 100;                       // 체력: +100
		pItemData->iMP = 50;                        // 마나: +50
		pItemData->iIntelligence = 15;              // 지능: +15
		pItemData->iStrength = 18;                  // 힘: +18
		pItemData->iPhysicalAtk = 12;               // 물리 공격: +12
		pItemData->iMagicAtk = 10;                  // 마법 공격: +10
		pItemData->iFireResist = 10;                // 화염 저항: +10
		pItemData->iLightResist = 12;               // 명저항: +12
		pItemData->iManaResist = 15;                // 마나 저항: +15
		pItemData->iAgility = 20;                   // 이속: +20
		pItemData->iAttackSpeed = 5;                // 공격 속도: +5
		pItemData->iPhysicalDef = 28;               // 물리 방어: +28
		pItemData->iMagicDef = 25;                  // 마법 방어: +25
		pItemData->iWaterResist = 10;               // 수저항: +10
		pItemData->iDarkResist = 12;                // 암저항: +12

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"08.Shoes2")
	{
		pItemData->strName = L"08.Shoes2";
		pItemData->strExplan = L"불꽃의 힘이 담겨 착용자의 속도와 내구성을 증가시키는 붉은 신발입니다.";
		pItemData->ItemName = L"이그니스 부츠";
		pItemData->eItemPart = ITEMPARTS::SHOES;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.04f;           // 치명타 확률: 4%
		pItemData->fWeight = 0.8f;                  // 무게: 가벼움
		pItemData->iDurability = 120;               // 내구도: 120
		pItemData->iRequiredLevel = 18;             // 요구 레벨: 18
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 80;                        // 체력: +80
		pItemData->iMP = 40;                        // 마나: +40
		pItemData->iIntelligence = 12;              // 지능: +12
		pItemData->iStrength = 15;                  // 힘: +15
		pItemData->iPhysicalAtk = 10;               // 물리 공격: +10
		pItemData->iMagicAtk = 8;                   // 마법 공격: +8
		pItemData->iFireResist = 12;                // 화염 저항: +12
		pItemData->iLightResist = 10;               // 명저항: +10
		pItemData->iManaResist = 8;                 // 마나 저항: +8
		pItemData->iAgility = 25;                   // 이속: +25
		pItemData->iAttackSpeed = 5;                // 공격 속도: +5
		pItemData->iPhysicalDef = 20;               // 물리 방어: +20
		pItemData->iMagicDef = 15;                  // 마법 방어: +15
		pItemData->iWaterResist = 8;                // 수저항: +8
		pItemData->iDarkResist = 10;                // 암저항: +10

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"09.Shoes3")
	{
		pItemData->strName = L"09.Shoes3";
		pItemData->strExplan = L"마법의 힘이 깃들어 이동 속도와 회피 능력을 크게 향상시키는 신비로운 신발입니다.";
		pItemData->ItemName = L"제피로스 부츠";
		pItemData->eItemPart = ITEMPARTS::SHOES;
		pItemData->eElement = ELEMENT::NORMAL;

		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.05f;           // 치명타 확률: 5%
		pItemData->fWeight = 0.7f;                  // 무게: 매우 가벼움
		pItemData->iDurability = 100;               // 내구도: 100
		pItemData->iRequiredLevel = 20;             // 요구 레벨: 20
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 70;                        // 체력: +70
		pItemData->iMP = 60;                        // 마나: +60
		pItemData->iIntelligence = 18;              // 지능: +18
		pItemData->iStrength = 12;                  // 힘: +12
		pItemData->iPhysicalAtk = 10;               // 물리 공격: +10
		pItemData->iMagicAtk = 12;                  // 마법 공격: +12
		pItemData->iFireResist = 10;                // 화염 저항: +10
		pItemData->iLightResist = 15;               // 명저항: +15
		pItemData->iManaResist = 10;                // 마나 저항: +10
		pItemData->iAgility = 30;                   // 이속: +30
		pItemData->iAttackSpeed = 6;                // 공격 속도: +6
		pItemData->iPhysicalDef = 18;               // 물리 방어: +18
		pItemData->iMagicDef = 20;                  // 마법 방어: +20
		pItemData->iWaterResist = 12;               // 수저항: +12
		pItemData->iDarkResist = 15;                // 암저항: +15

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}

	//소비아이템
	else if (_CString == L"00.Berry")
	{
		pItemData->strName = L"00.Berry";
		pItemData->strExplan = L"뒷산에서 따온 신선한 산딸기이다.";
		pItemData->ItemName = L"신선한 루비베리";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.0f;            // 치명타 확률: 없음
		pItemData->fWeight = 0.1f;                  // 무게: 가벼움
		pItemData->iDurability = 0;                 // 내구도: 없음
		pItemData->iRequiredLevel = 0;              // 요구 레벨: 없음
		pItemData->bEnchantable = false;            // 강화 가능 여부: 불가

		// 스탯 및 능력치 초기화
		pItemData->iHP = 50;                        // 체력: +50 (소비 시 효과)
		pItemData->iMP = 0;                         // 마나: 없음
		pItemData->iIntelligence = 0;               // 지능: 없음
		pItemData->iStrength = 0;                   // 힘: 없음
		pItemData->iPhysicalAtk = 0;                // 물리 공격: 없음
		pItemData->iMagicAtk = 0;                   // 마법 공격: 없음
		pItemData->iFireResist = 0;                 // 화염 저항: 없음
		pItemData->iLightResist = 0;                // 명저항: 없음
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 0;                    // 이속: 없음
		pItemData->iAttackSpeed = 0;                // 공격 속도: 없음
		pItemData->iPhysicalDef = 0;                // 물리 방어: 없음
		pItemData->iMagicDef = 0;                   // 마법 방어: 없음
		pItemData->iWaterResist = 0;                // 수저항: 없음
		pItemData->iDarkResist = 0;                 // 암저항: 없음

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"01.Bread")
	{
		pItemData->strName = L"01.Bread";
		pItemData->strExplan = L"칸나가 만든 신선한 빵이다.";
		pItemData->ItemName = L"칸나의 수제 빵";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.0f;            // 치명타 확률: 없음
		pItemData->fWeight = 0.2f;                  // 무게: 가벼움
		pItemData->iDurability = 0;                 // 내구도: 없음
		pItemData->iRequiredLevel = 0;              // 요구 레벨: 없음
		pItemData->bEnchantable = false;            // 강화 가능 여부: 불가

		// 스탯 및 능력치 초기화
		pItemData->iHP = 80;                        // 체력: +80 (소비 시 효과)
		pItemData->iMP = 0;                         // 마나: 없음
		pItemData->iIntelligence = 0;               // 지능: 없음
		pItemData->iStrength = 0;                   // 힘: 없음
		pItemData->iPhysicalAtk = 0;                // 물리 공격: 없음
		pItemData->iMagicAtk = 0;                   // 마법 공격: 없음
		pItemData->iFireResist = 0;                 // 화염 저항: 없음
		pItemData->iLightResist = 0;                // 명저항: 없음
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 0;                    // 이속: 없음
		pItemData->iAttackSpeed = 0;                // 공격 속도: 없음
		pItemData->iPhysicalDef = 0;                // 물리 방어: 없음
		pItemData->iMagicDef = 0;                   // 마법 방어: 없음
		pItemData->iWaterResist = 0;                // 수저항: 없음
		pItemData->iDarkResist = 0;                 // 암저항: 없음

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"02.Potion")
	{
		pItemData->strName = L"02.Potion";
		pItemData->strExplan = L"잡화상점에서 구할 수 있는 흔한 포션이다.";
		pItemData->ItemName = L"레드 포션";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.0f;            // 치명타 확률: 없음
		pItemData->fWeight = 0.2f;                  // 무게: 가벼움
		pItemData->iDurability = 0;                 // 내구도: 없음
		pItemData->iRequiredLevel = 0;              // 요구 레벨: 없음
		pItemData->bEnchantable = false;            // 강화 가능 여부: 불가

		// 스탯 및 능력치 초기화
		pItemData->iHP = 100;                        // 체력: +100 (소비 시 효과)
		pItemData->iMP = 0;                         // 마나: 없음
		pItemData->iIntelligence = 0;               // 지능: 없음
		pItemData->iStrength = 0;                   // 힘: 없음
		pItemData->iPhysicalAtk = 0;                // 물리 공격: 없음
		pItemData->iMagicAtk = 0;                   // 마법 공격: 없음
		pItemData->iFireResist = 0;                 // 화염 저항: 없음
		pItemData->iLightResist = 0;                // 명저항: 없음
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 0;                    // 이속: 없음
		pItemData->iAttackSpeed = 0;                // 공격 속도: 없음
		pItemData->iPhysicalDef = 0;                // 물리 방어: 없음
		pItemData->iMagicDef = 0;                   // 마법 방어: 없음
		pItemData->iWaterResist = 0;                // 수저항: 없음
		pItemData->iDarkResist = 0;                 // 암저항: 없음

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"03.Meat")
	{
		pItemData->strName = L"03.Meat";
		pItemData->strExplan = L"던전에서 잡은 멧돼지의 뒷다리고기이다.";
		pItemData->ItemName = L"멧크로우 고기";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.0f;            // 치명타 확률: 없음
		pItemData->fWeight = 0.2f;                  // 무게: 가벼움
		pItemData->iDurability = 0;                 // 내구도: 없음
		pItemData->iRequiredLevel = 0;              // 요구 레벨: 없음
		pItemData->bEnchantable = false;            // 강화 가능 여부: 불가

		// 스탯 및 능력치 초기화
		pItemData->iHP = 150;                        // 체력: +150 (소비 시 효과)
		pItemData->iMP = 0;                         // 마나: 없음
		pItemData->iIntelligence = 0;               // 지능: 없음
		pItemData->iStrength = 0;                   // 힘: 없음
		pItemData->iPhysicalAtk = 0;                // 물리 공격: 없음
		pItemData->iMagicAtk = 0;                   // 마법 공격: 없음
		pItemData->iFireResist = 0;                 // 화염 저항: 없음
		pItemData->iLightResist = 0;                // 명저항: 없음
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 0;                    // 이속: 없음
		pItemData->iAttackSpeed = 0;                // 공격 속도: 없음
		pItemData->iPhysicalDef = 0;                // 물리 방어: 없음
		pItemData->iMagicDef = 0;                   // 마법 방어: 없음
		pItemData->iWaterResist = 0;                // 수저항: 없음
		pItemData->iDarkResist = 0;                 // 암저항: 없음

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"04.Flower")
	{
		pItemData->strName = L"04.Flower";
		pItemData->strExplan = L"마나의 기운을 담고있는 약초이다.";
		pItemData->ItemName = L"에테르 리프";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.0f;            // 치명타 확률: 없음
		pItemData->fWeight = 0.2f;                  // 무게: 가벼움
		pItemData->iDurability = 0;                 // 내구도: 없음
		pItemData->iRequiredLevel = 0;              // 요구 레벨: 없음
		pItemData->bEnchantable = false;            // 강화 가능 여부: 불가

		// 스탯 및 능력치 초기화
		pItemData->iHP = 0;                         // 체력: 없음
		pItemData->iMP = 150;                       // 마나: +150
		pItemData->iIntelligence = 0;               // 지능: 없음
		pItemData->iStrength = 0;                   // 힘: 없음
		pItemData->iPhysicalAtk = 0;                // 물리 공격: 없음
		pItemData->iMagicAtk = 0;                   // 마법 공격: 없음
		pItemData->iFireResist = 0;                 // 화염 저항: 없음
		pItemData->iLightResist = 0;                // 명저항: 없음
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 0;                    // 이속: 없음
		pItemData->iAttackSpeed = 0;                // 공격 속도: 없음
		pItemData->iPhysicalDef = 0;                // 물리 방어: 없음
		pItemData->iMagicDef = 0;                   // 마법 방어: 없음
		pItemData->iWaterResist = 0;                // 수저항: 없음
		pItemData->iDarkResist = 0;                 // 암저항: 없음

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}

	//무기
	else if (_CString == L"Weapon01")
	{
		pItemData->strName = L"Weapon01";
		pItemData->strExplan = L"불꽃의 기운이 깃든 강력한 리볼버로, 적에게 화염 피해를 입힙니다.";
		pItemData->ItemName = L"화염의 서약";
		pItemData->eItemPart = ITEMPARTS::WEAPON;
		pItemData->eElement = ELEMENT::FIRE;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.15f;           // 치명타 확률: 15%
		pItemData->fWeight = 2.5f;                  // 무게: 무거움
		pItemData->iDurability = 150;               // 내구도: 150
		pItemData->iRequiredLevel = 25;             // 요구 레벨: 25
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 0;                         // 체력: 없음
		pItemData->iMP = 0;                         // 마나: 없음
		pItemData->iIntelligence = 5;               // 지능: +5 (약간의 마법 효과)
		pItemData->iStrength = 30;                  // 힘: +30 (물리 공격력 보조)
		pItemData->iPhysicalAtk = 50;               // 물리 공격: +50
		pItemData->iMagicAtk = 20;                  // 마법 공격: +20 (화염 속성 효과)
		pItemData->iFireResist = 10;                // 화염 저항: +10
		pItemData->iLightResist = 0;                // 명저항: 없음
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 10;                   // 이속: +10
		pItemData->iAttackSpeed = 8;                // 공격 속도: +8
		pItemData->iPhysicalDef = 5;                // 물리 방어: +5 (약간의 방어력 추가)
		pItemData->iMagicDef = 0;                   // 마법 방어: 없음
		pItemData->iWaterResist = 0;                // 수저항: 없음
		pItemData->iDarkResist = 0;                 // 암저항: 없음

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"Weapon02")
	{
		pItemData->strName = L"Weapon02";
		pItemData->strExplan = L"전기를 품은 리볼버로, 적에게 번개 피해를 가하는 강력한 무기입니다.";
		pItemData->ItemName = L"라이트닝 리볼버";
		pItemData->eItemPart = ITEMPARTS::WEAPON;
		pItemData->eElement = ELEMENT::LIGHT;
		
		// 아이템 특성 초기화
		pItemData->fCriticalRate = 0.2f;            // 치명타 확률: 20%
		pItemData->fWeight = 3.0f;                  // 무게: 무겁게 설정
		pItemData->iDurability = 160;               // 내구도: 160
		pItemData->iRequiredLevel = 30;             // 요구 레벨: 30
		pItemData->bEnchantable = true;             // 강화 가능 여부: 가능

		// 스탯 및 능력치 초기화
		pItemData->iHP = 0;                         // 체력: 없음
		pItemData->iMP = 0;                         // 마나: 없음
		pItemData->iIntelligence = 10;              // 지능: +10 (번개 속성 관련 효과)
		pItemData->iStrength = 35;                  // 힘: +35
		pItemData->iPhysicalAtk = 55;               // 물리 공격: +55
		pItemData->iMagicAtk = 25;                  // 마법 공격: +25 (번개 속성 효과)
		pItemData->iFireResist = 5;                 // 화염 저항: +5
		pItemData->iLightResist = 15;               // 번개 저항: +15
		pItemData->iManaResist = 0;                 // 마나 저항: 없음
		pItemData->iAgility = 8;                    // 이속: +8
		pItemData->iAttackSpeed = 10;               // 공격 속도: +10
		pItemData->iPhysicalDef = 10;               // 물리 방어: +10
		pItemData->iMagicDef = 5;                   // 마법 방어: +5
		pItemData->iWaterResist = 5;                // 수저항: +5
		pItemData->iDarkResist = 5;                 // 암저항: +5

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else{return;}
}





//저장 불러오기
void CEquipTool::OnEquipSave()
{
	CFileDialog		Dlg(FALSE,					// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat",									// default 확장자명
		L"*.dat",								// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
		L"Data Files(*.dat) | *.dat ||",		// 대화 상자에 표시될 파일 형식
		this);									// 부모 윈도우 주소


#pragma region 기본경로 설정코드

	TCHAR	szPath[MAX_PATH] = L"";
	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라주는 함수
	// 경로 상에 파일 이름이 없을 경우엔 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

#pragma endregion

	if (IDOK == Dlg.DoModal())
	{
#pragma region 스트림 개방

		// GetPathName : (다른이름으로 저장) 선택된 경로를 반환
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		// 스트림 개방
		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

#pragma endregion

		//맵에 키랑 클래스만 저장하면 된다.
		DWORD	dwByte(0), dwStrByte(0), dwClassStrByte(0);

		auto& UI = Inventory->GetChildUI();
		for (auto& BtnUI : UI)
		{
			if (BtnUI->GetChildUI().size() > 0)
			{
				auto& pItem = BtnUI->GetChildUI().front();
				dwStrByte = sizeof(TCHAR) * (pItem->GetName().length() + 1);

				WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, pItem->GetName().c_str(), dwStrByte, &dwByte, nullptr);

				// 2. 부모 클래스 정보 저장
				wstring parentClass = dynamic_cast<DH_UI*>(pItem)->GetParent()->GetName();
				DWORD dwClassStrByte = (parentClass.length() + 1) * sizeof(wchar_t);
				WriteFile(hFile, &dwClassStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, parentClass.c_str(), dwClassStrByte * sizeof(TCHAR), &dwByte, nullptr);
			}
		}

		auto& UI2 = Interface->GetChildUI();
		for (auto& BtnUI : UI2)
		{
			if (BtnUI->GetChildUI().size() > 0 && dynamic_cast<DH_BtnUI*>(BtnUI)->GeteItemParts() == ITEMPARTS::POTION)
			{
				auto& pItem = BtnUI->GetChildUI().front();
				dwStrByte = sizeof(TCHAR) * (pItem->GetName().length() + 1);

				WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, pItem->GetName().c_str(), dwStrByte, &dwByte, nullptr);

				// 2. 부모 클래스 정보 저장
				wstring parentClass = dynamic_cast<DH_UI*>(pItem)->GetParent()->GetName();
				DWORD dwClassStrByte = (parentClass.length() + 1) * sizeof(wchar_t);
				WriteFile(hFile, &dwClassStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, parentClass.c_str(), dwClassStrByte * sizeof(TCHAR), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}


}
void CEquipTool::OnEquipLoad()
{
	CFileDialog		Dlg(TRUE,		// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat",		// default 확장자명
		L"*.dat",	// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
		L"Data Files(*.dat) | *.dat ||", // 대화 상자에 표시될 파일 형식
		this);	// 부모 윈도우 주소


#pragma region 기본경로 설정코드

	TCHAR	szPath[MAX_PATH] = L"";
	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라주는 함수
	// 경로 상에 파일 이름이 없을 경우엔 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

#pragma endregion

	if (IDOK == Dlg.DoModal())
	{
#pragma region 스트림 개방


		// GetPathName : (다른이름으로 저장) 선택된 경로를 반환
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		// 스트림 개방
		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

#pragma endregion

#pragma region 아이템 초기화

		//기존 정보 초기화
		for (auto& MyPair : m_mapItemData)
			delete MyPair.second;
		auto& UI = Inventory->GetChildUI();
		for (auto& BtnUI : UI)
		{
			if (BtnUI->GetChildUI().size() > 0)
			{
				delete BtnUI->GetChildUI().front();
				BtnUI->GetChildUI().front() = nullptr;
				BtnUI->GetChildUI().clear();
			}
		}
		auto& UI2 = Interface->GetChildUI();
		for (auto& BtnUI : UI2)
		{
			if (BtnUI->GetChildUI().size() > 0)
			{
				delete BtnUI->GetChildUI().front();
				BtnUI->GetChildUI().front() = nullptr;
				BtnUI->GetChildUI().clear();
			}
		}
		m_mapItemData.clear();
		m_IvenAdd.ResetContent();

#pragma endregion		

		// 파일 읽기
		DWORD	dwByte(0), dwStrByte(0), dwClassStrByte(0);
		while (true)
		{
			//문자 열 읽기
			bool Read;
			Read = ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pName = new TCHAR[dwStrByte];
			Read = ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			//끝까지 읽으면 리턴
			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			//이름 추가
			ITEMDATA* pItemData = new ITEMDATA;
			pItemData->strName = pName;
			delete[]pName;

			//부모 읽기
			bool Class;
			Class = ReadFile(hFile, &dwClassStrByte, sizeof(DWORD), &dwByte, nullptr);
			TCHAR* pParentClass = new TCHAR[dwClassStrByte];
			Class = ReadFile(hFile, pParentClass, dwClassStrByte * sizeof(TCHAR), &dwByte, nullptr);

			SetItemInfo(pItemData->strName, pItemData);
			m_IvenAdd.AddString(pItemData->strName);

			for (auto Btn : Inventory->GetChildUI())
			{
				if (Btn->GetName() == pParentClass)
					AddItem(pItemData->strName.GetString(), pItemData->strName, pItemData, Btn);
			}
			for (auto Btn : Interface->GetChildUI())
			{
				if (Btn->GetName() == pParentClass)
					AddItem(pItemData->strName.GetString(), pItemData->strName, pItemData, Btn);
			}

			delete[] pParentClass;
		}
		CloseHandle(hFile);
	}

	DH_Player::Get_Instance()->SetItemUpdate(true);
}
void CEquipTool::OnSkillSave()
{
	CFileDialog		Dlg(FALSE,					// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat",									// default 확장자명
		L"*.dat",								// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
		L"Data Files(*.dat) | *.dat ||",		// 대화 상자에 표시될 파일 형식
		this);									// 부모 윈도우 주소


#pragma region 기본경로 설정코드

	TCHAR	szPath[MAX_PATH] = L"";
	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라주는 함수
	// 경로 상에 파일 이름이 없을 경우엔 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

#pragma endregion

	if (IDOK == Dlg.DoModal())
	{
#pragma region 스트림 개방

		// GetPathName : (다른이름으로 저장) 선택된 경로를 반환
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		// 스트림 개방
		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

#pragma endregion

		//맵에 키랑 클래스만 저장하면 된다.
		DWORD	dwByte(0), dwStrByte(0), dwClassStrByte(0);

		auto& UI = Skill->GetChildUI();
		for (auto& BtnUI : UI)
		{
			if (BtnUI->GetChildUI().size() > 0)
			{
				auto& pItem = BtnUI->GetChildUI().front();
				dwStrByte = sizeof(TCHAR) * (pItem->GetName().length() + 1);

				WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, pItem->GetName().c_str(), dwStrByte, &dwByte, nullptr);

				// 2. 부모 클래스 정보 저장
				wstring parentClass = dynamic_cast<DH_UI*>(pItem)->GetParent()->GetName();
				DWORD dwClassStrByte = (parentClass.length() + 1) * sizeof(wchar_t);
				WriteFile(hFile, &dwClassStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, parentClass.c_str(), dwClassStrByte * sizeof(TCHAR), &dwByte, nullptr);
			}
		}

		auto& UI2 = Interface->GetChildUI();
		for (auto& BtnUI : UI2)
		{
			if (BtnUI->GetChildUI().size() > 0 && dynamic_cast<DH_BtnUI*>(BtnUI)->GeteItemParts() == ITEMPARTS::SKILL)
			{
				auto& pItem = BtnUI->GetChildUI().front();
				dwStrByte = sizeof(TCHAR) * (pItem->GetName().length() + 1);

				WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, pItem->GetName().c_str(), dwStrByte, &dwByte, nullptr);

				// 2. 부모 클래스 정보 저장
				wstring parentClass = dynamic_cast<DH_UI*>(pItem)->GetParent()->GetName();
				DWORD dwClassStrByte = (parentClass.length() + 1) * sizeof(wchar_t);
				WriteFile(hFile, &dwClassStrByte, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, parentClass.c_str(), dwClassStrByte * sizeof(TCHAR), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}


}
void CEquipTool::OnSkillLoad()
{
	CFileDialog		Dlg(TRUE,		// TRUE(불러오기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat",		// default 확장자명
		L"*.dat",	// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,	// 읽기 전용 체크 박스 숨김 | 중복된 이름으로 파일 저장 시 경고 메세지 띄움
		L"Data Files(*.dat) | *.dat ||", // 대화 상자에 표시될 파일 형식
		this);	// 부모 윈도우 주소


#pragma region 기본경로 설정코드

	TCHAR	szPath[MAX_PATH] = L"";
	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// PathRemoveFileSpec : 전체 경로에서 파일 이름만 잘라주는 함수
	// 경로 상에 파일 이름이 없을 경우엔 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

#pragma endregion

	if (IDOK == Dlg.DoModal())
	{
#pragma region 스트림 개방


		// GetPathName : (다른이름으로 저장) 선택된 경로를 반환
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		// 스트림 개방
		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

#pragma endregion

#pragma region 아이템 초기화

		//기존 정보 초기화
		for (auto& MyPair : m_mapSkillData)
			delete MyPair.second;
		auto& UI = Skill->GetChildUI();
		for (auto& BtnUI : UI)
		{
			if (BtnUI->GetChildUI().size() > 0)
			{
				delete BtnUI->GetChildUI().front();
				BtnUI->GetChildUI().front() = nullptr;
				BtnUI->GetChildUI().clear();
			}
		}
		auto& UI2 = Interface->GetChildUI();
		for (auto& BtnUI : UI2)
		{
			if (BtnUI->GetChildUI().size() > 0)
			{
				delete BtnUI->GetChildUI().front();
				BtnUI->GetChildUI().front() = nullptr;
				BtnUI->GetChildUI().clear();
			}
		}
		m_mapSkillData.clear();
		m_SkillAdd.ResetContent();

#pragma endregion		

		// 파일 읽기
		DWORD	dwByte(0), dwStrByte(0), dwClassStrByte(0);
		while (true)
		{
			//문자 열 읽기
			bool Read;
			Read = ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pName = new TCHAR[dwStrByte];
			Read = ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			//끝까지 읽으면 리턴
			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			//이름 추가
			SKILLDATA* pISkillData = new SKILLDATA;
			TCHAR	szFileName[MAX_STR] = L"";
			lstrcpy(szFileName, pName);
			PathRemoveExtension(szFileName);
			pISkillData->strName = szFileName;
			delete[]pName;


			//부모 읽기
			bool Class;
			Class = ReadFile(hFile, &dwClassStrByte, sizeof(DWORD), &dwByte, nullptr);
			TCHAR* pParentClass = new TCHAR[dwClassStrByte];
			Class = ReadFile(hFile, pParentClass, dwClassStrByte * sizeof(TCHAR), &dwByte, nullptr);

			m_SkillAdd.AddString(pISkillData->strName);

			for (auto Btn : Skill->GetChildUI())
			{
				if (Btn->GetName() == pParentClass)
				{
					AddSkill(pISkillData->strName.GetString(), pISkillData->strName, pISkillData, Btn);
					m_mapSkillData.insert(make_pair(pISkillData->strName, pISkillData));
				}

			}
			for (auto Btn : Interface->GetChildUI())
			{
				if (Btn->GetName() == pParentClass)
				{
					AddSkill(pISkillData->strName.GetString(), pISkillData->strName, pISkillData, Btn);
					m_mapSkillData.insert(make_pair(pISkillData->strName, pISkillData));
				}
			}

			delete[] pParentClass;
		}
		CloseHandle(hFile);
	}

	DH_Player::Get_Instance()->SetItemUpdate(true);
}

void CEquipTool::OnItemSave()
{
	OnEquipSave();
}
void CEquipTool::OnItemLoad()
{
	OnEquipLoad();
}

void CEquipTool::OnSkillSaveBtn()
{
	OnSkillSave();
}
void CEquipTool::OnSkillLoadBtn()
{
	OnSkillLoad();
}
