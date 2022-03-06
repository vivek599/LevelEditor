
// LevelEditorDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "LevelEditorDlg.h"
#include "afxdialogex.h"
#include "../ui/CPictureControl.h"
#include "../Graphics/ACommonIncludes.h"
#include "../Graphics/AGraphic.h"
#include "../Graphics/ATerrain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLevelEditorDlg dialog



CLevelEditorDlg::CLevelEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEVELEDITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ControlsInitialized = false;
	m_ButtonLoadHeightMap = nullptr;
	m_ButtonErode = nullptr;
	m_BrushSizeSlider = nullptr;
	m_BrushSizeTextbox = nullptr;
	m_HeightMapFileName = nullptr;
	m_BrushComboBox = nullptr;
	m_ErodeIterationText = nullptr;
	m_BrushTypeText		= nullptr;
	m_frameCounter		= 0;
	m_MouseState.LeftDown = false;
}

void CLevelEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLevelEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, &CLevelEditorDlg::OnFileExit)
	ON_BN_CLICKED(IDCANCEL, &CLevelEditorDlg::OnBnClickedCancel)
	ON_COMMAND(ID_HELP_ABOUT, &CLevelEditorDlg::OnHelpAbout)
	ON_BN_CLICKED(IDC_LOADHEIGHTMAP, &CLevelEditorDlg::OnBnClickedLoadheightmap)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_BRUSHSIZETEXTBOX, &CLevelEditorDlg::OnEnChangeBrushsizetextbox)
	ON_CBN_SELENDOK(IDC_COMBO_BRUSHTYPE, &CLevelEditorDlg::OnCbnSelendokComboBrushtype)
	ON_BN_CLICKED(IDC_BUTTON_ERODE, &CLevelEditorDlg::OnBnClickedButtonErode)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_EN_CHANGE(IDC_BRUSHSTRENGTHTEXTBOX, &CLevelEditorDlg::OnChangeBrushstrengthtextbox)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_BN_CLICKED(IDC_LOADALPHAMAP, &CLevelEditorDlg::OnBnClickedLoadalphamap)
	ON_BN_CLICKED(IDC_BUTTON_NOISE_RNDMZ, &CLevelEditorDlg::OnBnClickedButtonNoiseRndmz)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SEED, &CLevelEditorDlg::OnDeltaposSpinSeed)
	ON_EN_CHANGE(IDC_TEXTBOX_NSCALE, &CLevelEditorDlg::OnEnChangeTextboxNscale)
	ON_EN_CHANGE(IDC_TEXTBOX_NFREQ, &CLevelEditorDlg::OnEnChangeTextboxNfreq)
END_MESSAGE_MAP()


// CLevelEditorDlg message handlers

