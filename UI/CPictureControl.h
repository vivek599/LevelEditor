#pragma once


// CPictureControl
#include "framework.h"

class CPictureControl : public CStatic
{
	DECLARE_DYNAMIC(CPictureControl)

public:
	CPictureControl();
	virtual ~CPictureControl();

	virtual BOOL OnEraseBkgnd(CDC* pDC);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
};


