#pragma once
#include "afxwin.h"


// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLbnSelectTileList();
	//afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnCbnSelchangeTileType();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedSetback();

public:
	////////////////////////////////// user
	void HorizontalScroll();
	void Release();
	void SetTileLst();
	//void SetMapID(const TCHAR* StateKey); // �̰ɷ� �ȵ�
public:
	const TCHAR* GetStateKey(MAPID eID);
	MAPID GetMapID() { return m_eCurID; }

public:
	////////////////////////////////// control
	CListBox	m_ListBox;
	CStatic		m_PictureCtrl;
	CComboBox	m_ComboBox;
	CButton		m_Radio[MAPID::MAP_END];



public:
	////////////////////////////////// value
	float m_fScaleX;
	float m_fScaleY;

public:
	////////////////////////////////// user
	// CImage: jpg, gif, bmp, png ���ĵ��� �����ϴ� MFC�� �̹��� Ŭ����.
	map<CString, CImage*>	m_MapImg[MAPID::MAP_END];
	int						m_iDrawID = 0;

	MAPID	m_eCurID;
	MAPID	m_ePreID;
	
	
	
	afx_msg void OnEnChangeScaleX();
	afx_msg void OnEnChangeScaleY();
};
