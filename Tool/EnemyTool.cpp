// EnemyTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "EnemyTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"

// CEnemyTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEnemyTool, CDialog)

CEnemyTool::CEnemyTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ENEMYTOOL, pParent)
{

}

CEnemyTool::~CEnemyTool()
{
}

void CEnemyTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_RADIO4, m_Radio[3]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio[4]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio[5]);

	DDX_Control(pDX, IDC_PICTURE2, m_PictureCtrl);
}

void CEnemyTool::Release()
{
}


BEGIN_MESSAGE_MAP(CEnemyTool, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CEnemyTool::OnBnClickedRifle)
	ON_BN_CLICKED(IDC_RADIO2, &CEnemyTool::OnBnClickedSMG)
	ON_BN_CLICKED(IDC_RADIO3, &CEnemyTool::OnBnClickedShotGun)
	ON_BN_CLICKED(IDC_RADIO4, &CEnemyTool::OnBnClickedClub)
	ON_BN_CLICKED(IDC_RADIO5, &CEnemyTool::OnBnClickedBat)
	ON_BN_CLICKED(IDC_RADIO6, &CEnemyTool::OnBnClickedKnife)
	ON_BN_CLICKED(IDC_BUTTON4, &CEnemyTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CEnemyTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CEnemyTool �޽��� ó�����Դϴ�.


BOOL CEnemyTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_Radio[0].SetCheck(TRUE);
	m_eCurID = WEAPONID::RIFLE;
	m_ePreID = WEAPONID::RIFLE;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CEnemyTool::OnBnClickedRifle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_eCurID = WEAPONID::RIFLE;

	if (m_eCurID == m_ePreID)
		return;
	m_ePreID = m_eCurID;

	const TCHAR* szStateKey = GetStateKey(m_eCurID);

	int iMax = CTextureMgr::GetInstance()->GetTexture(szStateKey, szStateKey, 0)->iCount;
	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iMax; ++i)
	{
		// EnemyEdit ��Ƽ �ؽ���
		CString strRelativePath = CTextureMgr::GetInstance()->GetTexture(szStateKey,szStateKey,i)->szImagePath;

		// ��ο��� ���ϸ� �����.
		// PathFindFileName: ���ڷ� ���� ��ο��� ���ϸ� �������ִ� �Լ�.
		CString strFileName = PathFindFileName(strRelativePath.GetString());

		// ���ϸ��� Ȯ���� ����.
		// PathRemoveExtension: ���� ���ϸ��� Ȯ���ڸ� �������ִ� �Լ�.
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		// strFileName�� map�� key�� ����� ��!
		strFileName = szFileName;

		auto iter_find = m_MapImg.find(strFileName);

		// �ش� Ű�� �������� ���� �� CImage�� map�� ���� �߰��Ѵ�.
		if (m_MapImg.end() == iter_find)
		{
			CImage* pImage = new CImage;
			pImage->Load(strRelativePath); // ����ηκ��� �̹����� �ε�.

			m_MapImg.insert({ strFileName, pImage });
		}
		UpdateData(FALSE);
	}

	// ���� ��Ʈ�ѿ� ����
	CString strSelectName = GetStateKey(m_eCurID);
	auto iter_find = m_MapImg.find(strSelectName);

	if (m_MapImg.end() == iter_find)
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
	pMainView->m_bIsEnemyOn = true;
	//~�߰�

	UpdateData(FALSE);


}


void CEnemyTool::OnBnClickedSMG()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEnemyTool::OnBnClickedShotGun()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEnemyTool::OnBnClickedClub()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEnemyTool::OnBnClickedBat()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_eCurID = WEAPONID::BAT;

	if (m_eCurID == m_ePreID)
		return;
	m_ePreID = m_eCurID;

	const TCHAR* szStateKey = GetStateKey(m_eCurID);

	int iMax = CTextureMgr::GetInstance()->GetTexture(szStateKey)->iCount;
	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iMax; ++i)
	{
		// EnemyEdit ��Ƽ �ؽ���
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

		auto iter_find = m_MapImg.find(strFileName);

		// �ش� Ű�� �������� ���� �� CImage�� map�� ���� �߰��Ѵ�.
		if (m_MapImg.end() == iter_find)
		{
			CImage* pImage = new CImage;
			pImage->Load(strRelativePath); // ����ηκ��� �̹����� �ε�.

			m_MapImg.insert({ strFileName, pImage });
		}
		UpdateData(FALSE);
	}

	// ���� ��Ʈ�ѿ� ����
	CString strSelectName = GetStateKey(m_eCurID);
	auto iter_find = m_MapImg.find(strSelectName);

	if (m_MapImg.end() == iter_find)
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
	pMainView->m_bIsEnemyOn = true;
	//~�߰�

	UpdateData(FALSE);
}


void CEnemyTool::OnBnClickedKnife()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEnemyTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEnemyTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

const TCHAR * CEnemyTool::GetStateKey(WEAPONID eID)
{
	switch (eID)
	{
	case RIFLE:
		return L"Rifle";
		break;
	case SMG:
		return L"Smg";
		break;
	case SHOTGUN:
		return L"Shotgun";
		break;
	case CLUB:
		return L"Club";
		break;
	case BAT:
		return L"Bat";
		break;
	case KNIFE:
		return L"Knife";
		break;

	default:
		break;
	}
	return nullptr;
}
