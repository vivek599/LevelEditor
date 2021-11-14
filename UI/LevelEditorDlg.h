
// LevelEditorDlg.h : header file
//

#pragma once


#define WM_IDLE WM_USER + 1

// CLevelEditorDlg dialog
class CLevelEditorDlg : public CDialogEx
{
// Construction
public:
	CLevelEditorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEVELEDITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnIdle(LONG lCount);

	void CalculateFps();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool				m_ControlsInitialized;

	CMenu				m_Menu;
	CButton*			m_ButtonLoadHeightMap;
	CButton*			m_ButtonErode;
	CSliderCtrl*		m_BrushSizeSlider;
	CEdit*				m_BrushSizeTextbox;
	CEdit*				m_HeightMapFileName;
	CComboBox*			m_BrushComboBox;
	CStatic*			m_ErodeIterationText;
	CStatic*			m_BrushTypeText;
	class CPictureControl*			m_RenderBox;
	CStatic*			m_FpsText;
	CFont				m_FpsFont;
	CString				m_BrushSizeSliderVal;

	float				m_deltaTime;
	int					m_frameCounter;
public:
	afx_msg void OnFileExit();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnHelpAbout();
	afx_msg void OnBnClickedLoadheightmap();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeBrushsizetextbox();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelendokComboBrushtype();
	afx_msg void OnBnClickedButtonErode();


	bool Update();
	bool Render();

private:
	bool InitializeControls();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
