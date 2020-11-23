// EnemyTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "EnemyTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"

// CEnemyTool 대화 상자입니다.

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


// CEnemyTool 메시지 처리기입니다.


BOOL CEnemyTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Radio[0].SetCheck(TRUE);
	m_eCurID = WEAPONID::RIFLE;
	m_ePreID = WEAPONID::RIFLE;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEnemyTool::OnBnClickedRifle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		// EnemyEdit 멀티 텍스쳐
		CString strRelativePath = CTextureMgr::GetInstance()->GetTexture(szStateKey,szStateKey,i)->szImagePath;

		// 경로에서 파일명만 남기기.
		// PathFindFileName: 인자로 받은 경로에서 파일명만 추출해주는 함수.
		CString strFileName = PathFindFileName(strRelativePath.GetString());

		// 파일명에서 확장자 제거.
		// PathRemoveExtension: 현재 파일명에서 확장자를 제거해주는 함수.
		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		// strFileName을 map의 key로 사용할 것!
		strFileName = szFileName;

		auto iter_find = m_MapImg.find(strFileName);

		// 해당 키가 존재하지 않을 때 CImage를 map에 새로 추가한다.
		if (m_MapImg.end() == iter_find)
		{
			CImage* pImage = new CImage;
			pImage->Load(strRelativePath); // 상대경로로부터 이미지를 로드.

			m_MapImg.insert({ strFileName, pImage });
		}
		UpdateData(FALSE);
	}

	// 픽쳐 컨트롤에 띄우기
	CString strSelectName = GetStateKey(m_eCurID);
	auto iter_find = m_MapImg.find(strSelectName);

	if (m_MapImg.end() == iter_find)
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
	pMainView->m_bIsEnemyOn = true;
	//~추가

	UpdateData(FALSE);


}


void CEnemyTool::OnBnClickedSMG()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEnemyTool::OnBnClickedShotGun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEnemyTool::OnBnClickedClub()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEnemyTool::OnBnClickedBat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eCurID = WEAPONID::BAT;

	if (m_eCurID == m_ePreID)
		return;
	m_ePreID = m_eCurID;

	const TCHAR* szStateKey = GetStateKey(m_eCurID);

	int iMax = CTextureMgr::GetInstance()->GetTexture(szStateKey)->iCount;
	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iMax; ++i)
	{
		// EnemyEdit 멀티 텍스쳐
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

		auto iter_find = m_MapImg.find(strFileName);

		// 해당 키가 존재하지 않을 때 CImage를 map에 새로 추가한다.
		if (m_MapImg.end() == iter_find)
		{
			CImage* pImage = new CImage;
			pImage->Load(strRelativePath); // 상대경로로부터 이미지를 로드.

			m_MapImg.insert({ strFileName, pImage });
		}
		UpdateData(FALSE);
	}

	// 픽쳐 컨트롤에 띄우기
	CString strSelectName = GetStateKey(m_eCurID);
	auto iter_find = m_MapImg.find(strSelectName);

	if (m_MapImg.end() == iter_find)
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
	pMainView->m_bIsEnemyOn = true;
	//~추가

	UpdateData(FALSE);
}


void CEnemyTool::OnBnClickedKnife()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEnemyTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEnemyTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
