
// LevelEditorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "LevelEditor.h"
#include "LevelEditorDlg.h"
#include "afxdialogex.h"

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
END_MESSAGE_MAP()


// CLevelEditorDlg message handlers

BOOL CLevelEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	InitializeControls();



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

		//Update();
		//Render();

		//Invalidate(FALSE);
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

}

void CLevelEditorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)m_BrushSizeSlider)
	{
		m_BrushSizeSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
		m_BrushSizeTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());
		UpdateData(FALSE);
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

	m_BrushSizeSlider	= (CSliderCtrl*)(GetDlgItem(IDC_BRUSHSIZESLIDER));
	m_BrushSizeTextbox	= (CEdit*)GetDlgItem(IDC_BRUSHSIZETEXTBOX);
	m_BrushComboBox		= (CComboBox*)GetDlgItem(IDC_COMBO_BRUSHTYPE);
	m_HeightMapFileName = (CEdit*)GetDlgItem(IDC_HMFILENAME);

	m_BrushSizeSlider->SetRange(0, 100, TRUE);
	m_BrushSizeSlider->SetPos(0);
	m_BrushSizeSliderVal.Format(_T("%d"), m_BrushSizeSlider->GetPos());
	m_BrushSizeTextbox->SetWindowTextW(m_BrushSizeSliderVal.GetBuffer());

	return true;
}

bool CLevelEditorDlg::Update()
{
	static int frames = 0;

	CString frameStr;
	frameStr.Format(_T("%d"), frames);

	this->SetWindowTextW(frameStr.GetBuffer());

	frames++;

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
	Update();
	Render();

	return TRUE;
}
