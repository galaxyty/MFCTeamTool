﻿// CMapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CMapTool.h"
#include "CDevice.h"
#include "CTerrain.h"
#include "MainFrm.h"
#include "CTextureMgr.h"
#include "CMapManager.h"


// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent), m_objectKey(nullptr)
{	
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RATIO_SLIDER, m_RatioSlider);
	DDX_Control(pDX, IDC_RATIO_TEXT, m_RatioText);
	DDX_Control(pDX, IDC_LIST_BOX_MAP, m_ListBoxMap);
	DDX_Control(pDX, IDC_BACKGROUND_PICTURE, m_BGPicture);
	DDX_Control(pDX, IDC_LIST_BOX_OBJECT, m_ListBoxObject);
	DDX_Control(pDX, IDC_OBJECT_PICTURE, m_ObjectPicture);
	DDX_Control(pDX, IDC_MAP_COMBO, m_RoomComboBox);
	DDX_Control(pDX, IDC_LIST_BOX_OBJECT_LIST, m_ListBoxObjectList);
	DDX_Control(pDX, IDC_OBJECT_NAME_EDIT, m_NameEdit);
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);	

	// 드래그해서 가져온 파일 갯수.
	UINT dragCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	// 파일 경로.
	TCHAR* szFilePath = new TCHAR[MAX_PATH];

	// 파일 이름.
	TCHAR* szFileName = new TCHAR[MAX_PATH];

	// 마우스 좌표.
	POINT mousePosition;

	// 마우스 스크린 좌표를 가져옴
	GetCursorPos(&mousePosition);

	// 마우스 스크린 좌표를 클라이언트 좌표로 변환.
	ScreenToClient(&mousePosition);

	// 맵 리스트 박스 렉트.
	CRect mapRect;

	// m_ListBoxMap 윈도우 창 기준 렉트 좌표 반환.
	m_ListBoxMap.GetWindowRect(mapRect);

	// 클라이언트 좌표로 변환.
	ScreenToClient(mapRect);

	// 오브젝트 리스트 박스 크기.
	CRect objectRect;
	m_ListBoxObject.GetWindowRect(objectRect);
	ScreenToClient(objectRect);

	for (int i = 0; i < dragCount; i++)
	{		
		// 드래그 정보에서 파일 절대 경로 추출.
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		// 파일 절대 경로를 상대 경로로 변환.
		CString strRelative = ConvertRelativePath(szFilePath);

		// PathFindFileName : 경로 중 파일 이름만 추출.
		lstrcpy(szFileName, PathFindFileName(strRelative));

		// 확장자 명 제거.
		PathRemoveExtension(szFileName);		

		// 파일 이름을 콤보 박스에 추가
		if (mapRect.PtInRect(mousePosition))
		{
			// 맵 중복 확인.
			auto iter = find_if(m_mapBackground.begin(), m_mapBackground.end(), [szFileName](auto& pair)
				{
					// find_if에서 true를 반환하면 해당 iterator 요소를 반환.
					return pair.first == szFileName;
				});

			if (iter != m_mapBackground.end())
			{
				// 중복 키.
				continue;
			}

			// 맵 리스트 드래그.
			m_ListBoxMap.AddString(szFileName);

			CImage* bg = new CImage();
			bg->Load(strRelative);

			m_mapBackground.insert({ szFileName , bg });
		}
		else if (objectRect.PtInRect(mousePosition))
		{
			// 맵 중복 확인.
			auto iter = find_if(m_objectBackground.begin(), m_objectBackground.end(), [szFileName](auto& pair)
				{
					// find_if에서 true를 반환하면 해당 iterator 요소를 반환.
					return pair.first == szFileName;
				});

			if (iter != m_objectBackground.end())
			{
				// 중복 키.
				continue;
			}

			// 오브젝트 리스트 드래그.
			m_ListBoxObject.AddString(szFileName);

			CImage* bg = new CImage();
			bg->Load(strRelative);

			m_objectBackground.insert({ szFileName , bg });
		}

		if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
			strRelative,
			TEX_SINGLE, szFileName, nullptr, 0)))
		{
			AfxMessageBox(L"Background Texture Insert Failed");
			return;
		}		
	}

	Safe_Delete(szFilePath);
	Safe_Delete(szFileName);

	CDC* pDC = m_ListBoxMap.GetDC();
	CString		strName;
	int iWidth = 0;

	for (int i = 0; i < m_ListBoxMap.GetCount(); ++i)
	{
		m_ListBoxMap.GetText(i, strName);

		// GetTextExtent : 매개 변수에 해당하는 문자열의 길이를 픽셀 단위로 변환.
		if (pDC->GetTextExtent(strName).cx > iWidth)
		{
			iWidth = pDC->GetTextExtent(strName).cx;
		}
	}

	// 가로 스크롤 크기 조정.
	m_ListBoxMap.SetHorizontalExtent(iWidth);

	UpdateRender();

	m_ListBoxMap.ReleaseDC(pDC);

	UpdateData(FALSE);	
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RATIO_SLIDER, &CMapTool::OnSlider)
	ON_WM_DROPFILES()		// 드래그 이벤트 추가.
	ON_WM_DESTROY()			// 파괴 함수 추가.
	ON_LBN_SELCHANGE(IDC_LIST_BOX_MAP, &CMapTool::OnListBGClick)
	ON_BN_CLICKED(IDC_MAP_APPLY_BUTTON, &CMapTool::OnApplyClick)
	ON_BN_CLICKED(IDC_MAP_DELETE_BUTTON, &CMapTool::OnDeleteClick)
	ON_LBN_SELCHANGE(IDC_LIST_BOX_OBJECT, &CMapTool::OnListObjectClick)
	ON_BN_CLICKED(IDC_OBJECT_APPLY_BUTTON, &CMapTool::OnObjectApplyClick)
	ON_BN_CLICKED(IDC_OBJECT_DELETE_BUTTON, &CMapTool::OnObjectDeleteClick)
	ON_BN_CLICKED(IDC_ROOM_ADD_BUTTON, &CMapTool::OnRoomAdd)
	ON_CBN_SELCHANGE(IDC_MAP_COMBO, &CMapTool::OnRoomClick)
	ON_COMMAND(ID_DELETE, &CMapTool::OnDeleteObject)
	ON_COMMAND(ID_UPDATE, &CMapTool::OnNameUpdateObject)
	ON_WM_CONTEXTMENU()
	ON_EN_KILLFOCUS(IDC_OBJECT_NAME_EDIT, &CMapTool::OnNameChange)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_NameEdit.ShowWindow(SW_HIDE);	

	if (m_objectKey == nullptr)
	{
		m_objectKey = new TCHAR[MAX_PATH];
	}

	// TODO:  여기에 추가 초기화 작업을 추가합니다.	
	m_RatioSlider.SetRange(20, 100);	
	m_RatioSlider.SetPos(100);	

	m_RoomComboBox.AddString(L"룸 1");
	m_RoomComboBox.SelectString(0, L"룸 1");
	
	UpdateRender();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::OnSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_Ratio = m_RatioSlider.GetPos() / 100.f;
	UpdateRender();
	*pResult = 0;	
}

