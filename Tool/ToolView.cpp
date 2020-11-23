
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
	: m_pTerrain(nullptr)/*,m_pWall(nullptr),m_pDoor(nullptr),m_pDeco(nullptr),m_pWeapon(nullptr)*/,m_bIsTerrainOn(false),m_bIsEnemyOn(false)
	,m_fScaleX(1.f),m_fScaleY(1.f),m_ObjKey(L"Floor"),m_StateKey(L"Floor"), m_iIndex(1), m_iAlpha(0)
	
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
	// ���� ����.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;	

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CDevice::GetInstance()->Render_Begin();


	m_pTerrain->Render();
	/*m_pWall->Render();
	m_pDoor->Render();
	m_pDeco->Render();
	m_pWeapon->Render();*/

	/*TCHAR szIndex[MIN_STR] = L"";*/

	// ���콺�� �̹��� ������ �ǰ�
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

	// ���ڼ� �׸���

}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes: ��ũ�� ���� ũ�⸦ �����ϴ� CScrollView�� ����Լ�.
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY /*/ 2*/));


	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;

	// 1.���� ������ ũ�� ����
	// AfxGetMainWnd: ���� ���� �����츦 ��ȯ���ִ� MFC�� �����Լ��̴�.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// 1-1. ���� �������� ũ�� ������ ���´�.
	// GetWindowRect: ���� �������� ũ��(RECT)������ ������ �Լ�.
	RECT rcMainWnd = {};
	pMainFrm->GetWindowRect(&rcMainWnd);
	
	// 1-2. ���� rcWnd�� �����ϱ� �����ϰ� �»���� 0, 0�� ��ġ�� ����.
	SetRect(&rcMainWnd, 0, 0, rcMainWnd.right - rcMainWnd.left, rcMainWnd.bottom - rcMainWnd.top);

	// 1-3. View �������� RECT�� ���´�.
	RECT rcView = {};

	// GetClientRect: ���� �������� RECT�� ������ �Լ�.
	GetClientRect(&rcView);

	// 1-4. ���� �����Ӱ� View�� ����, ���� ���� ���Ѵ�.
	int iRowFrm = rcMainWnd.right - rcView.right;
	int iColFrm = rcMainWnd.bottom - rcView.bottom;

	// 1-5. ���� �����ӿ� WINCX + iRowFrm, WINCY + iColFrm ũ��� �����Ѵ�.
	// SetWindowPos: �μ���� �������� ��ġ�� ũ�⸦ �����ϴ� �Լ�.
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


// OnLButtonDown: ���콺 ��Ŭ�� ���� �� ȣ��Ǵ� �Լ�.
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnLButtonDown(nFlags, point);

	// �̴Ϻ並 ����
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplit.GetPane(1, 0));
	
	int iDrawID = pMyForm->m_MapTool.m_iDrawID;

	// GetScrollPos: CScrollView�� ����Լ�.
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
			//// ���������� ���콺 ��ġ�� ���� ������ vPos�� ����
			//pMyForm->m_EnemyTool.m_Monster.vPos = { (float)point.x, (float)point.y, 0.f };
			//// �����͸� vector�� push_back
			//m_pTerrain->m_vecMonster.push_back(new UNIT_DATA(pMyForm->m_MonTool.m_Monster));

			m_pTerrain->AddEnemy(D3DXVECTOR3({ (float)point.x, (float)point.y, 0.f }), m_tEnemy, pMyForm->m_EnemyTool.GetWeaponID());
		}
	}

	// Invalidate: WM_PAINT�� WM_ERASEBKGND �޽����� �߻�.
	Invalidate(FALSE);

	
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplit.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);
}

// OnMouseMove: ���콺�� �����̴� ���� ȣ��Ǵ� �Լ�.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

			// GetScrollPos: CScrollView�� ����Լ�.
			/*point.x += GetScrollPos(0);
			point.y += GetScrollPos(1);*/

			SelectMapID((float)point.x, (float)point.y);

			Invalidate(FALSE);

		}
	}
	// Invalidate: WM_PAINT�� WM_ERASEBKGND �޽����� �߻�.
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

	// �߰�~
	Invalidate(FALSE);

	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplit.GetPane(0, 0));
	pMiniView->Invalidate(FALSE); 
	// ~�߰�
}
