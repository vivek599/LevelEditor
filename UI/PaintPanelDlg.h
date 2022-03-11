#pragma once


// PaintPanelDlg dialog

class PaintPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PaintPanelDlg)

public:
	PaintPanelDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaintPanelDlg();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	bool InitializeControls();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
