// MiniView.cpp : ���� �����Դϴ�.
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


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CDevice::GetInstance()->Render_Begin();


	// CMainFrame�� ������ 3���� ���.
	// 1. AfxGetMainWnd: ���� ������κ����� Wnd�� ��ȯ�Ѵ�.
	// ���� �����尡 main�������� CMainFrame�� Wnd�� ��ȯ�Ѵ�.
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// 2. GetParentFrame: ���� �����츦 ���ΰ� �ִ� Frame�� ��ȯ�Ѵ�.
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());

	// 3. AfxGetApp: ���� ���� App�� ��ȯ�Ѵ�.
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


// CMiniView �����Դϴ�.

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


// CMiniView �޽��� ó�����Դϴ�.
