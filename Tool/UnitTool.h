#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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


	// ��Ʈ�� ����������
	CButton		m_PushBtn;

public:
	//////////////////////////// value
	
	// ����
	CString m_strType;


public:
	//////////////////////////// user
	map<CString, UNIT_DATA*>	m_MapData;	
	HBITMAP						m_hBitmap;	
};
