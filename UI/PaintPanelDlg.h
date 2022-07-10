#pragma once


// PaintPanelDlg dialog

class PaintPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PaintPanelDlg)

public:
	PaintPanelDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaintPanelDlg();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	bool InitializeControls();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	vector<unique_ptr<class PaintLayer>>	m_PaintLayers;
public:
	afx_msg void OnBnClickedButtonAddlayer();
};
