// SculptPanelDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "SculptPanelDlg.h"
#include "afxdialogex.h"
#include "CPictureControl.h"
#include "../Graphics/ACommonIncludes.h"
#include "../Graphics/AGraphic.h"
#include "../Graphics/ATerrain.h"

// SculptPanelDlg dialog

IMPLEMENT_DYNAMIC(SculptPanelDlg, CDialogEx)

SculptPanelDlg::SculptPanelDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCULPT, pParent)
{
	m_ControlsInitialized = false;
	m_Graphic = nullptr;
}

SculptPanelDlg::~SculptPanelDlg()
{

}

BEGIN_MESSAGE_MAP(SculptPanelDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_CBN_SELENDOK(IDC_COMBO_BRUSHTYPE, &SculptPanelDlg::OnCbnSelendokComboBrushtype)
	ON_BN_CLICKED(IDC_LOADHEIGHTMAP, &SculptPanelDlg::OnBnClickedLoadheightmap)
	ON_BN_CLICKED(IDC_BUTTON_ERODE, &SculptPanelDlg::OnBnClickedButtonErode)
	ON_BN_CLICKED(IDC_LOADALPHAMAP, &SculptPanelDlg::OnBnClickedLoadalphamap)
	ON_BN_CLICKED(IDC_BUTTON_NOISE_RNDMZ, &SculptPanelDlg::OnBnClickedButtonNoiseRndmz)
	ON_EN_CHANGE(IDC_BRUSHSTRENGTHTEXTBOX, &SculptPanelDlg::OnChangeBrushstrengthtextbox)
	ON_EN_CHANGE(IDC_BRUSHSIZETEXTBOX, &SculptPanelDlg::OnEnChangeBrushsizetextbox)
	ON_EN_CHANGE(IDC_TEXTBOX_NSCALE, &SculptPanelDlg::OnEnChangeTextboxNscale)
	ON_EN_CHANGE(IDC_TEXTBOX_NFREQ, &SculptPanelDlg::OnEnChangeTextboxNfreq)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SEED, &SculptPanelDlg::OnDeltaposSpinSeed)
END_MESSAGE_MAP()


// SculptPanelDlg message handlers

