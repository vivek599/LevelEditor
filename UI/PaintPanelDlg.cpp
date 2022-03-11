// PaintPanelDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
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

BOOL PaintPanelDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == this->m_hWnd && pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
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
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &PaintPanelDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// PaintPanelDlg message handlers


void PaintPanelDlg::OnBnClickedButtonTest()
{
	// TODO: Add your control notification handler code here
	//MessageBox(_T("Help, Something went wrong."), _T("Error"), MB_ICONERROR | MB_OK);
}
