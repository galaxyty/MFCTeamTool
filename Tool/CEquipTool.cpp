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
#include "DH_Skill.h"
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

	ITEMDATA* pItemData = new ITEMDATA;
	SetItemInfo(strFindName, pItemData);
	// AddString : 리스트 박스에 문자열을 삽입
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
	AddSkill(strFindName.GetString(), strFindName);

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

}



//아이템 추가, 삭제 로직
void CEquipTool::AddItem(wstring _Item, CString _CString, ITEMDATA* pItemData)
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
			Item->Initialize();
			Inventory->GetChildUI()[i]->AddParent(Item);
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
void CEquipTool::AddSkill(wstring _Item, CString _CString)
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
			SkillIcon->Initialize();
			Skill->GetChildUI()[i]->AddParent(SkillIcon);
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

void CEquipTool::SetItemInfo(CString _CString, ITEMDATA* pItemData)
{
	//악세서리
	if (_CString == L"00.Band")
	{
		pItemData->strName = L"00.Band";
		pItemData->strSpecialEffect = L"손가락에 착용 시 방어력을 약간 증가시키는 단순한 금속 밴드입니다.";
		pItemData->eItemPart = ITEMPARTS::BRACELET;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 10.0f;
		pItemData->fAttackSpeed = 1.0f;
		pItemData->fWeight = 5.0f;
		pItemData->iHp = 50;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 15;
		pItemData->iDefense = 30;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Heart")
	{
		pItemData->strName = L"00.Heart";
		pItemData->strSpecialEffect = L"사용 시 랜덤한 효과를 발휘하는 신비로운 하트 형태의 아이템입니다.";
		pItemData->eItemPart = ITEMPARTS::JEWERLY;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 15.0f;
		pItemData->fAttackSpeed = 5.0f;
		pItemData->fWeight = 10.0f;
		pItemData->iHp = 300;
		pItemData->iAttack = 30;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 0;
		pItemData->iDefense = 50;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Neck")
	{
		pItemData->strName = L"00.Neck";
		pItemData->strSpecialEffect = L"착용 시 마법 방어력을 강화하는 신비로운 빛을 내뿜는 목걸이입니다.";
		pItemData->eItemPart = ITEMPARTS::NECK;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 25.f;
		pItemData->fAttackSpeed = 0.0f;
		pItemData->fWeight = 15.0f;
		pItemData->iHp = 200;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 20;
		pItemData->iDefense = 25;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Ring1")
	{
		pItemData->strName = L"00.Ring1";
		pItemData->strSpecialEffect = L"마법의 힘을 담고 있어 착용자의 능력을 소폭 강화시키는 반지입니다.";
		pItemData->eItemPart = ITEMPARTS::RING;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 10.f;
		pItemData->fAttackSpeed = 0.0f;
		pItemData->fWeight = 25.0f;
		pItemData->iHp = 150;
		pItemData->iAttack = 15;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 10;
		pItemData->iDefense = 15;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Ring2")
	{
		pItemData->strName = L"00.Ring2";
		pItemData->strSpecialEffect = L"착용자에게 공격력을 증폭시키는 어둠의 기운이 깃든 반지입니다.";
		pItemData->eItemPart = ITEMPARTS::RING;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 30.f;
		pItemData->fAttackSpeed = 5.0f;
		pItemData->fWeight = 25.0f;
		pItemData->iHp = 50;
		pItemData->iAttack = 40;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 30;
		pItemData->iDefense = 5;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"00.Ring3")
	{
		pItemData->strName = L"00.Ring3";
		pItemData->strSpecialEffect = L"착용 시 마력을 증대시키며 푸른 에너지를 발산하는 신비로운 목걸이입니다.";
		pItemData->eItemPart = ITEMPARTS::NECK;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 5.f;
		pItemData->fAttackSpeed = 0.0f;
		pItemData->fWeight = 10.0f;
		pItemData->iHp = 100;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 25;
		pItemData->iDefense = 100;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}

	//방어구
	else if (_CString == L"00.Muffler")
	{
		pItemData->strName = L"00.Muffler";
		pItemData->strSpecialEffect = L"착용 시 따뜻함과 함께 방어력을 소폭 증가시키는 황금빛 머플러입니다.";
		pItemData->eItemPart = ITEMPARTS::NECK;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.0f;
		pItemData->fWeight = 5.0f;
		pItemData->iHp = 150;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 5;
		pItemData->iDefense = 50;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"01.Belt2")
	{
		pItemData->strName = L"01.Belt2";
		pItemData->strSpecialEffect = L"공격 속도를 증가시키며 전투 능력을 향상시키는 강철 벨트입니다.";
		pItemData->eItemPart = ITEMPARTS::BELT;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 5.f;
		pItemData->fAttackSpeed = 15.0f;
		pItemData->fWeight = 10.0f;
		pItemData->iHp = 150;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 75;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"01.Belt3")
	{
		pItemData->strName = L"01.Belt3";
		pItemData->strSpecialEffect = L"마법 에너지가 깃들어 착용자의 마력과 방어력을 증대시키는 신비로운 벨트입니다.";
		pItemData->eItemPart = ITEMPARTS::BELT;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.0f;
		pItemData->fWeight = 15.0f;
		pItemData->iHp = 300;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 150;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"02.Coat2")
	{
		pItemData->strName = L"02.Coat2";
		pItemData->strSpecialEffect = L"불꽃의 힘을 품고 있어 방어력과 화염 저항을 강화하는 붉은 코트입니다.";
		pItemData->eItemPart = ITEMPARTS::TOP;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 10.f;
		pItemData->fAttackSpeed = 5.f;
		pItemData->fWeight = 20.0f;
		pItemData->iHp = 200;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 200;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"03.Coat3")
	{
		pItemData->strName = L"03.Coat3";
		pItemData->strSpecialEffect = L"마법 보호막을 생성하여 방어력과 마법 저항을 크게 증가시키는 고급 코트입니다.";
		pItemData->eItemPart = ITEMPARTS::TOP;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 5.f;
		pItemData->fAttackSpeed = 5.f;
		pItemData->fWeight = 30.0f;
		pItemData->iHp = 500;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 150;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"04.Neck2")
	{
		pItemData->strName = L"04.Neck2";
		pItemData->strSpecialEffect = L"고대의 힘이 깃들어 착용자의 체력과 방어력을 증가시키는 견갑입니다.";
		pItemData->eItemPart = ITEMPARTS::SHOULDER;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 15.f;
		pItemData->fAttackSpeed = 5.f;
		pItemData->fWeight = 20.0f;
		pItemData->iHp = 200;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 100;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"05.Neck3")
	{
		pItemData->strName = L"05.Neck3";
		pItemData->strSpecialEffect = L"마력을 증폭시켜 강력한 마법 효과를 부여하는 신비로운 견갑입니다.";
		pItemData->eItemPart = ITEMPARTS::SHOULDER;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 25.f;
		pItemData->fAttackSpeed = 5.f;
		pItemData->fWeight = 10.0f;
		pItemData->iHp = 150;
		pItemData->iAttack = 00;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 50;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"06.Pant2")
	{
		pItemData->strName = L"06.Pant2";
		pItemData->strSpecialEffect = L"화염 속성의 보호 효과를 제공하며 방어력을 강화하는 붉은 하의입니다.";
		pItemData->eItemPart = ITEMPARTS::BOTTOM;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 5.f;
		pItemData->fAttackSpeed = 10.f;
		pItemData->fWeight = 30.0f;
		pItemData->iHp = 400;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 200;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"07.Pant3")
	{
		pItemData->strName = L"07.Pant3";
		pItemData->strSpecialEffect = L"착용자의 이동 속도와 방어력을 크게 향상시키는 마법이 깃든 하의입니다.";
		pItemData->eItemPart = ITEMPARTS::BOTTOM;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 30.f;
		pItemData->fWeight = 30.0f;
		pItemData->iHp = 350;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 100;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"08.Shoes2")
	{
		pItemData->strName = L"08.Shoes2";
		pItemData->strSpecialEffect = L"불꽃의 힘이 담겨 착용자의 속도와 내구성을 증가시키는 붉은 신발입니다.";
		pItemData->eItemPart = ITEMPARTS::SHOES;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 10.f;
		pItemData->fAttackSpeed = 10.f;
		pItemData->fWeight = 15.0f;
		pItemData->iHp = 150;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 150;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"09.Shoes3")
	{
		pItemData->strName = L"09.Shoes3";
		pItemData->strSpecialEffect = L"마법의 힘이 깃들어 이동 속도와 회피 능력을 크게 향상시키는 신비로운 신발입니다.";
		pItemData->eItemPart = ITEMPARTS::SHOES;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 15.f;
		pItemData->fAttackSpeed = 5.f;
		pItemData->fWeight = 15.0f;
		pItemData->iHp = 200;
		pItemData->iAttack = 10;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 50;
		pItemData->iDefense = 100;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}

	//소비아이템
	else if (_CString == L"00.Berry")
	{
		pItemData->strName = L"00.Berry";
		pItemData->strSpecialEffect = L"뒷산에서 따온 신선한 산딸기이다.";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.f;
		pItemData->fWeight = 1.0f;
		pItemData->iHp = 100;
		pItemData->iAttack = 0;
		pItemData->iDurability = 0;
		pItemData->iRequiredLevel = 0;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"01.Bread")
	{
		pItemData->strName = L"01.Bread";
		pItemData->strSpecialEffect = L"칸나가 만든 신선한 빵이다.";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.f;
		pItemData->fWeight = 2.0f;
		pItemData->iHp = 150;
		pItemData->iAttack = 0;
		pItemData->iDurability = 0;
		pItemData->iRequiredLevel = 0;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"02.Potion")
	{
		pItemData->strName = L"02.Potion";
		pItemData->strSpecialEffect = L"잡화상점에서 구할 수 있는 흔한 포션이다.";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.f;
		pItemData->fWeight = 3.0f;
		pItemData->iHp = 200;
		pItemData->iAttack = 0;
		pItemData->iDurability = 0;
		pItemData->iRequiredLevel = 0;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"03.Meat")
	{
		pItemData->strName = L"03.Meat";
		pItemData->strSpecialEffect = L"던전에서 잡은 멧돼지의 뒷다리고기이다.";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.f;
		pItemData->fWeight = 5.0f;
		pItemData->iHp = 300;
		pItemData->iAttack = 0;
		pItemData->iDurability = 0;
		pItemData->iRequiredLevel = 0;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
		}
	else if (_CString == L"04.Flower")
	{
		pItemData->strName = L"04.Flower";
		pItemData->strSpecialEffect = L"마나의 기운을 담고있는 약초이다.";
		pItemData->eItemPart = ITEMPARTS::POTION;
		pItemData->eElement = ELEMENT::NORMAL;
		pItemData->fCriticalRate = 0.f;
		pItemData->fAttackSpeed = 0.f;
		pItemData->fWeight = 2.0f;
		pItemData->iHp = 100;
		pItemData->iAttack = 0;
		pItemData->iDurability = 0;
		pItemData->iRequiredLevel = 0;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}

	//무기
	else if (_CString == L"Weapon01")
	{
		pItemData->strName = L"Weapon01";
		pItemData->strSpecialEffect = L"불꽃의 기운이 깃든 강력한 검으로, 적에게 화염 피해를 입힙니다.";
		pItemData->eItemPart = ITEMPARTS::WEAPON;
		pItemData->eElement = ELEMENT::FIRE;
		pItemData->fCriticalRate = 30.f;
		pItemData->fAttackSpeed = 30.f;
		pItemData->fWeight = 50.0f;
		pItemData->iHp = 0;
		pItemData->iAttack = 150;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 60;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else if (_CString == L"Weapon02")
	{
		pItemData->strName = L"Weapon02";
		pItemData->strSpecialEffect = L"전기를 품은 총기로, 적에게 번개 피해를 가하는 강력한 무기입니다.";
		pItemData->eItemPart = ITEMPARTS::WEAPON;
		pItemData->eElement = ELEMENT::LIGHT;
		pItemData->fCriticalRate = 20.f;
		pItemData->fAttackSpeed = 25.f;
		pItemData->fWeight = 60.0f;
		pItemData->iHp = 0;
		pItemData->iAttack = 200;
		pItemData->iDurability = 100;
		pItemData->iRequiredLevel = 60;
		pItemData->iDefense = 0;
		pItemData->bEnchantable = true;

		m_mapItemData.insert(make_pair(pItemData->strName, pItemData));
	}
	else{return;}
}


