// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
{

}

CMapTool::~CMapTool()
{
	Release();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]); // 추후 수정 각?
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);

	DDX_Text(pDX, IDC_EDIT1, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT2, m_fScaleY);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelectTileList)
	//ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnClickedLoad)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMapTool::OnCbnSelchangeTileType)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSetback)
	ON_EN_CHANGE(IDC_EDIT1, &CMapTool::OnEnChangeScaleX)
	ON_EN_CHANGE(IDC_EDIT2, &CMapTool::OnEnChangeScaleY)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ComboBox.SetWindowText(L"Choose Tile Type");
	m_ComboBox.AddString(L"Floor");
	m_ComboBox.AddString(L"Wall");
	m_ComboBox.AddString(L"Door");
	m_ComboBox.AddString(L"Deco");
	m_ComboBox.AddString(L"Weapon");

	m_Radio[0].SetCheck(TRUE);
	m_eCurID = MAPID::FLOOR;
	m_ePreID = MAPID::FLOOR;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CMapTool::OnLbnSelectTileList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iSelect = m_ListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	CString strSelectName;
	m_ListBox.GetText(iSelect, strSelectName);

	auto iter_find = m_MapImg[m_eCurID].find(strSelectName);

	if (m_MapImg[m_eCurID].end() == iter_find)
		return;

	// SetBitmap(CImage img): CImage 객체가 가진 리소스를 픽처컨트롤에 출력.
	m_PictureCtrl.SetBitmap(*(iter_find->second));

	// 리스트박스에서 선택된 타일의 DrawID를 얻어내자!
	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit: 현재 문자가 숫자인지 검사.
		// 숫자라고 판명이 날 경우 0이 아닌 값을 반환.
		if (0 != isdigit(strSelectName[i]))
			break;
	}

	// CString::Delete(start, count): 현재 문자열의 start위치부터 count만큼 문자들을 제거.
	strSelectName.Delete(0, i);

	// _tstoi: 문자를 정수타입으로 변환하는 함수.
	m_iDrawID = _tstoi(strSelectName);	

	//추가~ 마우스에 이미지 띄우는 코드
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	pMainView->m_ObjKey = GetStateKey(m_eCurID);
	pMainView->m_StateKey = GetStateKey(m_eCurID);
	pMainView->m_iIndex = m_iDrawID;
	pMainView->m_bIsTerrainOn = true;
	//~추가

	UpdateData(FALSE);
}

// OnDropFiles: 파일을 드래그 앤 드롭 하였을 때 WM_DROPFILES 메시지가 발생하고 해당 함수를 호출.
// HDROP hDropInfo: 드롭된 파일들의 정보를 갖는 구조체.
//void CMapTool::OnDropFiles(HDROP hDropInfo)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CDialog::OnDropFiles(hDropInfo);
//
//	UpdateData(TRUE);
//
//	m_ListBox.ResetContent();	// 현재 리스트박스 초기화.
//	TCHAR szFilePath[MAX_STR] = L"";
//
//	// DragQueryFile: 드래그 앤 드롭된 파일의 정보를 얻어올 수 있는 함수.
//	// 드롭된 파일들 중 index에 해당하는 파일의 경로를 얻어옴.
//	// DragQueryFile(hDropInfo, index, szFilePath, MAX_STR);
//
//	// 만약에 DragQueryFile함수의 두번째 인자가 -1일 경우 드롭된 파일의 전체 개수를 반환.
//	int iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);
//
//	TCHAR szFileName[MAX_STR] = L"";
//
//	for (int i = 0; i < iFileCnt; ++i)
//	{
//		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
//
//		// 상대경로로 변환.
//		CString strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
//
//		// 경로에서 파일명만 남기기.
//		// PathFindFileName: 인자로 받은 경로에서 파일명만 추출해주는 함수.
//		CString strFileName = PathFindFileName(strRelativePath.GetString());
//
//		// 파일명에서 확장자 제거.
//		// PathRemoveExtension: 현재 파일명에서 확장자를 제거해주는 함수.
//		lstrcpy(szFileName, strFileName.GetString());		
//		PathRemoveExtension(szFileName);
//
//		// strFileName을 map의 key로 사용할 것!
//		strFileName = szFileName;
//
//		auto iter_find = m_MapImg.find(strFileName);
//
//		// 해당 키가 존재하지 않을 때 CImage를 map에 새로 추가한다.
//		if (m_MapImg.end() == iter_find)
//		{
//			CImage* pImage = new CImage;
//			pImage->Load(strRelativePath); // 상대경로로부터 이미지를 로드.
//
//			m_MapImg.insert({ strFileName, pImage });
//			m_ListBox.AddString(szFileName);
//		}
//	}
//
//	// 리스트 박스의 가로 스크롤을 생성.
//	HorizontalScroll();
//
//	UpdateData(FALSE);
//}

void CMapTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName().GetString(), GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Tile Save Failed!!");
			return;
		}

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
		CTerrain*	pTerrain = pMainView->GetTerrain();
		//CWall*	pWall = pMainView->GetWall();
		/*CDoor*	pDoor = pMainView->GetDoor();
		CDeco*	pDeco = pMainView->GetDeco();
		CWeapon*	pWeapon = pMainView->GetWeapon();*/

		// 콤보박스 선택지에 따라 저장
		/*if (m_eCurID == MAPID::FLOOR)
		{*/
			vector<INFO*>& vecFloor = pTerrain->GetVecTile();
			vector<INFO*>& vecWall = pTerrain->GetVecWall();
			vector<INFO*>& vecDoor = pTerrain->GetVecDoor();
			vector<INFO*>& vecDeco = pTerrain->GetVecDeco();
			vector<INFO*>& vecWeapon = pTerrain->GetVecWeapon();


			DWORD dwByte = 0;

			for (auto& pTile : vecFloor)
				WriteFile(hFile, pTile, sizeof(INFO), &dwByte, nullptr);

			for (auto& pTile : vecWall)
				WriteFile(hFile, pTile, sizeof(INFO), &dwByte, nullptr);
			for (auto& pTile : vecDoor)
				WriteFile(hFile, pTile, sizeof(INFO), &dwByte, nullptr);
			for (auto& pTile : vecDeco)
				WriteFile(hFile, pTile, sizeof(INFO), &dwByte, nullptr);
			for (auto& pTile : vecWeapon)
				WriteFile(hFile, pTile, sizeof(INFO), &dwByte, nullptr);

			CloseHandle(hFile);
		//}
	}	
	UpdateData(FALSE);
}

	

void CMapTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName().GetString(), GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Tile Load Failed!!");
			return;
		}

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));
		CTerrain*	pTerrain = pMainView->GetTerrain();
		//CWall*		pWall = pMainView->GetWall();
		/*CDoor*		pDoor = pMainView->GetDoor();
		CDeco*		pDeco = pMainView->GetDeco();
		CWeapon*	pWeapon = pMainView->GetWeapon();*/

		if (nullptr == pTerrain/* || nullptr == pWall*/
			/*|| nullptr == pDoor || nullptr == pDeco
			|| nullptr == pWeapon*/)
		{
			AfxMessageBox(L"Tile Load Failed!!");
			return;
		}

		pTerrain->Release();
		//pWall->Release();
		/*pDoor->Release();
		pDeco->Release();
		pWeapon->Release();*/
		vector<INFO*>& vecTile = pTerrain->GetVecTile();
		vector<INFO*>& vecWall = pTerrain->GetVecWall();
		vector<INFO*>& vecDoor = pTerrain->GetVecDoor();
		vector<INFO*>& vecDeco = pTerrain->GetVecDeco();
		vector<INFO*>& vecWeapon = pTerrain->GetVecWeapon();


		INFO tTile = {};
		DWORD dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tTile, sizeof(INFO), &dwByte, nullptr);
			
			if (0 == dwByte)
				break;

			if (tTile.eMapID == MAPID::FLOOR)
			{
				INFO* pTile = new INFO(tTile);
				vecTile.push_back(pTile);
			}
			if (tTile.eMapID == MAPID::WALL)
			{
				INFO* pTile = new INFO(tTile);
				vecWall.push_back(pTile);
			}
			if (tTile.eMapID == MAPID::DOOR)
			{
				INFO* pTile = new INFO(tTile);
				vecDoor.push_back(pTile);
			}
			if (tTile.eMapID == MAPID::DECO)
			{
				INFO* pTile = new INFO(tTile);
				vecDeco.push_back(pTile);
			}
			if (tTile.eMapID == MAPID::WEAPON)
			{
				INFO* pTile = new INFO(tTile);
				vecWeapon.push_back(pTile);
			}
			
		}

		/*vector<TILE*>& vecWall = pWall->GetVecTile();

		TILE tTile1 = {};
		DWORD dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tTile1, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			TILE* pTile = new TILE(tTile1);
			vecWall.push_back(pTile);
		}*/

		/*vector<TILE*>& vecDoor = pDoor->GetVecTile();
		TILE tTile2 = {};
		dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tTile2, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			TILE* pTile = new TILE(tTile2);
			vecDoor.push_back(pTile);
		}*/

		/*vector<TILE*>& vecDeco = pDeco->GetVecTile();
		TILE tTile3 = {};
		dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tTile3, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			TILE* pTile = new TILE(tTile3);
			vecDeco.push_back(pTile);
		}*/

	/*	vector<TILE*>& vecWeapon = pWeapon->GetVecTile();
		TILE tTile4 = {};
		dwByte = 0;

		while (true)
		{
			ReadFile(hFile, &tTile4, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			TILE* pTile = new TILE(tTile4);
			vecWeapon.push_back(pTile);
		}



		CloseHandle(hFile);
		pMainView->Invalidate(FALSE);
	}*/

		UpdateData(FALSE);
	}
}

