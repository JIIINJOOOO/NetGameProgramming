// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool ��ȭ �����Դϴ�.

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
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]); // ���� ���� ��?
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


// CMapTool �޽��� ó�����Դϴ�.

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void CMapTool::OnLbnSelectTileList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iSelect = m_ListBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	CString strSelectName;
	m_ListBox.GetText(iSelect, strSelectName);

	auto iter_find = m_MapImg[m_eCurID].find(strSelectName);

	if (m_MapImg[m_eCurID].end() == iter_find)
		return;

	// SetBitmap(CImage img): CImage ��ü�� ���� ���ҽ��� ��ó��Ʈ�ѿ� ���.
	m_PictureCtrl.SetBitmap(*(iter_find->second));

	// ����Ʈ�ڽ����� ���õ� Ÿ���� DrawID�� ����!
	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit: ���� ���ڰ� �������� �˻�.
		// ���ڶ�� �Ǹ��� �� ��� 0�� �ƴ� ���� ��ȯ.
		if (0 != isdigit(strSelectName[i]))
			break;
	}

	// CString::Delete(start, count): ���� ���ڿ��� start��ġ���� count��ŭ ���ڵ��� ����.
	strSelectName.Delete(0, i);

	// _tstoi: ���ڸ� ����Ÿ������ ��ȯ�ϴ� �Լ�.
	m_iDrawID = _tstoi(strSelectName);	

	//�߰�~ ���콺�� �̹��� ���� �ڵ�
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	pMainView->m_ObjKey = GetStateKey(m_eCurID);
	pMainView->m_StateKey = GetStateKey(m_eCurID);
	pMainView->m_iIndex = m_iDrawID;
	pMainView->m_bIsTerrainOn = true;
	//~�߰�

	UpdateData(FALSE);
}

// OnDropFiles: ������ �巡�� �� ��� �Ͽ��� �� WM_DROPFILES �޽����� �߻��ϰ� �ش� �Լ��� ȣ��.
// HDROP hDropInfo: ��ӵ� ���ϵ��� ������ ���� ����ü.
//void CMapTool::OnDropFiles(HDROP hDropInfo)
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//	CDialog::OnDropFiles(hDropInfo);
//
//	UpdateData(TRUE);
//
//	m_ListBox.ResetContent();	// ���� ����Ʈ�ڽ� �ʱ�ȭ.
//	TCHAR szFilePath[MAX_STR] = L"";
//
//	// DragQueryFile: �巡�� �� ��ӵ� ������ ������ ���� �� �ִ� �Լ�.
//	// ��ӵ� ���ϵ� �� index�� �ش��ϴ� ������ ��θ� ����.
//	// DragQueryFile(hDropInfo, index, szFilePath, MAX_STR);
//
//	// ���࿡ DragQueryFile�Լ��� �ι�° ���ڰ� -1�� ��� ��ӵ� ������ ��ü ������ ��ȯ.
//	int iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);
//
//	TCHAR szFileName[MAX_STR] = L"";
//
//	for (int i = 0; i < iFileCnt; ++i)
//	{
//		DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
//
//		// ����η� ��ȯ.
//		CString strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
//
//		// ��ο��� ���ϸ� �����.
//		// PathFindFileName: ���ڷ� ���� ��ο��� ���ϸ� �������ִ� �Լ�.
//		CString strFileName = PathFindFileName(strRelativePath.GetString());
//
//		// ���ϸ��� Ȯ���� ����.
//		// PathRemoveExtension: ���� ���ϸ��� Ȯ���ڸ� �������ִ� �Լ�.
//		lstrcpy(szFileName, strFileName.GetString());		
//		PathRemoveExtension(szFileName);
//
//		// strFileName�� map�� key�� ����� ��!
//		strFileName = szFileName;
//
//		auto iter_find = m_MapImg.find(strFileName);
//
//		// �ش� Ű�� �������� ���� �� CImage�� map�� ���� �߰��Ѵ�.
//		if (m_MapImg.end() == iter_find)
//		{
//			CImage* pImage = new CImage;
//			pImage->Load(strRelativePath); // ����ηκ��� �̹����� �ε�.
//
//			m_MapImg.insert({ strFileName, pImage });
//			m_ListBox.AddString(szFileName);
//		}
//	}
//
//	// ����Ʈ �ڽ��� ���� ��ũ���� ����.
//	HorizontalScroll();
//
//	UpdateData(FALSE);
//}

void CMapTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

		// �޺��ڽ� �������� ���� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// ����Ʈ �ڽ� ������ ���ڿ��� ���� �� �༮�� �������� ���� ��ũ�ѹ� ũ�⸦ ����.
	int iMaxCX = 0;

	CDC* pDC = m_ListBox.GetDC(); // ����Ʈ �ڽ��� DC�� ���´�.

	// CListBox::GetCount(): ���� ����Ʈ �ڽ��� ��� ������ ����.
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// CDC::GetTextExtent(string): string�� ���̸� �ȼ������� ��ȯ.
		size = pDC->GetTextExtent(strName);

		// �� �߿��� ���� �� �༮�� ã�´�.
		if (size.cx > iMaxCX)
			iMaxCX = size.cx;
	}

	// ������ ���� �Ǵٸ� DC�� ����.
	m_ListBox.ReleaseDC(pDC);

	// iMaxCX�� �������� ��ũ�ѹ� ����.
	if (iMaxCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iMaxCX); // iMaxCX ũ�⸸ŭ ���� ��ũ�ѹٰ� ����!

}

void CMapTool::Release()
{
	for (int i = 0; i < MAPID::MAP_END; ++i)
	{
		for_each(m_MapImg[i].begin(), m_MapImg[i].end(),
			[](auto& MyPair)
		{
			// Destroy: CImage��ü�� ���� �ִ� ���ҽ� �ڿ��� ���� �����Լ�.
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
		// Destroy: CImage��ü�� ���� �ִ� ���ҽ� �ڿ��� ���� �����Լ�.
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

		// ��ο��� ���ϸ� �����.
		// PathFindFileName: ���ڷ� ���� ��ο��� ���ϸ� �������ִ� �Լ�.
		CString strFileName = PathFindFileName(strRelativePath.GetString());

		// ���ϸ��� Ȯ���� ����.
		// PathRemoveExtension: ���� ���ϸ��� Ȯ���ڸ� �������ִ� �Լ�.
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		// strFileName�� map�� key�� ����� ��!
		strFileName = szFileName;

		auto iter_find = m_MapImg[m_eCurID].find(strFileName);

		// �ش� Ű�� �������� ���� �� CImage�� map�� ���� �߰��Ѵ�.
		if (m_MapImg[m_eCurID].end() == iter_find)
		{
			CImage* pImage = new CImage;
			pImage->Load(strRelativePath); // ����ηκ��� �̹����� �ε�.

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

// �̰ɷ� �ȵ�
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


// �޺��ڽ��� �������� ������ ����Ʈ�ڽ��� ������ƮŰ�� ���� �̹������� �ε�ǰ�
void CMapTool::OnCbnSelchangeTileType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	//if (m_eCurID == m_ePreID) // �߰�
	//	return;

	CString m_strSelectName = L"";

	// GetCurSel: ���� �޺��ڽ����� ���õ� ����� �ε����� ������ �Լ�.
	int iSelect = m_ComboBox.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	// GetLBText: �ε����� �ش��ϴ� ���ڿ��� �޺��ڽ��κ��� ������ �Լ�.
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

	//m_ePreID = m_eCurID; // �߰�

	UpdateData(FALSE);
}


void CMapTool::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CMainFrame* pMainFram = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFram->m_MainSplit.GetPane(0, 1));

	pMainView->m_fScaleX = m_fScaleX;

	UpdateData(FALSE);
}


void CMapTool::OnEnChangeScaleY()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CMainFrame* pMainFram = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFram->m_MainSplit.GetPane(0, 1));

	pMainView->m_fScaleY = m_fScaleY;

	UpdateData(FALSE);
}