BOOL CLevelEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_ControlsInitialized = InitializeControls();

	if (m_ControlsInitialized)
	{
		CRect rect;
		m_RenderBox->GetWindowRect(rect);
		m_RenderBox->ScreenToClient(&rect);
		m_Graphic.reset(new AGraphic( m_RenderBox->m_hWnd,rect.Width(), rect.Height(), false, false)); 
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLevelEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLevelEditorDlg::OnPaint()
{
 

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLevelEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLevelEditorDlg::OnFileExit()
{
	// TODO: Add your command handler code here
	PostQuitMessage(0);
}

void CLevelEditorDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CLevelEditorDlg::OnHelpAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CLevelEditorDlg::OnBnClickedLoadheightmap()
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
	ofn.nMaxFile = MAX_PATH+1;
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

void CLevelEditorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

void CLevelEditorDlg::OnEnChangeBrushsizetextbox()
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
	else if( value < 0 )
	{
		value = 0;
	}
	m_BrushSizeSlider->SetPos(value);

}

BOOL CLevelEditorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == this->m_hWnd && pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CLevelEditorDlg::OnCbnSelendokComboBrushtype()
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


void CLevelEditorDlg::OnBnClickedButtonErode()
{
	// TODO: Add your control notification handler code here
}

bool CLevelEditorDlg::InitializeControls()
{
	m_Menu.LoadMenuW(MAKEINTRESOURCE(IDR_MENU1));
	this->SetMenu(&m_Menu);

	m_ErodeIterationText	= (CStatic*)(GetDlgItem(IDC_STATIC_ITER));
	m_SliderSizeText		= (CStatic*)(GetDlgItem(IDC_STATIC_SZ));
	m_SliderWeightText		= (CStatic*)(GetDlgItem(IDC_STATIC_WT));
	m_ButtonLoadHeightMap	= (CButton*)(GetDlgItem(IDC_LOADHEIGHTMAP));
	m_ButtonLoadAlphaMap	= (CButton*)(GetDlgItem(IDC_LOADALPHAMAP));
	m_ButtonErode			= (CButton*)(GetDlgItem(IDC_BUTTON_ERODE));
	m_BrushSizeSlider	= (CSliderCtrl*)(GetDlgItem(IDC_BRUSHSIZESLIDER));
	m_BrushSizeTextbox	= (CEdit*)GetDlgItem(IDC_BRUSHSIZETEXTBOX);
	m_BrushStrengthSlider	= (CSliderCtrl*)(GetDlgItem(IDC_BRUSHSTRENGTHSLIDER));
	m_BrushStrengthTextbox	= (CEdit*)GetDlgItem(IDC_BRUSHSTRENGTHTEXTBOX);
	m_BrushComboBox		= (CComboBox*)GetDlgItem(IDC_COMBO_BRUSHTYPE);
	m_HeightMapFileName = (CEdit*)GetDlgItem(IDC_HMFILENAME);
	m_BrushTypeText		= (CStatic*)GetDlgItem(IDC_STATIC_BRUSHTYPELEBAL);
	m_RenderBox			= (CPictureControl*)GetDlgItem(IDC_RENDERBOX);
	m_FpsText			= (CStatic*)GetDlgItem(IDC_STATIC_FPS);
	m_FpsText->SetWindowPos(&wndTop, 35, 10, 0, 0, SWP_NOSIZE); 
	m_FpsText->LockWindowUpdate();

	m_NoiseScaleText	= (CStatic*)GetDlgItem(IDC_STATIC_NSCALE);
	m_NoiseFreqText		= (CStatic*)GetDlgItem(IDC_STATIC_NFREQ);
	m_NoiseSeedText		= (CStatic*)GetDlgItem(IDC_STATIC_NSEED);
	m_ButtonNoiseSeedRandomize = (CButton*)(GetDlgItem(IDC_BUTTON_NOISE_RNDMZ));
	m_NoiseScaleTextBox		= (CEdit*)GetDlgItem(IDC_TEXTBOX_NSCALE);;
	m_NoiseFreqTextBox		= (CEdit*)GetDlgItem(IDC_TEXTBOX_NFREQ);;
	m_NoiseSeedTextBox		= (CEdit*)GetDlgItem(IDC_TEXTBOX_NSEED);;
	m_NoiseSeedSpin			= (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SEED);;
	m_NoiseScaleSlider		= (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_NSCALE));
	m_NoiseFreqSlider		= (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_NFREQ));

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

	m_NoiseScaleSlider->SetRange(1, RAND_MAX, TRUE);
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

	m_FpsText->SetFont(&m_FpsFont);

	m_hIconAlphaMap = AfxGetApp()->LoadIcon(IDR_ALPHAMAPICON);

	return true;
}

void CLevelEditorDlg::CheckMouseMovingState()
{ 
	m_MouseState.Draging = false;
}

bool CLevelEditorDlg::Update()
{


	return true;
}

bool CLevelEditorDlg::Render()
{
	return true;
}

