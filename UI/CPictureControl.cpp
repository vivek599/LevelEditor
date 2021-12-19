// CPictureControl.cpp : implementation file
//

#include "CPictureControl.h"


// CPictureControl

IMPLEMENT_DYNAMIC(CPictureControl, CStatic)

CPictureControl::CPictureControl()
{

}

CPictureControl::~CPictureControl()
{
}


BOOL CPictureControl::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

BEGIN_MESSAGE_MAP(CPictureControl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CPictureControl message handlers
void CPictureControl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	Invalidate();
	RedrawWindow();
	UpdateWindow();
	// TODO: Add your message handler code here
}

void CPictureControl::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CStatic::OnNcPaint() for painting messages
}

BOOL CPictureControl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN && GetSafeHwnd() == pMsg->hwnd)
	{
		CPoint point(pMsg->pt);
		ScreenToClient(&point);

		OnLButtonDown(pMsg->wParam, point);    //passes the coordinates of the clicked Point in the dialog box



	}

	return CStatic::PreTranslateMessage(pMsg);

}

void CPictureControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnLButtonDown(nFlags, point);
}
