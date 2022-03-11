// PaintPanelDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "LevelEditorDlg.h"
#include "PaintPanelDlg.h"
#include "afxdialogex.h"


// PaintPanelDlg dialog

IMPLEMENT_DYNAMIC(PaintPanelDlg, CDialogEx)

PaintPanelDlg::PaintPanelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAINT, pParent)
{

}

PaintPanelDlg::~PaintPanelDlg()
{

}

void PaintPanelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);





}

bool PaintPanelDlg::InitializeControls()
{






	return true;
}

void PaintPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PaintPanelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PaintPanelDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// PaintPanelDlg message handlers


void PaintPanelDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	MessageBoxA(nullptr, "", "", MB_OK);
}