void CMapTool::HorizontalScroll()
{
	CString strName;
	CSize	size;

	// 리스트 박스 내에서 문자열이 가장 긴 녀석을 기준으로 가로 스크롤바 크기를 결정.
	int iMaxCX = 0;

	CDC* pDC = m_ListBox.GetDC(); // 리스트 박스의 DC를 얻어온다.

	// CListBox::GetCount(): 현재 리스트 박스의 목록 개수를 얻어옴.
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// CDC::GetTextExtent(string): string의 길이를 픽셀단위로 변환.
		size = pDC->GetTextExtent(strName);

		// 이 중에서 제일 긴 녀석을 찾는다.
		if (size.cx > iMaxCX)
			iMaxCX = size.cx;
	}

	// 위에서 얻어온 또다른 DC를 해제.
	m_ListBox.ReleaseDC(pDC);

	// iMaxCX를 기준으로 스크롤바 생성.
	if (iMaxCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iMaxCX); // iMaxCX 크기만큼 가로 스크롤바가 생성!

}

void CMapTool::Release()
{
	for (int i = 0; i < MAPID::MAP_END; ++i)
	{
		for_each(m_MapImg[i].begin(), m_MapImg[i].end(),
			[](auto& MyPair)
		{
			// Destroy: CImage객체가 갖고 있던 리소스 자원에 대한 해제함수.
			MyPair.second->Destroy();
			SafeDelete(MyPair.second);
		});

		m_MapImg[i].clear();
	}
}

void CMapTool::SetTileLst()
{
	UpdateData(TRUE);
	m_ListBox.ResetContent();
	for_each(m_MapImg[m_eCurID].begin(), m_MapImg[m_eCurID].end(),
		[](auto& MyPair)
	{
		// Destroy: CImage객체가 갖고 있던 리소스 자원에 대한 해제함수.
		MyPair.second->Destroy();
		SafeDelete(MyPair.second);
	});

	m_MapImg[m_eCurID].clear();

	const TCHAR* szStateKey = GetStateKey(m_eCurID);

	int iMax = CTextureMgr::GetInstance()->GetTexture(szStateKey, szStateKey, 0)->iCount;
	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iMax; ++i)
	{
		CString strRelativePath = CTextureMgr::GetInstance()->GetTexture(szStateKey, szStateKey, i)->szImagePath;

		// 경로에서 파일명만 남기기.
		// PathFindFileName: 인자로 받은 경로에서 파일명만 추출해주는 함수.
		CString strFileName = PathFindFileName(strRelativePath.GetString());

		// 파일명에서 확장자 제거.
		// PathRemoveExtension: 현재 파일명에서 확장자를 제거해주는 함수.
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		// strFileName을 map의 key로 사용할 것!
		strFileName = szFileName;

		auto iter_find = m_MapImg[m_eCurID].find(strFileName);

		// 해당 키가 존재하지 않을 때 CImage를 map에 새로 추가한다.
		if (m_MapImg[m_eCurID].end() == iter_find)
		{
			CImage* pImage = new CImage;
			pImage->Load(strRelativePath); // 상대경로로부터 이미지를 로드.

			m_MapImg[m_eCurID].insert({ strFileName, pImage });
			m_ListBox.AddString(szFileName);
		}

		UpdateData(FALSE);
	}
}

const TCHAR * CMapTool::GetStateKey(MAPID eID)
{
	switch (eID)
	{
	case FLOOR:
		return L"Floor";
		break;
	case WALL:
		return L"Wall";
		break;
	case DOOR:
		return L"Door";
		break;
	case DECO:
		return L"Deco";
		break;
	case WEAPON:
		return L"Weapon";
		break;

	default:
		break;
	}
	return nullptr;
}

