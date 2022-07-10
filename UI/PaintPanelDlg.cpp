// PaintPanelDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "PaintPanelDlg.h"
#include "PaintLayer.h"
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
	ON_BN_CLICKED(IDC_BUTTON_ADDLAYER, &PaintPanelDlg::OnBnClickedButtonAddlayer)
END_MESSAGE_MAP()


// PaintPanelDlg message handlers
void PaintPanelDlg::OnBnClickedButtonAddlayer()
{
	// TODO: Add your control notification handler code here
	m_PaintLayers.emplace_back(new PaintLayer(this));
	m_PaintLayers.back()->Create(IDD_DIALOG_PAINT_LAYER, this);
	m_PaintLayers.back()->OnInitDialog();
	CRect tabRect;
	this->GetClientRect(tabRect);
	tabRect.left	+= 20;
	tabRect.top		+= 50;
	CRect layerRect;
	m_PaintLayers.front()->GetClientRect(layerRect);
	for (int layeri = 0; layeri < m_PaintLayers.size(); layeri++)
	{
		layerRect.top		= tabRect.top + layeri * 100;
		layerRect.bottom	= layerRect.top + 100;
		layerRect.left		= tabRect.left;
		layerRect.right		= tabRect.right;

		//m_PaintLayers[layeri]->SetWindowPos(nullptr, layerRect.left, layerRect.top, layerRect.right, layerRect.bottom, 0);
		m_PaintLayers[layeri]->MoveWindow(layerRect, 1);
	}
}
