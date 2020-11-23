
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "MiniView.h"
#include "Terrain.h"
//#include "Wall.h"
//#include "Door.h"
//#include "Deco.h"
//#include "Weapon.h"
#include "MyForm.h"
#include "MapTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pTerrain(nullptr)/*,m_pWall(nullptr),m_pDoor(nullptr),m_pDeco(nullptr),m_pWeapon(nullptr)*/,m_bIsTerrainOn(false),m_bIsEnemyOn(false)
	,m_fScaleX(1.f),m_fScaleY(1.f),m_ObjKey(L"Floor"),m_StateKey(L"Floor"), m_iIndex(1), m_iAlpha(0)
	
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	// 순서 주의.
	//SafeDelete(m_pWeapon);
	//SafeDelete(m_pDeco);
	//SafeDelete(m_pDoor);
	//SafeDelete(m_pWall);
	SafeDelete(m_pTerrain);
	CTextureMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;	

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CDevice::GetInstance()->Render_Begin();


	m_pTerrain->Render();
	/*m_pWall->Render();
	m_pDoor->Render();
	m_pDeco->Render();
	m_pWeapon->Render();*/

	/*TCHAR szIndex[MIN_STR] = L"";*/

	// 마우스에 이미지 렌더링 되게
	D3DXMATRIX matWorld, matScale, matTrans;

	D3DXMatrixIdentity(&matWorld);
	if(m_ObjKey == L"Deco")
		D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_Mouse.x - (FLOAT)GetScrollPos(0),
		m_Mouse.y - (FLOAT)GetScrollPos(1),
		0.f);

	D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_ObjKey, m_StateKey, m_iIndex);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));


	CDevice::GetInstance()->Render_End();

	// 격자선 그리기

}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes: 스크롤 바의 크기를 지정하는 CScrollView의 멤버함수.
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY /*/ 2*/));


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;

	// 1.메인 프레임 크기 보정
	// AfxGetMainWnd: 현재 메인 윈도우를 반환해주는 MFC의 전역함수이다.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// 1-1. 메인 프레임의 크기 정보를 얻어온다.
	// GetWindowRect: 현재 윈도우의 크기(RECT)정보를 얻어오는 함수.
	RECT rcMainWnd = {};
	pMainFrm->GetWindowRect(&rcMainWnd);
	
	// 1-2. 얻어온 rcWnd를 연산하기 수월하게 좌상단을 0, 0의 위치로 설정.
	SetRect(&rcMainWnd, 0, 0, rcMainWnd.right - rcMainWnd.left, rcMainWnd.bottom - rcMainWnd.top);

	// 1-3. View 윈도우의 RECT를 얻어온다.
	RECT rcView = {};

	// GetClientRect: 현재 윈도우의 RECT를 얻어오는 함수.
	GetClientRect(&rcView);

	// 1-4. 메인 프레임과 View의 가로, 세로 갭을 구한다.
	int iRowFrm = rcMainWnd.right - rcView.right;
	int iColFrm = rcMainWnd.bottom - rcView.bottom;

	// 1-5. 메인 프레임에 WINCX + iRowFrm, WINCY + iColFrm 크기로 설정한다.
	// SetWindowPos: 인수대로 윈도우의 위치와 크기를 조정하는 함수.
	pMainFrm->SetWindowPos(nullptr, 0, 0, WINCX + iRowFrm, WINCY + iColFrm, SWP_NOZORDER);	

	if (FAILED(CDevice::GetInstance()->InitDevice()))
	{
		AfxMessageBox(L"Device Init Failed!!!");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->SetMainView(this);

	/*m_pWall = new CWall;
	m_pWall->Initialize();
	m_pWall->SetMainView(this);

	m_pDoor = new CDoor;
	m_pDoor->Initialize();
	m_pDoor->SetMainView(this);

	m_pDeco = new CDeco;
	m_pDeco->Initialize();
	m_pDeco->SetMainView(this);

	m_pWeapon = new CWeapon;
	m_pWeapon->Initialize();
	m_pWeapon->SetMainView(this);*/
}


