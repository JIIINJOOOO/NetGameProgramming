// ObjectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"


// CObjectTool ��ȭ �����Դϴ�.

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


// CObjectTool �޽��� ó�����Դϴ�.
