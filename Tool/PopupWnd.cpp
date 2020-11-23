// PopupWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PopupWnd.h"
#include "afxdialogex.h"


// CPopupWnd 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPopupWnd, CDialog)

CPopupWnd::CPopupWnd(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_POPUPWND, pParent)
{

}

CPopupWnd::~CPopupWnd()
{
	SafeDelete(m_pMySheet);
}

void CPopupWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopupWnd, CDialog)
END_MESSAGE_MAP()


// CPopupWnd 메시지 처리기입니다.


BOOL CPopupWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pMySheet = new CMySheet;

	// Create: 모달리스 형식으로 생성.
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, 500, 400); // 현재 윈도우의 위치와 크기 조정.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
