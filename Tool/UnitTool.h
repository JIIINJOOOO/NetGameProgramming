#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLbnSelectClass();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:
	void Release();

public:
	//////////////////////////// control
	CButton m_Radio[6];


	// 비트맵 입혀볼거임
	CButton		m_PushBtn;

public:
	//////////////////////////// value
	
	// 무기
	CString m_strType;


public:
	//////////////////////////// user
	map<CString, UNIT_DATA*>	m_MapData;	
	HBITMAP						m_hBitmap;	
};