void CMapTool::UpdateRender()
{
	TCHAR	szBuf[MIN_STR] = L"";
	swprintf_s(szBuf, L"현재 맵 비율 : %f", g_Ratio);
	m_RatioText.SetWindowTextW(szBuf);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CTerrain* pTerrain = pView->m_pTerrain;
	CDevice::Get_Instance()->Render_Begin();

	pTerrain->Render();

	CDevice::Get_Instance()->Render_End();
}

CString CMapTool::ConvertRelativePath(CString _path)
{
	// 상대 경로.
	TCHAR   szRelativePath[MAX_PATH] = L"";

	// 프로젝트 경로.
	TCHAR   szCurDirPath[MAX_PATH] = L"";

	// 프로젝트 경로 추출.
	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : 절대 경로를 상대 경로로 변환
	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		_path.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapBackground.begin(), m_mapBackground.end(), [](auto& pair)
		{
			pair.second->Destroy();
			Safe_Delete(pair.second);
		});

	m_mapBackground.clear();

	for_each(m_objectBackground.begin(), m_objectBackground.end(), [](auto& pair)
		{
			pair.second->Destroy();
			Safe_Delete(pair.second);
		});

	m_objectBackground.clear();

	Safe_Delete(m_objectKey);	
}

// 배경 리스트 박스 클릭.
void CMapTool::OnListBGClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_ListBoxMap.GetCurSel();

	m_ListBoxMap.GetText(iIndex, strFindName);

	auto	iter = m_mapBackground.find(strFindName);

	if (iter == m_mapBackground.end())
		return;

	CDC* dc = m_BGPicture.GetDC();		

	iter->second->StretchBlt(*dc, 0, 0, 426, 220, SRCCOPY);

	m_BGPicture.ReleaseDC(dc);

	UpdateData(FALSE);
}

