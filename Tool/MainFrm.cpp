
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "MyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

// OnCreate: WM_CREATE(초기화)메시지에 의해 호출되는 함수.
// WM_CREATE 메시지: CMainFrame객체가 생성됬을 때 발생하는 메시지.
// OnCreate 함수에서 도구바, 상태모음이 만들어진다.
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


// OnCreateClient: View 객체를 생성하는 시점에 호출되는 함수.
// OnCreateClient함수는 사용자가 재정의하여 뷰 생성을 제한하거나 자신 만의 뷰를 생성할 수 있게 해준다.
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 창 3개 분할
	m_MainSplit.CreateStatic(this, 1, 2);
	m_MainSplit.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	// SetColumnInfo(지정할 열, 열의 크기, 허용 가능한 최소 크기)
	m_MainSplit.SetColumnInfo(0, 300, 10);

	// IdFromRowCol(행, 열): 지정한 행과 열에 대한 창의 ID값을 반환.
	m_SecondSplit.CreateStatic(&m_MainSplit, 2, 1, WS_CHILD | WS_VISIBLE, m_MainSplit.IdFromRowCol(0, 0));
	m_SecondSplit.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	m_SecondSplit.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);



	///////////////////////////////////////////////////////////////////////////////////////////
	//// 창 4개 분할.
	//m_MainSplit.CreateStatic(this, 2, 2);

	//m_MainSplit.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	//m_MainSplit.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(WINCX, 300), pContext);
	//m_MainSplit.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY), pContext);	
	//m_MainSplit.CreateView(1, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);





	///////////////////////////////////////////////////////////////////////////////////////////
	//// 창 2개 분할.
	//// CreateStatic: 분할 윈도우를 생성하여 윈도우 영역을 나눈다.
	//m_MainSplit.CreateStatic(this, 1, 2);

	//// CreateView: 분할한 영역에 View객체를 생성하여 배치하는 함수.
	//m_MainSplit.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY), pContext);
	//m_MainSplit.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	return TRUE; //CFrameWnd::OnCreateClient(lpcs, pContext);
}