LRESULT CLevelEditorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD QueueStatus;
	LRESULT resValue = 0;
	bool OnIdleRetVal = true;

	if (message == WM_QUIT || message == WM_DESTROY || message == WM_CLOSE)
	{
		exit(0);
	}

	//m_MouseState.Draging = (message == WM_MOUSEMOVE) && ((wParam & MK_LBUTTON) == 1); TODO

	if (message == WM_IDLE)
	{
		OnIdleRetVal = OnIdle((UINT)wParam);
		if (!OnIdleRetVal)
		{
			wParam = 0;
		}
	}
	else
	{
		resValue = CDialog::WindowProc(message, wParam, lParam);
	}

	QueueStatus = GetQueueStatus(QS_ALLINPUT);

	if (HIWORD(QueueStatus) == 0)
	{
		PostMessage(WM_IDLE, wParam + (OnIdleRetVal ? 1 : 0), 0);
	}

	return resValue;
}

BOOL CLevelEditorDlg::OnIdle(LONG lCount)
{
	if (m_Graphic)
	{
		m_Graphic->BeginScene(ArgbToColor(ColorARGB::Blue));
		m_Graphic->Update(m_deltaTime);
		m_Graphic->Render();
		m_Graphic->EndScene();
	}

	CalculateFps();
	CheckMouseMovingState();

	return TRUE;
}

void CLevelEditorDlg::CalculateFps()
{
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES] = {};
	static time_point<system_clock> prevTick = system_clock::now();
	time_point<system_clock> currentTick = system_clock::now();

	duration<double> elapsed_seconds = currentTick - prevTick;
	prevTick = currentTick;

	m_deltaTime = elapsed_seconds.count();

	frameTimes[m_frameCounter % NUM_SAMPLES] = m_deltaTime;

	int count = __min(NUM_SAMPLES, m_frameCounter);

	float avgFrameTime = 0.0f;
	for (int i = 0; i < count; i++)
	{
		avgFrameTime += frameTimes[i];
	}
	avgFrameTime /= count;

	int Fps = avgFrameTime > 0?  int(1.0f / avgFrameTime) : 60;

	m_frameCounter++;

	CString frameStr;
	frameStr.Format(_T("X:%d Y:%d, FPS : %d"), pointOnRenderBox.x, pointOnRenderBox.y, Fps);
	m_FpsText->SetWindowTextW(frameStr.GetBuffer());
	m_FpsText->UpdateWindow();
}

void CLevelEditorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_ControlsInitialized)
	{
		int hspacing = 0;
		int vspacing = 30;
		m_ButtonLoadHeightMap->SetWindowPos(nullptr, cx - 250 - hspacing, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonLoadAlphaMap->SetWindowPos(nullptr, cx - 150 - hspacing, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_HeightMapFileName->SetWindowPos(nullptr, cx - 250 - hspacing, 50 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushSizeSlider->SetWindowPos(nullptr, cx - 217 - hspacing, 80 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushSizeTextbox->SetWindowPos(nullptr, cx - 68 - hspacing, 80 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_SliderSizeText->SetWindowPos(nullptr, cx - 252 - hspacing, 85 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_BrushStrengthSlider->SetWindowPos(nullptr, cx - 217 - hspacing, 110 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushStrengthTextbox->SetWindowPos(nullptr, cx - 68 - hspacing, 110 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_SliderWeightText->SetWindowPos(nullptr, cx - 252 - hspacing, 115 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_BrushComboBox->SetWindowPos(nullptr, cx - 185 - hspacing, 140 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushTypeText->SetWindowPos(nullptr, cx - 250 - hspacing, 145 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonErode->SetWindowPos(nullptr, cx - 250 - hspacing, 170 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ErodeIterationText->SetWindowPos(nullptr, cx - 190 - hspacing, 173 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_NoiseScaleText->SetWindowPos(nullptr, cx - 250 - hspacing, 210 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseScaleSlider->SetWindowPos(nullptr, cx - 190 - hspacing, 205 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseScaleTextBox->SetWindowPos(nullptr, cx - 80 - hspacing, 205 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_NoiseFreqText->SetWindowPos(nullptr, cx - 250 - hspacing, 240 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseFreqSlider->SetWindowPos(nullptr, cx - 190 - hspacing, 235 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseFreqTextBox->SetWindowPos(nullptr, cx - 80 - hspacing, 235 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		m_NoiseSeedText->SetWindowPos(nullptr, cx - 250 - hspacing, 270 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseSeedTextBox->SetWindowPos(nullptr, cx - 185 - hspacing, 265 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_NoiseSeedSpin->SetWindowPos(nullptr, cx - 120 - hspacing, 265 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonNoiseSeedRandomize->SetWindowPos(nullptr, cx - 90 - hspacing, 265 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		
		





		m_RenderBox->SetWindowPos(&wndBottom, 25, 50, cx - 300, cy - 100, SWP_NOZORDER);
		//m_FpsText->SetWindowPos(&wndTop, 35, 10, 0, 0, SWP_NOSIZE);

		CRect rect;
		m_RenderBox->GetWindowRect(rect);
		ScreenToClient(rect);

		m_Graphic->Resize(rect.right - rect.left, rect.bottom - rect.top);
		m_SliderSizeText->RedrawWindow(nullptr);
		m_SliderWeightText->RedrawWindow(nullptr);
		m_BrushStrengthSlider->RedrawWindow(nullptr);
		m_ErodeIterationText->RedrawWindow(nullptr);
		m_BrushSizeTextbox->RedrawWindow(nullptr);
		m_BrushStrengthTextbox->RedrawWindow(nullptr);
	}


	// TODO: Add your message handler code here
}

BOOL CLevelEditorDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


	return CDialogEx::OnEraseBkgnd(pDC);
}

void CLevelEditorDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 480;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CLevelEditorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_MouseState.LeftDown = true;
	SendMouseState(point, m_MouseState);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLevelEditorDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_MouseState.LeftDown = false;
	m_MouseState.Draging = false;
	SendMouseState(point, m_MouseState);

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CLevelEditorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	TRACKMOUSEEVENT tme = { 0 };
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = 50;  // HOVER_DEFAULT, or the hover timeout in milliseconds.
	::TrackMouseEvent(&tme);
	m_MouseState.Draging = true;
	SendMouseState(point, m_MouseState);

	CDialogEx::OnMouseMove(nFlags, point);
}

void CLevelEditorDlg::SendMouseState(CPoint& point, const MouseState& state )
{
	CRect rect;
	m_RenderBox->GetWindowRect(rect);
	ScreenToClient(rect);

	if (point.x < rect.right && point.y < rect.bottom)
	{
		pointOnRenderBox = point - rect.TopLeft();
		m_Graphic->SetMouseState(pointOnRenderBox.x, pointOnRenderBox.y, state.LeftDown, state.Draging);
	}
	else
	{ 
		m_Graphic->SetMouseState(pointOnRenderBox.x, pointOnRenderBox.y, false, state.Draging);
	}
}


void CLevelEditorDlg::OnChangeBrushstrengthtextbox()
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


void CLevelEditorDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_MouseState.Draging = false;
	SendMouseState(point, m_MouseState);

	CDialogEx::OnMouseHover(nFlags, point);
}

void CLevelEditorDlg::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnMouseLeave();
}

void CLevelEditorDlg::OnBnClickedLoadalphamap()
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


void CLevelEditorDlg::OnBnClickedButtonNoiseRndmz()
{
	// TODO: Add your control notification handler code here
	m_NoiseSeedSpin->SetPos(rand());
	UpdateData(FALSE);
}


void CLevelEditorDlg::OnDeltaposSpinSeed(NMHDR* pNMHDR, LRESULT* pResult)
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


void CLevelEditorDlg::OnEnChangeTextboxNscale()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString text;
	m_NoiseScaleTextBox->GetWindowTextW(text);
	int value = StrToIntW(text.GetBuffer());
	if (value > RAND_MAX)
	{
		value = RAND_MAX;
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


void CLevelEditorDlg::OnEnChangeTextboxNfreq()
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
