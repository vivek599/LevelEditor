// PaintLayer.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "PaintLayer.h"
#include "afxdialogex.h"


// PaintLayer dialog

IMPLEMENT_DYNAMIC(PaintLayer, CDialogEx)

PaintLayer::PaintLayer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAINT_LAYER, pParent)
{
	 
}

PaintLayer::~PaintLayer()
{

}

void PaintLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PaintLayer, CDialogEx)
	ON_BN_CLICKED(IDC_MOVE_LAYER_REMOVE, &PaintLayer::OnBnClickedMoveLayerRemove)
END_MESSAGE_MAP()


// PaintLayer message handlers


void PaintLayer::OnBnClickedMoveLayerRemove()
{
	// TODO: Add your control notification handler code here
	delete this;
}
