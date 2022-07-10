#pragma once


// PaintLayer dialog

class PaintLayer : public CDialogEx
{
	DECLARE_DYNAMIC(PaintLayer)

public:
	PaintLayer(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaintLayer();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAINT_LAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMoveLayerRemove();
};