// 적용 버튼.
void CMapTool::OnApplyClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxMap.GetText(m_ListBoxMap.GetCurSel(), strItem);

	// CString에서 TCHAR 배열로 복사
	TCHAR* bg = new TCHAR[MAX_PATH];

	_tcscpy_s(bg, MAX_PATH, strItem);

	delete CMapManager::Get_Instance()->m_vecBG[CMapManager::Get_Instance()->m_RoomIndex]->szBGKey;
	CMapManager::Get_Instance()->m_vecBG[CMapManager::Get_Instance()->m_RoomIndex]->szBGKey = bg;

	UpdateRender();
}

// 삭제 버튼.
void CMapTool::OnDeleteClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxMap.GetText(m_ListBoxMap.GetCurSel(), strItem);
	m_ListBoxMap.DeleteString(m_ListBoxMap.GetCurSel());	

	Safe_Delete(m_mapBackground[strItem]);
	m_mapBackground.erase(strItem);

	UpdateRender();
}

void CMapTool::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd == GetDlgItem(IDC_LIST_BOX_OBJECT_LIST) && m_ListBoxObjectList.GetCurSel() != -1)
	{
		currentCurIndex = m_ListBoxObjectList.GetCurSel();

		// 컨텍스트 메뉴 생성
		CMenu menu;
		menu.CreatePopupMenu();

		// 메뉴 항목 추가 (속성 항목)
		menu.AppendMenu(MF_STRING, ID_UPDATE, L"이름 수정");
		menu.AppendMenu(MF_STRING, ID_DELETE, L"삭제");

		menu.TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
	}
}

// 오브젝트 리스트 박스 클릭.
void CMapTool::OnListObjectClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 커서가 선택한 셀의 인덱스 값을 반환
	int	iIndex = m_ListBoxObject.GetCurSel();

	m_ListBoxObject.GetText(iIndex, strFindName);

	auto	iter = m_objectBackground.find(strFindName);

	if (iter == m_objectBackground.end())
		return;

	CDC* dc = m_ObjectPicture.GetDC();
	//m_BGPicture.SetBitmap(*(iter->second));
	iter->second->StretchBlt(*dc, 0, 0, 299, 151, SRCCOPY);

	m_ObjectPicture.ReleaseDC(dc);

	UpdateData(FALSE);
}

// 오브젝트 배치 버튼.
void CMapTool::OnObjectApplyClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxObject.GetText(m_ListBoxObject.GetCurSel(), strItem);

	// CString에서 TCHAR 배열로 복사
	_tcscpy_s(m_objectKey, MAX_PATH, strItem);
	
	CMapManager::Get_Instance()->SetObjectKey(m_objectKey);
	CMapManager::Get_Instance()->m_IsObjectSetting = true;
}

