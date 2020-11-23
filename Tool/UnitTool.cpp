// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strType(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	Release();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO4, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio[0]);

}

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Radio[0].SetCheck(TRUE);

	m_hBitmap = (HBITMAP)LoadImage(nullptr, L"../Texture/JusinLogo1.bmp",
		IMAGE_BITMAP, 100, 50, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_PushBtn.SetBitmap(m_hBitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelectClass)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.



void CUnitTool::OnLbnSelectClass()
{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//UpdateData(TRUE);

	//CString m_strSelectName = L"";

	//// GetCurSel: ���� ����Ʈ�ڽ����� ���õ� ����� �ε����� ������ �Լ�.
	//int iSelect = m_ListBox.GetCurSel();

	//// GetText: �ε����� �ش��ϴ� ���ڿ��� ����Ʈ�ڽ��κ��� ������ �Լ�.
	//m_ListBox.GetText(iSelect, m_strSelectName);

	//auto& iter_find = m_MapData.find(m_strSelectName);

	//if (m_MapData.end() == iter_find)
	//	return;

	//m_strType = iter_find->second->strName;

	//for (int i = 0; i < 3; ++i)
	//	m_Radio[i].SetCheck(FALSE);	// ���� ��ư üũ ����

	//// ���� ����Ʈ ��Ͽ��� ���õ� �ε����� �ش��ϴ� ������ư üũ!
	//m_Radio[iter_find->second->byJobIndex].SetCheck(TRUE);

	//

	//UpdateData(FALSE);	
}


void CUnitTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// CFileDialog: ���� ���� Ȥ�� ���� �۾��� �ʿ��� ��ȭ���ڸ� �����ϴ� ��ü.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	// GetCurrentDirectory: ���� �۾� ��θ� ����.
	GetCurrentDirectory(MAX_STR, szPath);

	// Data������ �⺻ ��η� �����.
	
	// PathRemoveFileSpec: ��ü ��ο��� ���ϸ� �߶󳻴� �Լ�.
	// ��, ��� �� ���ϸ��� ���ٸ� ���� ���� �������� �߶��ش�.
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	// lpstrInitialDir: ��ȭ���ڸ� ������ �� �⺻ ��θ� ����. ������O
	Dlg.m_ofn.lpstrInitialDir = szPath;
	
	// DoModal: ��ȭ���ڸ� ����. IDOK Ȥ�� IDCANCEL�� ��ȯ.
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName: ��ȭ���ڿ��� ����ڰ� ������ ���� ��� �� ���� ���� ����.
		CString strGetPath = Dlg.GetPathName();
		const TCHAR* pGetPath = strGetPath.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"UnitTool Save Failed!!");
			return;
		}

		DWORD dwByte = 0;

	/*	for (auto& MyPair : m_MapData)
		{
			const TCHAR* pName = MyPair.second->strName.GetString();
			int iLength = MyPair.second->strName.GetLength() + 1;

			WriteFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, pName, sizeof(TCHAR) * iLength, &dwByte, nullptr);
			WriteFile(hFile, &MyPair.second->iAtt, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &MyPair.second->iDef, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &MyPair.second->byJobIndex, sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &MyPair.second->byItem, sizeof(BYTE), &dwByte, nullptr);
		}*/

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";
	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	// �⺻ ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szPath;

	//if (IDOK == Dlg.DoModal())
	//{
	//	// �ҷ����� ���� ������ map�� ����.
	//	for_each(m_MapData.begin(), m_MapData.end(), 
	//		[](auto& MyPair)
	//	{
	//		SafeDelete(MyPair.second);
	//	});

	//	m_MapData.clear();

	//	// �ҷ����� ���� ����Ʈ�ڽ� ��ϵ� ����.
	//	m_ListBox.ResetContent();

	//	CString strGetName = Dlg.GetPathName();
	//	HANDLE hFile = CreateFile(strGetName.GetString(), GENERIC_READ, 0, 0,
	//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	//	if (INVALID_HANDLE_VALUE == hFile)
	//	{
	//		AfxMessageBox(L"UnitTool Load Failed!!");
	//		return;
	//	}

	//	int iLength = 0;
	//	DWORD dwByte = 0;
	//	TCHAR* pName = nullptr;
	//	int iAtt = 0, iDef = 0;
	//	BYTE byJobIndex = 0;
	//	BYTE byItem = 0;

	//	UNIT_DATA* pData = nullptr;

	//	while (true)
	//	{
	//		ReadFile(hFile, &iLength, sizeof(int), &dwByte, nullptr);

	//		pName = new TCHAR[iLength];
	//		ReadFile(hFile, pName, sizeof(TCHAR) * iLength, &dwByte, nullptr);
	//		ReadFile(hFile, &iAtt, sizeof(int), &dwByte, nullptr);
	//		ReadFile(hFile, &iDef, sizeof(int), &dwByte, nullptr);
	//		ReadFile(hFile, &byJobIndex, sizeof(BYTE), &dwByte, nullptr);
	//		ReadFile(hFile, &byItem, sizeof(BYTE), &dwByte, nullptr);

	//		if (0 == dwByte)
	//		{
	//			if (pName)
	//			{
	//				delete[] pName;
	//				pName = nullptr;
	//			}

	//			break;
	//		}

	//		pData = new UNIT_DATA;
	//		pData->strName = pName;
	//		delete[] pName;
	//		pName = nullptr;

	//		pData->iAtt = iAtt;
	//		pData->iDef = iDef;
	//		pData->byJobIndex = byJobIndex;
	//		pData->byItem = byItem;

	//		m_MapData.insert({ pData->strName, pData });
	//		m_ListBox.AddString(pData->strName);
	//	}

	//	CloseHandle(hFile);
	//}
	//

	UpdateData(FALSE);
}

void CUnitTool::Release()
{
	for_each(m_MapData.begin(), m_MapData.end(),
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});

	m_MapData.clear();

	DeleteObject(m_hBitmap);
}