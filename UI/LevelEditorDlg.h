
// LevelEditorDlg.h : header file
//

#pragma once

#include <memory>

class AGraphic;
using namespace std;

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
	HICON m_hIconAlphaMap;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool				m_ControlsInitialized;

	CMenu				m_Menu;
	CButton*			m_ButtonLoadHeightMap;
	CButton*			m_ButtonLoadAlphaMap;
	CButton*			m_ButtonErode;
	CSliderCtrl*		m_BrushSizeSlider;
	CEdit*				m_BrushSizeTextbox;
	CSliderCtrl*		m_BrushStrengthSlider;
	CEdit*				m_BrushStrengthTextbox;
	CEdit*				m_HeightMapFileName;
	CComboBox*			m_BrushComboBox;
	CStatic*			m_ErodeIterationText;
	CStatic*			m_SliderSizeText;
	CStatic*			m_SliderWeightText;
	CStatic*			m_BrushTypeText;
	class CPictureControl*			m_RenderBox;
	CStatic*			m_FpsText;
	CFont				m_FpsFont;
	CString				m_BrushSizeSliderVal;
	CString				m_BrushStrengthSliderVal;

	CStatic*			m_NoiseScaleText;
	CStatic*			m_NoiseFreqText;
	CStatic*			m_NoiseSeedText;
	CButton*			m_ButtonNoiseSeedRandomize;
	CEdit*				m_NoiseScaleTextBox;
	CEdit*				m_NoiseFreqTextBox;
	CEdit*				m_NoiseSeedTextBox;
	CSpinButtonCtrl*	m_NoiseSeedSpin;
	CSliderCtrl*		m_NoiseScaleSlider;
	CSliderCtrl*		m_NoiseFreqSlider;
	CString				m_NoiseScaleSliderVal;
	CString				m_NoiseFreqSliderVal;
	CString				m_NoiseSeedSpinVal;

	float				m_deltaTime;
	int					m_frameCounter;
	bool				m_AlphaMapLoaded = false;

	unique_ptr<AGraphic> m_Graphic;	
	
	struct MouseState
	{
		bool LeftDown;
		bool MiddleDown;
		bool RightDown;
		bool Draging;
	};

	MouseState				m_MouseState;

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

	void CheckMouseMovingState();
	uint64_t NewMouseMovingState = 0;
	uint64_t OldMouseMovingState = 0;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void SendMouseState(CPoint& point, const MouseState& state);
	CPoint pointOnRenderBox;

	afx_msg void OnChangeBrushstrengthtextbox();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnBnClickedLoadalphamap();
	afx_msg void OnBnClickedButtonNoiseRndmz();
	afx_msg void OnDeltaposSpinSeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeTextboxNscale();
	afx_msg void OnEnChangeTextboxNfreq();
};
