
// LevelEditorDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "LevelEditorDlg.h"
#include "afxdialogex.h"
#include "../ui/CPictureControl.h"
#include "../Graphics/ACommonIncludes.h"
#include "../Graphics/AGraphic.h"

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
	params.textureLayers.push_back(_T("../Data/Textures/Grass0130_1.jpg"));

	m_Graphic->InitializeTerrain(params);




}

void CLevelEditorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)m_BrushSizeSlider)
	{
		m_BrushSizeSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
		m_BrushSizeTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());
		UpdateData(FALSE);
		m_Graphic->SetTextureUVScale(m_BrushSizeSlider->GetPos() + 1);
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
		TRACE(L"%s\n", strValue.GetString());
	}
	else if (strValue == "Flatten")
	{
		TRACE(L"%s\n", strValue.GetString());
	}
	else if (strValue == "Raise")
	{
		TRACE(L"%s\n", strValue.GetString());
	}
	else if (strValue == "Lower")
	{
		TRACE(L"%s\n", strValue.GetString());
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
	m_ButtonLoadHeightMap	= (CButton*)(GetDlgItem(IDC_LOADHEIGHTMAP));
	m_ButtonErode			= (CButton*)(GetDlgItem(IDC_BUTTON_ERODE));
	m_BrushSizeSlider	= (CSliderCtrl*)(GetDlgItem(IDC_BRUSHSIZESLIDER));
	m_BrushSizeTextbox	= (CEdit*)GetDlgItem(IDC_BRUSHSIZETEXTBOX);
	m_BrushComboBox		= (CComboBox*)GetDlgItem(IDC_COMBO_BRUSHTYPE);
	m_HeightMapFileName = (CEdit*)GetDlgItem(IDC_HMFILENAME);
	m_BrushTypeText		= (CStatic*)GetDlgItem(IDC_STATIC_BRUSHTYPELEBAL);
	m_RenderBox			= (CPictureControl*)GetDlgItem(IDC_RENDERBOX);
	m_FpsText			= (CStatic*)GetDlgItem(IDC_STATIC_FPS);
	m_FpsText->SetWindowPos(&wndTop, 35, 10, 0, 0, SWP_NOSIZE);

	m_FpsText->LockWindowUpdate();

	m_BrushSizeSlider->SetRange(0, 100, TRUE);
	m_BrushSizeSlider->SetPos(0);
	m_BrushSizeSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
	m_BrushSizeTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());

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

	return true;
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
	frameStr.Format(_T("X:%d Y:%d"), pointOnRenderBox.x, pointOnRenderBox.y);
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
		m_HeightMapFileName->SetWindowPos(nullptr, cx - 250 - hspacing, 50 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushSizeSlider->SetWindowPos(nullptr, cx - 257 - hspacing, 80 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushSizeTextbox->SetWindowPos(nullptr, cx - 68 - hspacing, 80 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushComboBox->SetWindowPos(nullptr, cx - 185 - hspacing, 110 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_BrushTypeText->SetWindowPos(nullptr, cx - 250 - hspacing, 115 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ButtonErode->SetWindowPos(nullptr, cx - 250 - hspacing, 135 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_ErodeIterationText->SetWindowPos(nullptr, cx - 190 - hspacing, 140 + vspacing, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_RenderBox->SetWindowPos(&wndBottom, 25, 50, cx - 300, cy - 100, SWP_NOZORDER);
		//m_FpsText->SetWindowPos(&wndTop, 35, 10, 0, 0, SWP_NOSIZE);

		CRect rect;
		m_RenderBox->GetWindowRect(rect);
		ScreenToClient(rect);

		m_Graphic->Resize(rect.right - rect.left, rect.bottom - rect.top);
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
	SendMouseState(point);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLevelEditorDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_MouseState.LeftDown = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CLevelEditorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_MouseState.LeftDown)
	{
		SendMouseState(point);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CLevelEditorDlg::SendMouseState(CPoint& point)
{
	CRect rect;
	m_RenderBox->GetWindowRect(rect);
	ScreenToClient(rect);

	if (point.x < rect.right && point.y < rect.bottom)
	{
		pointOnRenderBox = point - rect.TopLeft();
		m_Graphic->SetMouseState(pointOnRenderBox.x, pointOnRenderBox.y, m_MouseState.LeftDown);
	}
}
