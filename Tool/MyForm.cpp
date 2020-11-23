// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedUnitTool)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnBnClickedPopUp)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnBnClickedPathFind)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyForm::OnBnClickedEnemyTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// CreatePointFont(폰트 크기, 글씨체)
	m_Font.CreatePointFont(180, L"궁서");

	// GetDlgItem: 현재 다이얼로그에 배치된 오브젝트를 얻어오는 함수.
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON6)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_Font);

}


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//AfxMessageBox(L"Hello world");

	// GetSafeHwnd: 현재 다이얼로그의 윈도우를 반환.
	// nullptr을 반환한다면 아직 다이얼로그는 생성되지 않음.
	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);	// 해당 ID에 맞는 다이얼로그가 생성된다.

	m_UnitTool.ShowWindow(SW_SHOW);		// 창 모양으로 출력
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplit.GetPane(0, 1));

	pMainView->m_bIsTerrainOn = true;*/

	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);	// 해당 ID에 맞는 다이얼로그가 생성된다.

	m_MapTool.ShowWindow(SW_SHOW);		// 창 모양으로 출력
}


void CMyForm::OnBnClickedPopUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_PopupWnd.GetSafeHwnd())
		m_PopupWnd.Create(IDD_POPUPWND);	// 해당 ID에 맞는 다이얼로그가 생성된다.

	m_PopupWnd.ShowWindow(SW_SHOW);		// 창 모양으로 출력
}


void CMyForm::OnBnClickedPathFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_Pathfind.GetSafeHwnd())
		m_Pathfind.Create(IDD_PATHFIND);	// 해당 ID에 맞는 다이얼로그가 생성된다.

	m_Pathfind.ShowWindow(SW_SHOW);		// 창 모양으로 출력
}


void CMyForm::OnBnClickedEnemyTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_EnemyTool.GetSafeHwnd())
		m_EnemyTool.Create(IDD_ENEMYTOOL);	// 해당 ID에 맞는 다이얼로그가 생성된다.

	m_EnemyTool.ShowWindow(SW_SHOW);		// 창 모양으로 출력

}