bool SculptPanelDlg::InitializeControls()
{
	m_ErodeIterationText = (CStatic*)(GetDlgItem(IDC_STATIC_ITER));
	m_SliderSizeText = (CStatic*)(GetDlgItem(IDC_STATIC_SZ));
	m_SliderWeightText = (CStatic*)(GetDlgItem(IDC_STATIC_WT));
	m_ButtonLoadHeightMap = (CButton*)(GetDlgItem(IDC_LOADHEIGHTMAP));
	m_ButtonLoadAlphaMap = (CButton*)(GetDlgItem(IDC_LOADALPHAMAP));
	m_ButtonErode = (CButton*)(GetDlgItem(IDC_BUTTON_ERODE));
	m_BrushSizeSlider = (CSliderCtrl*)(GetDlgItem(IDC_BRUSHSIZESLIDER));
	m_BrushSizeTextbox = (CEdit*)GetDlgItem(IDC_BRUSHSIZETEXTBOX);
	m_BrushStrengthSlider = (CSliderCtrl*)(GetDlgItem(IDC_BRUSHSTRENGTHSLIDER));
	m_BrushStrengthTextbox = (CEdit*)GetDlgItem(IDC_BRUSHSTRENGTHTEXTBOX);
	m_BrushComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BRUSHTYPE);
	m_HeightMapFileName = (CEdit*)GetDlgItem(IDC_HMFILENAME);
	m_BrushTypeText = (CStatic*)GetDlgItem(IDC_STATIC_BRUSHTYPELEBAL); 

	m_NoiseScaleText = (CStatic*)GetDlgItem(IDC_STATIC_NSCALE);
	m_NoiseFreqText = (CStatic*)GetDlgItem(IDC_STATIC_NFREQ);
	m_NoiseSeedText = (CStatic*)GetDlgItem(IDC_STATIC_NSEED);
	m_ButtonNoiseSeedRandomize = (CButton*)(GetDlgItem(IDC_BUTTON_NOISE_RNDMZ));
	m_NoiseScaleTextBox = (CEdit*)GetDlgItem(IDC_TEXTBOX_NSCALE);;
	m_NoiseFreqTextBox = (CEdit*)GetDlgItem(IDC_TEXTBOX_NFREQ);;
	m_NoiseSeedTextBox = (CEdit*)GetDlgItem(IDC_TEXTBOX_NSEED);;
	m_NoiseSeedSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SEED);;
	m_NoiseScaleSlider = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_NSCALE));
	m_NoiseFreqSlider = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_NFREQ));

	m_BrushSizeSlider->SetRange(1, 100, TRUE);
	m_BrushSizeSlider->SetPos(0);
	m_BrushSizeSlider->SetPageSize(1);
	m_BrushSizeSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
	m_BrushSizeTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());

	m_BrushStrengthSlider->SetRange(1, 100, TRUE);
	m_BrushStrengthSlider->SetPos(0);
	m_BrushStrengthSlider->SetPageSize(1);
	m_BrushStrengthSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
	m_BrushStrengthTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());

	m_BrushComboBox->SetCurSel(0);

	m_NoiseScaleSlider->SetRange(1, 4096, TRUE);
	m_NoiseScaleSlider->SetPos(0);
	m_NoiseScaleSliderVal.Format(_T("%d"), m_NoiseScaleSlider->GetPos());
	m_NoiseScaleTextBox->SetWindowTextW(m_NoiseScaleSliderVal.GetBuffer());

	m_NoiseFreqSlider->SetRange(-1000, 1000, TRUE);
	m_NoiseFreqSlider->SetPos(200);
	m_NoiseFreqSlider->SetTicFreq(1);
	m_NoiseFreqSliderVal.Format(_T("%d"), m_NoiseFreqSlider->GetPos());
	m_NoiseFreqTextBox->SetWindowTextW(m_NoiseFreqSliderVal.GetBuffer());

	m_NoiseSeedSpin->SetRange(0, RAND_MAX);
	m_NoiseSeedSpin->SetPos(1337);
	m_NoiseSeedSpin->SetBuddy(m_NoiseSeedTextBox);
	CString windowtext;
	windowtext.Format(_T("%d"), 1337);
	m_NoiseSeedTextBox->SetWindowTextW(windowtext);

	m_FpsFont.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Agency FB"));                 // lpszFacename


	m_hIconAlphaMap = AfxGetApp()->LoadIcon(IDR_ALPHAMAPICON);

	m_ControlsInitialized = true;

	return true;
}

bool SculptPanelDlg::InitGraphic(AGraphic* Graphic)
{
	m_Graphic = Graphic;
	return true;
}

void SculptPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void SculptPanelDlg::OnBnClickedLoadheightmap()
{
	// TODO: Add your control notification handler code here

	CFileDialog dlgFile(TRUE);

	const int MAX_CFileDialog_FILE_COUNT = 99;
	const int FILE_LIST_BUFFER_SIZE = ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1);
	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);


	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.Flags |= OFN_PATHMUSTEXIST;
	ofn.Flags |= OFN_FILEMUSTEXIST;
	ofn.lpstrFile = p;
	ofn.nMaxFile = MAX_PATH + 1;
	dlgFile.DoModal();
	fileName.ReleaseBuffer();

	m_HeightMapFileName->SetWindowTextW(p);


	TerrainInitializationParams params;
	params.heightMap = p;
	params.pixelSHader = _T("../Data/Shaders/terrain_ps.hlsl");
	params.vertexSHader = _T("../Data/Shaders/terrain_vs.hlsl");
	params.sculptPixelSHader = _T("../Data/Shaders/terrain_ps_sculpt.hlsl");
	params.sculptVertexSHader = _T("../Data/Shaders/terrain_vs_sculpt.hlsl");
	params.textureLayers.push_back(_T("../Data/Textures/Grass0130_1.jpg"));

	m_Graphic->InitializeTerrain(&params);
	m_Graphic->SetTextureUVScale(1);
	m_Graphic->SetTerrainSculptMode(ESculptMode::RAISE);
	m_Graphic->SetSculptNoiseScale(1);
	m_Graphic->SetSculptNoiseFreq(200);
	m_Graphic->SetSculptNoiseSeed(1337);


}

void SculptPanelDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)m_BrushSizeSlider)
	{
		m_BrushSizeSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
		m_BrushSizeTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());
		UpdateData(FALSE);
		m_Graphic->SetSculptRadius(m_BrushSizeSlider->GetPos());
	}
	else if (pScrollBar == (CScrollBar*)m_BrushStrengthSlider)
	{
		m_BrushStrengthSliderVal.Format(_T("%d"), m_BrushStrengthSlider->GetPos());
		m_BrushStrengthTextbox->SetWindowTextW(m_BrushStrengthSliderVal.GetBuffer());
		UpdateData(FALSE);
		m_Graphic->SetSculptStrenght(m_BrushStrengthSlider->GetPos());
	}
	else if (pScrollBar == (CScrollBar*)m_NoiseScaleSlider)
	{
		m_NoiseScaleSliderVal.Format(_T("%d"), m_NoiseScaleSlider->GetPos());
		m_NoiseScaleTextBox->SetWindowTextW(m_NoiseScaleSliderVal.GetBuffer());
		UpdateData(FALSE);
		m_Graphic->SetSculptNoiseScale(m_NoiseScaleSlider->GetPos());
	}
	else if (pScrollBar == (CScrollBar*)m_NoiseFreqSlider)
	{
		m_NoiseFreqSliderVal.Format(_T("%d"), m_NoiseFreqSlider->GetPos());
		m_NoiseFreqTextBox->SetWindowTextW(m_NoiseFreqSliderVal.GetBuffer());
		UpdateData(FALSE);
		m_Graphic->SetSculptNoiseFreq(m_NoiseFreqSlider->GetPos());
	}
	else
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void SculptPanelDlg::OnEnChangeBrushsizetextbox()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString text;
	m_BrushSizeTextbox->GetWindowTextW(text);
	int value = StrToIntW(text.GetBuffer());
	if (value > 100)
	{
		value = 100;
	}
	else if (value < 0)
	{
		value = 0;
	}
	m_BrushSizeSlider->SetPos(value);

}

void SculptPanelDlg::OnCbnSelendokComboBrushtype()
{
	// TODO: Add your control notification handler code here
	//UpdateData();
	CString strValue;
	if (m_BrushComboBox)
	{
		int sel = m_BrushComboBox->GetCurSel();
		if (sel >= 0)
		{
			m_BrushComboBox->GetLBText(sel, strValue);
		}
	}

	if (strValue == "Smooth")
	{
		m_Graphic->SetTerrainSculptMode(ESculptMode::SMOOTH);
	}
	else if (strValue == "Flatten")
	{
		m_Graphic->SetTerrainSculptMode(ESculptMode::FLATTEN);
	}
	else if (strValue == "Raise")
	{
		m_Graphic->SetTerrainSculptMode(ESculptMode::RAISE);
	}
	else if (strValue == "Lower")
	{
		m_Graphic->SetTerrainSculptMode(ESculptMode::LOWER);
	}
	else if (strValue == "AlphaMap")
	{
		m_Graphic->SetTerrainSculptMode(ESculptMode::ALPHAMAP);
	}
	else if (strValue == "Noise")
	{
		m_Graphic->SetTerrainSculptMode(ESculptMode::NOISE);
	}

}


void SculptPanelDlg::OnBnClickedButtonErode()
{
	// TODO: Add your control notification handler code here
}

void SculptPanelDlg::OnChangeBrushstrengthtextbox()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString text;
	m_BrushStrengthTextbox->GetWindowTextW(text);
	int value = StrToIntW(text.GetBuffer());
	if (value > 100)
	{
		value = 100;
	}
	else if (value < 1)
	{
		value = 1;
	}
	m_BrushStrengthSlider->SetPos(value);
}


void SculptPanelDlg::OnBnClickedLoadalphamap()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFile(TRUE);

	const int MAX_CFileDialog_FILE_COUNT = 99;
	const int FILE_LIST_BUFFER_SIZE = ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1);
	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);


	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.Flags |= OFN_PATHMUSTEXIST;
	ofn.Flags |= OFN_FILEMUSTEXIST;
	ofn.lpstrFile = p;
	ofn.nMaxFile = MAX_PATH + 1;
	dlgFile.DoModal();
	fileName.ReleaseBuffer();


	m_AlphaMapLoaded = m_Graphic->SetTerrainAlphaMap(p);
	if (m_AlphaMapLoaded)
	{
		m_ButtonLoadAlphaMap->SetIcon(m_hIconAlphaMap);
		m_ButtonLoadAlphaMap->SetWindowTextW(_T("AlphaMap"));
	}
	else
	{
		m_ButtonLoadAlphaMap->SetIcon(nullptr);
		m_ButtonLoadAlphaMap->SetWindowTextW(_T("Load AlphaMap"));
	}



}


