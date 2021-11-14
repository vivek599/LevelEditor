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
