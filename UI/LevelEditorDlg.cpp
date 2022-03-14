
// LevelEditorDlg.cpp : implementation file
//

#include "framework.h"
#include "LevelEditor.h"
#include "LevelEditorDlg.h"
#include "afxdialogex.h"
#include "../Graphics/ACommonIncludes.h"
#include "../Graphics/AGraphic.h"
#include "SculptPanelDlg.h"
#include "PaintPanelDlg.h"

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
	m_ControlsInitializedSculpt = false;
	m_ControlsInitializedPaint = false;
	m_frameCounter		= 0;
	m_MouseState.LeftDown = false;
}

BEGIN_MESSAGE_MAP(CLevelEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, &CLevelEditorDlg::OnFileExit)
	ON_BN_CLICKED(IDCANCEL, &CLevelEditorDlg::OnBnClickedCancel)
	ON_COMMAND(ID_HELP_ABOUT, &CLevelEditorDlg::OnHelpAbout)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODE, &CLevelEditorDlg::OnTcnSelchangeTabMode)
END_MESSAGE_MAP()


// CLevelEditorDlg message handlers
void CLevelEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX); 
}

BOOL CLevelEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_Menu.LoadMenuW(MAKEINTRESOURCE(IDR_MENU1));
	this->SetMenu(&m_Menu);

	m_RenderBox = (CStatic*)GetDlgItem(IDC_RENDERBOX);
	m_FpsText = (CStatic*)GetDlgItem(IDC_STATIC_FPS);
	m_FpsText->SetWindowPos(&wndTop, 35, 10, 0, 0, SWP_NOSIZE);
	m_FpsText->LockWindowUpdate();

	m_ModeTabs = (CTabCtrl*)(GetDlgItem(IDC_TAB_MODE));
	m_ModeTabs->SetOwner(this);
	m_ModeTabs->InsertItem(0, _T("Sculpt"));
	m_ModeTabs->InsertItem(1, _T("Paint"));
	m_ModeTabs->SetCurSel(0);
	m_ModeTabs->SetMinTabWidth(100);

	m_SculptDialog.reset(new SculptPanelDlg(m_ModeTabs));
	m_SculptDialog->Create(IDD_DIALOG_SCULPT, m_ModeTabs);
	m_SculptDialog->OnInitDialog();

	m_PaintDialog.reset(new PaintPanelDlg(m_ModeTabs));
	m_PaintDialog->Create(IDD_DIALOG_PAINT, m_ModeTabs);
	m_PaintDialog->OnInitDialog();

	//m_FpsText->SetFont(&m_FpsFont);

	m_ControlsInitializedSculpt = m_SculptDialog->InitializeControls();
	m_ControlsInitializedPaint = m_PaintDialog->InitializeControls();

	if (m_ControlsInitializedSculpt && m_ControlsInitializedPaint)
	{
		CRect rect;
		m_RenderBox->GetWindowRect(rect);
		m_RenderBox->ScreenToClient(&rect);
		m_Graphic.reset(new AGraphic( m_RenderBox->m_hWnd,rect.Width(), rect.Height(), false, false)); 
		m_SculptDialog->InitGraphic(m_Graphic.get());
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
		//if (m_SculptDialog)
		//{
		//	m_SculptDialog->WindowProc(message, wParam, lParam);
		//}

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

	if (m_ControlsInitializedSculpt && m_ControlsInitializedPaint)
	{ 
		m_ModeTabs->SetWindowPos(nullptr, cx - 265, 50, 250, 480, SWP_NOZORDER);

		CRect rcClient, rcWindow;
		m_SculptDialog->OnSize(nType, cx, cy);
		m_PaintDialog->OnSize(nType, cx, cy);
		m_ModeTabs->GetClientRect(rcClient);
		m_ModeTabs->AdjustRect(FALSE, rcClient);
		m_SculptDialog->MoveWindow(rcClient);
		m_SculptDialog->ShowWindow(SW_SHOW);

		m_PaintDialog->MoveWindow(rcClient);
		m_PaintDialog->ShowWindow(SW_HIDE);


		 
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



void CLevelEditorDlg::OnTcnSelchangeTabMode(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	
	if (m_ModeTabs->GetCurSel() == 0)
	{
		m_SculptDialog->ShowWindow(SW_SHOW); 
		m_PaintDialog->ShowWindow(SW_HIDE);
	}
	else if(m_ModeTabs->GetCurSel() == 1)
	{
		m_SculptDialog->ShowWindow(SW_HIDE);
		m_PaintDialog->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}
