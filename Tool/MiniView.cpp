// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "ToolView.h"
#include "Terrain.h"
//#include "Wall.h"
//#include "Door.h"
//#include "Deco.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	CDevice::GetInstance()->Render_Begin();


	// CMainFrame을 얻어오는 3가지 방법.
	// 1. AfxGetMainWnd: 현재 쓰레드로부터의 Wnd를 반환한다.
	// 현재 쓰레드가 main쓰레드라면 CMainFrame의 Wnd를 반환한다.
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// 2. GetParentFrame: 현재 윈도우를 감싸고 있는 Frame을 반환한다.
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());

	// 3. AfxGetApp: 현재 메인 App을 반환한다.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());	
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplit.GetPane(0, 1));

	CTerrain* pTerrain = pMainView->GetTerrain();
	pTerrain->MiniRender();

	/*CWall*	pWall = pMainView->GetWall();
	pWall->MiniRender();

	CDoor*	pDoor = pMainView->GetDoor();
	pDoor->MiniRender();

	CDeco*	pDeco = pMainView->GetDeco();
	pDeco->MiniRender();*/

	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.