// 오브젝트 삭제 버튼.
void CMapTool::OnObjectDeleteClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strItem;
	m_ListBoxObject.GetText(m_ListBoxObject.GetCurSel(), strItem);
	m_ListBoxObject.DeleteString(m_ListBoxObject.GetCurSel());

	Safe_Delete(m_objectBackground[strItem]);
	m_objectBackground.erase(strItem);

	UpdateRender();
}

// 룸 추가.
void CMapTool::OnRoomAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int count = m_RoomComboBox.GetCount();
	CMapManager::Get_Instance()->CreateRoom();

	CString szRoom;
	szRoom.Format(L"룸 %d", count + 1);

	m_RoomComboBox.AddString(szRoom);
	m_RoomComboBox.SelectString(count + 1, szRoom);

	szRoom += L"번이 추가되었습니다";

	MessageBox(szRoom, L"룸", MB_OK);

	// 이전 룸에 배치 된 오브젝트 목록 비움.
	m_ListBoxObjectList.ResetContent();
}


void CMapTool::OnRoomClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 룸에 배치 된 오브젝트 목록 비움.
	m_ListBoxObjectList.ResetContent();

	CMapManager::Get_Instance()->m_RoomIndex = m_RoomComboBox.GetCurSel();

	int roomIndex = CMapManager::Get_Instance()->m_RoomIndex;

	for (auto& obj : CMapManager::Get_Instance()->m_vecObject[roomIndex])
	{
		m_ListBoxObjectList.AddString(obj->szShowName->GetString());
	}
}

// 오브젝트 목록에서 해당 오브젝트 삭제.
void CMapTool::OnDeleteObject()
{
	m_ListBoxObjectList.DeleteString(m_ListBoxObjectList.GetCurSel());

	auto it = CMapManager::Get_Instance()->m_vecObject[CMapManager::Get_Instance()->m_RoomIndex].begin();

	for (int i = 0; i < CMapManager::Get_Instance()->m_vecObject[CMapManager::Get_Instance()->m_RoomIndex].size(); i++)
	{
		if (i == currentCurIndex)
		{
			Safe_Delete(*it);
			CMapManager::Get_Instance()->m_vecObject[CMapManager::Get_Instance()->m_RoomIndex].erase(it);			
			break;
		}

		it++;
	}

	currentCurIndex = -1;
}

// 이름 수정.
void CMapTool::OnNameUpdateObject()
{
	int currentCell = m_ListBoxObjectList.GetCurSel();	

	CRect rect;
	CRect brect;

	// m_ListBoxObjectList 좌표를 윈도우 창 기준으로 가져온다.
	m_ListBoxObjectList.GetWindowRect(&rect);
	m_NameEdit.GetWindowRect(&brect);

	// 클라이언트 좌표로 변경.
	ScreenToClient(&rect);
	ScreenToClient(&brect);

	CString* name = CMapManager::Get_Instance()->m_vecObject[CMapManager::Get_Instance()->m_RoomIndex][currentCell]->szShowName;
	m_NameEdit.SetWindowText(*name);
	m_NameEdit.SetWindowPos(NULL, rect.left, rect.top + (13 * currentCell), rect.Width(), brect.Height(), SWP_SHOWWINDOW);
	m_NameEdit.SetFocus();	
}


void CMapTool::OnNameChange()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int currentCell = m_ListBoxObjectList.GetCurSel();

	CString* newName = new CString();

	m_NameEdit.GetWindowText(*newName);

	// 기존 항목 삭제 후 새 항목 삽입
	m_ListBoxObjectList.DeleteString(currentCell);
	m_ListBoxObjectList.InsertString(currentCell, *newName);

	delete CMapManager::Get_Instance()->m_vecObject[CMapManager::Get_Instance()->m_RoomIndex][currentCell]->szShowName;
	CMapManager::Get_Instance()->m_vecObject[CMapManager::Get_Instance()->m_RoomIndex][currentCell]->szShowName = newName;

	m_NameEdit.ShowWindow(SW_HIDE);
}