// OnLButtonDown: 마우스 좌클릭 했을 때 호출되는 함수.
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnLButtonDown(nFlags, point);

	// 미니뷰를 갱신
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplit.GetPane(1, 0));
	
	int iDrawID = pMyForm->m_MapTool.m_iDrawID;

	// GetScrollPos: CScrollView의 멤버함수.
	point.x += GetScrollPos(0);
	point.y += GetScrollPos(1);

	//m_pTerrain->TileChange(D3DXVECTOR3((float)point.x, (float)point.y, 0.f), iDrawID, 1);
	if (m_bIsTerrainOn)
	{
		SelectMapID((float)point.x, (float)point.y);
	}


	if (m_bIsEnemyOn)
	{
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplit.GetPane(1, 0));

		if (pMyForm->m_EnemyTool)
		{
			//// 최종적으로 마우스 위치를 몬스터 정보의 vPos에 대입
			//pMyForm->m_EnemyTool.m_Monster.vPos = { (float)point.x, (float)point.y, 0.f };
			//// 데이터를 vector에 push_back
			//m_pTerrain->m_vecMonster.push_back(new UNIT_DATA(pMyForm->m_MonTool.m_Monster));

			m_pTerrain->AddEnemy(D3DXVECTOR3({ (float)point.x, (float)point.y, 0.f }), m_tEnemy, pMyForm->m_EnemyTool.GetWeaponID());
		}
	}

	// Invalidate: WM_PAINT와 WM_ERASEBKGND 메시지를 발생.
	Invalidate(FALSE);

	
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplit.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);
}

// OnMouseMove: 마우스가 움직이는 동안 호출되는 함수.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnMouseMove(nFlags, point);

	point.x += (LONG)GetScrollPos(0);
	point.y += (LONG)GetScrollPos(1);

	m_Mouse = point;

	if (m_bIsTerrainOn == false && m_bIsEnemyOn == false )
		m_iAlpha = 0;
	else
		m_iAlpha = 200;

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrame->m_SecondSplit.GetPane(0, 0));

	if (m_bIsTerrainOn)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{

			// GetScrollPos: CScrollView의 멤버함수.
			/*point.x += GetScrollPos(0);
			point.y += GetScrollPos(1);*/

			SelectMapID((float)point.x, (float)point.y);

			Invalidate(FALSE);

		}
	}
	// Invalidate: WM_PAINT와 WM_ERASEBKGND 메시지를 발생.
	Invalidate(FALSE);
	pMiniView->Invalidate(FALSE);

}


void CToolView::SelectMapID(float x, float y)
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplit.GetPane(1, 0));

	int iDrawID = pMyForm->m_MapTool.m_iDrawID;
	MAPID eID = pMyForm->m_MapTool.GetMapID();
	int iIndex = 0;
	D3DXVECTOR3 vPos = {};

	switch (eID)
	{
	case FLOOR:
		m_pTerrain->TileChange(D3DXVECTOR3(x, y, 0.f), iDrawID, 0);
		break;
	case WALL:
		//m_pWall->TileChange(D3DXVECTOR3(x, y, 0.f), iDrawID, 0);
		iIndex = m_pTerrain->GetTileIndex(D3DXVECTOR3(float(x), float(y), 0.f));
		if (iIndex == -1)
			return;

		vPos = m_pTerrain->GetTilePos(iIndex);

		m_pTerrain->AddWall(vPos, m_iIndex, m_fScaleX, m_fScaleY,0);

		Invalidate(FALSE);
		break;
	case DOOR:
		//m_pDoor->TileChange(D3DXVECTOR3(x, y, 0.f), iDrawID, 0);
		iIndex = m_pTerrain->GetTileIndex(D3DXVECTOR3(float(x), float(y), 0.f));
		if (iIndex == -1)
			return;

		vPos = m_pTerrain->GetTilePos(iIndex);

		m_pTerrain->AddDoor(vPos, m_iIndex, m_fScaleX, m_fScaleY,0);

		Invalidate(FALSE);
		break;
	case DECO:
		//m_pDeco->TileChange(D3DXVECTOR3(x, y, 0.f), iDrawID, 1);
		iIndex = m_pTerrain->GetTileIndex(D3DXVECTOR3(float(x), float(y), 0.f));
		if (iIndex == -1)
			return;

		vPos = m_pTerrain->GetTilePos(iIndex);

		m_pTerrain->AddDeco(vPos, m_iIndex, m_fScaleX, m_fScaleY,0);

		Invalidate(FALSE);
		break;
	case WEAPON:
		//m_pWeapon->TileChange(D3DXVECTOR3(x, y, 0.f), iDrawID, 0);
		iIndex = m_pTerrain->GetTileIndex(D3DXVECTOR3(float(x), float(y), 0.f));
		if (iIndex == -1)
			return;

		vPos = m_pTerrain->GetTilePos(iIndex);

		m_pTerrain->AddWeapon(vPos, m_iIndex, m_fScaleX, m_fScaleY,0);

		Invalidate(FALSE);
		break;
	default:
		return;
	}

	// 추가~
	Invalidate(FALSE);

	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplit.GetPane(0, 0));
	pMiniView->Invalidate(FALSE); 
	// ~추가
}
