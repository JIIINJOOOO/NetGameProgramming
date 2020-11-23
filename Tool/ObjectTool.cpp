// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"


// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
{

}

CObjectTool::~CObjectTool()
{
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBox);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.
