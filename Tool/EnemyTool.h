#pragma once
#include "afxwin.h"


// CEnemyTool 대화 상자입니다.

class CEnemyTool : public CDialog
{
	DECLARE_DYNAMIC(CEnemyTool)

public:
	CEnemyTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEnemyTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENEMYTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRifle();
	afx_msg void OnBnClickedSMG();
	afx_msg void OnBnClickedShotGun();
	afx_msg void OnBnClickedClub();
	afx_msg void OnBnClickedBat();
	afx_msg void OnBnClickedKnife(); 
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
public:
	const TCHAR* GetStateKey(WEAPONID eID);
	WEAPONID GetWeaponID() { return m_eCurID; }

public:
	void Release();
public:
	////////////////////////////////// control
	CButton m_Radio[WEAPONID::WEAPON_END];
	CStatic m_PictureCtrl;
public:
	//////////////////////////// value
public:
	//////////////////////////// user
	map<CString, CImage*>	m_MapImg;
	int						m_iDrawID = 0;

	
	WEAPONID m_eCurID;
	WEAPONID m_ePreID;
	
	
};