void SculptPanelDlg::OnBnClickedButtonNoiseRndmz()
{
	// TODO: Add your control notification handler code here
	m_NoiseSeedSpin->SetPos(rand());
	UpdateData(FALSE);
}


void SculptPanelDlg::OnDeltaposSpinSeed(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (pNMUpDown->hdr.hwndFrom == m_NoiseSeedSpin->GetSafeHwnd())
	{
		UpdateData(FALSE);
		m_Graphic->SetSculptNoiseSeed(pNMUpDown->iPos);
	}

	*pResult = 0;
}


void SculptPanelDlg::OnEnChangeTextboxNscale()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString text;
	m_NoiseScaleTextBox->GetWindowTextW(text);
	int value = StrToIntW(text.GetBuffer());
	if (value > 4096)
	{
		value = 4096;
	}
	else if (value < 0)
	{
		value = 0;
	}
	m_NoiseScaleSlider->SetPos(value);
	if (m_Graphic)
	{
		m_Graphic->SetSculptNoiseScale(value);
	}
}


void SculptPanelDlg::OnEnChangeTextboxNfreq()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString text;
	m_NoiseFreqTextBox->GetWindowTextW(text);
	int value = StrToIntW(text.GetBuffer());
	if (value > 1000)
	{
		value = 1000;
	}
	else if (value < -1000)
	{
		value = -1000;
	}
	m_NoiseFreqSlider->SetPos(value);
	if (m_Graphic)
	{
		m_Graphic->SetSculptNoiseFreq(value);
	}
}

void SculptPanelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	int hspacing = 0;
	int vspacing = 40;

	// TODO: Add your message handler code here
	if (m_ControlsInitialized)
	{
		cx = 260;

		m_ButtonLoadHeightMap->SetWindowPos(nullptr, cx - 250 - hspacing, 10, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonLoadAlphaMap->SetWindowPos(nullptr, cx - 150 - hspacing, 10, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_HeightMapFileName->SetWindowPos(nullptr, cx - 250 - hspacing, 0 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushSizeSlider->SetWindowPos(nullptr, cx - 217 - hspacing, 30 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushSizeTextbox->SetWindowPos(nullptr, cx - 68 - hspacing, 30 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_SliderSizeText->SetWindowPos(nullptr, cx - 252 - hspacing, 35 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_BrushStrengthSlider->SetWindowPos(nullptr, cx - 217 - hspacing, 80 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushStrengthTextbox->SetWindowPos(nullptr, cx - 68 - hspacing, 80 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_SliderWeightText->SetWindowPos(nullptr, cx - 252 - hspacing, 85 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_BrushComboBox->SetWindowPos(nullptr, cx - 185 - hspacing, 110 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushTypeText->SetWindowPos(nullptr, cx - 250 - hspacing, 115 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonErode->SetWindowPos(nullptr, cx - 250 - hspacing, 140 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ErodeIterationText->SetWindowPos(nullptr, cx - 190 - hspacing, 143 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_NoiseScaleText->SetWindowPos(nullptr, cx - 250 - hspacing, 180 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseScaleSlider->SetWindowPos(nullptr, cx - 190 - hspacing, 175 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseScaleTextBox->SetWindowPos(nullptr, cx - 80 - hspacing, 175 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_NoiseFreqText->SetWindowPos(nullptr, cx - 250 - hspacing, 210 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseFreqSlider->SetWindowPos(nullptr, cx - 190 - hspacing, 205 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseFreqTextBox->SetWindowPos(nullptr, cx - 80 - hspacing, 205 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_NoiseSeedText->SetWindowPos(nullptr, cx - 250 - hspacing, 240 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseSeedTextBox->SetWindowPos(nullptr, cx - 185 - hspacing, 235 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseSeedSpin->SetWindowPos(nullptr, cx - 120 - hspacing, 235 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonNoiseSeedRandomize->SetWindowPos(nullptr, cx - 90 - hspacing, 235 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);


		m_SliderSizeText->RedrawWindow(nullptr);
		m_SliderWeightText->RedrawWindow(nullptr);
		m_BrushStrengthSlider->RedrawWindow(nullptr);
		m_ErodeIterationText->RedrawWindow(nullptr);
		m_BrushSizeTextbox->RedrawWindow(nullptr);
		m_BrushStrengthTextbox->RedrawWindow(nullptr);
	}



}