// 이걸로 안돼
//void CMapTool::SetMapID(const TCHAR * StateKey)
//{
//	
//	if (StateKey == L"Floor")
//	{
//		m_eCurID = MAPID::FLOOR;
//	}
//	if (StateKey == L"Wall")
//	{
//		m_eCurID = MAPID::WALL;
//	}
//	if (StateKey == L"Door")
//	{
//		m_eCurID = MAPID::DOOR;
//	}
//	if (StateKey == L"Deco")
//	{
//		m_eCurID = MAPID::DECO;
//	}
//	if (StateKey == L"Weapon")
//	{
//		m_eCurID = MAPID::WEAPON;
//	}
//}


// 콤보박스의 선택지를 누르면 리스트박스에 스테이트키에 속한 이미지들이 로드되게
void CMapTool::OnCbnSelchangeTileType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//if (m_eCurID == m_ePreID) // 추가
	//	return;

	CString m_strSelectName = L"";

	// GetCurSel: 현재 콤보박스에서 선택된 목록의 인덱스를 얻어오는 함수.
	int iSelect = m_ComboBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	// GetLBText: 인덱스에 해당하는 문자열을 콤보박스로부터 얻어오는 함수.
	m_ComboBox.GetLBText(iSelect, m_strSelectName);
	if (m_strSelectName == L"Floor")
	{
		m_eCurID = MAPID::FLOOR;
	}
	if (m_strSelectName == L"Wall")
	{
		m_eCurID = MAPID::WALL;
	}
	if (m_strSelectName == L"Door")
	{
		m_eCurID = MAPID::DOOR;
	}
	if (m_strSelectName == L"Deco")
	{
		m_eCurID = MAPID::DECO;
	}
	if (m_strSelectName == L"Weapon")
	{
		m_eCurID = MAPID::WEAPON;
	}
	//SetMapID(m_strSelectName);

	/*auto& iter_find = m_MapImg[m_eCurID].find(m_strSelectName);

	if (m_MapImg[m_eCurID].end() == iter_find)
		return;*/

	SetTileLst();

	//m_ePreID = m_eCurID; // 추가

	UpdateData(FALSE);
}


void CMapTool::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);
	if (nID == SC_CLOSE)
	{
		CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView*	pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

		pMainView->m_bIsTerrainOn = false;
	}

}



void CMapTool::OnBnClickedSetback()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMainFram = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFram->m_MainSplit.GetPane(0, 1));
	CTerrain* pTerrain = pMainView->GetTerrain();

	if (m_eCurID == MAPID::FLOOR)
	{
		if (!pTerrain->GetVecTile().empty())
		{
			pTerrain->GetVecTile().pop_back();
			//m_iVecSize = pTerrain->GetVecTile.size();
		}
	}
	if (m_eCurID == MAPID::WALL)
	{
		if (!pTerrain->GetVecWall().empty())
		{
			pTerrain->GetVecWall().pop_back();
			//m_iVecSize = pTerrain->GetVecTile.size();
		}
	}
	if (m_eCurID == MAPID::DOOR)
	{
		if (!pTerrain->GetVecDoor().empty())
		{
			pTerrain->GetVecDoor().pop_back();
			//m_iVecSize = pTerrain->GetVecTile.size();
		}
	}
	if (m_eCurID == MAPID::DECO)
	{
		if (!pTerrain->GetVecDeco().empty())
		{
			pTerrain->GetVecDeco().pop_back();
			//m_iVecSize = pTerrain->GetVecTile.size();
		}
	}
	if (m_eCurID == MAPID::WEAPON)
	{
		if (!pTerrain->GetVecWeapon().empty())
		{
			pTerrain->GetVecWeapon().pop_back();
			//m_iVecSize = pTerrain->GetVecTile.size();
		}
	}

	pMainView->Invalidate(FALSE);
}


void CMapTool::OnEnChangeScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame* pMainFram = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFram->m_MainSplit.GetPane(0, 1));

	pMainView->m_fScaleX = m_fScaleX;

	UpdateData(FALSE);
}


void CMapTool::OnEnChangeScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame* pMainFram = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFram->m_MainSplit.GetPane(0, 1));

	pMainView->m_fScaleY = m_fScaleY;

	UpdateData(FALSE);
}
