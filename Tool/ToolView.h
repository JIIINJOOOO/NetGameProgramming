
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once


class CTerrain;
//class CWall;
//class CDoor;
//class CDeco;
//class CWeapon;
class CToolDoc;
class CToolView : public CScrollView // CScrollView: ��ũ�ѹ� ���¸� ���� �������� �ڽ� Ŭ����.
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	CTerrain* GetTerrain() { return m_pTerrain; }
	//CWall*	GetWall() { return m_pWall; }
	//CDoor*	GetDoor() { return m_pDoor; }
	//CDeco*	GetDeco() { return m_pDeco; }
	//CWeapon* GetWeapon() { return m_pWeapon; }


private:
	void SelectMapID(float x, float y);


private:
	CTerrain*	m_pTerrain;	
	//CWall*		m_pWall;
	//CDoor*		m_pDoor;
	//CDeco*		m_pDeco;
	//CWeapon*	m_pWeapon;

public:
	bool m_bIsTerrainOn;
	bool m_bIsEnemyOn; // ���߿� ������ �߰��ϸ�
	// �̹��� ũ��
	float	m_fScaleX;
	float	m_fScaleY;

	/*���콺 ��ǥ*/
	CPoint		m_Mouse;
	/*���콺 ��ġ�� �׷��� �̹��� Key��.*/
	const TCHAR*	m_ObjKey;
	const TCHAR*	m_StateKey;
	/*�̹��� Draw�� index & ���İ�.*/
	int	m_iIndex;
	int	m_iAlpha;

	// �� ����
	UNIT_DATA	m_tEnemy;
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

