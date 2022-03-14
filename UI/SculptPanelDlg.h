#pragma once


// SculptPanelDlg dialog

class SculptPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SculptPanelDlg)

public:
	SculptPanelDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SculptPanelDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCULPT };
#endif

	afx_msg void OnBnClickedLoadheightmap();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeBrushsizetextbox();
	afx_msg void OnCbnSelendokComboBrushtype();
	afx_msg void OnBnClickedButtonErode();
	afx_msg void OnChangeBrushstrengthtextbox();
	afx_msg void OnBnClickedLoadalphamap();
	afx_msg void OnBnClickedButtonNoiseRndmz();
	afx_msg void OnDeltaposSpinSeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeTextboxNscale();
	afx_msg void OnEnChangeTextboxNfreq();
	afx_msg void OnEnChangeTextboxNoctaves();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	bool InitializeControls();
	bool InitGraphic(class AGraphic* Graphic);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	bool				m_ControlsInitialized;
	bool				m_AlphaMapLoaded = false;
	class AGraphic*			m_Graphic;

	HICON m_hIconAlphaMap;

	CButton* m_ButtonLoadHeightMap;
	CButton* m_ButtonLoadAlphaMap;
	CButton* m_ButtonErode;
	CSliderCtrl* m_BrushSizeSlider;
	CEdit* m_BrushSizeTextbox;
	CSliderCtrl* m_BrushStrengthSlider;
	CEdit* m_BrushStrengthTextbox;
	CEdit* m_HeightMapFileName;
	CComboBox* m_BrushComboBox;
	CStatic* m_ErodeIterationText;
	CStatic* m_SliderSizeText;
	CStatic* m_SliderWeightText;
	CStatic* m_BrushTypeText;
	CFont				m_FpsFont;
	CString				m_BrushSizeSliderVal;
	CString				m_BrushStrengthSliderVal;

	CStatic* m_NoiseScaleText;
	CStatic* m_NoiseFreqText;
	CStatic* m_NoiseOctavesText;
	CStatic* m_NoiseSeedText;
	CButton* m_ButtonNoiseSeedRandomize;
	CEdit* m_NoiseScaleTextBox;
	CEdit* m_NoiseFreqTextBox;
	CEdit* m_NoiseOctavesTextBox;
	CEdit* m_NoiseSeedTextBox;
	CSpinButtonCtrl* m_NoiseSeedSpin;
	CSliderCtrl* m_NoiseScaleSlider;
	CSliderCtrl* m_NoiseFreqSlider;
	CSliderCtrl* m_NoiseOctavesSlider;
	CString				m_NoiseScaleSliderVal;
	CString				m_NoiseFreqSliderVal;
	CString				m_NoiseOctavesSliderVal;
	CString				m_NoiseSeedSpinVal; 
};
