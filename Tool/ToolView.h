
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once


class CTerrain;
//class CWall;
//class CDoor;
//class CDeco;
//class CWeapon;
class CToolDoc;
class CToolView : public CScrollView // CScrollView: 스크롤바 형태를 갖는 윈도우의 자식 클래스.
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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
	bool m_bIsEnemyOn; // 나중에 유닛툴 추가하면
	// 이미지 크기
	float	m_fScaleX;
	float	m_fScaleY;

	/*마우스 좌표*/
	CPoint		m_Mouse;
	/*마우스 위치에 그려질 이미지 Key값.*/
	const TCHAR*	m_ObjKey;
	const TCHAR*	m_StateKey;
	/*이미지 Draw될 index & 알파값.*/
	int	m_iIndex;
	int	m_iAlpha;

	// 적 정보
	UNIT_DATA	m_tEnemy;
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

