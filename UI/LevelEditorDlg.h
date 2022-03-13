
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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

	bool							m_ControlsInitializedSculpt;
	bool							m_ControlsInitializedPaint;

	unique_ptr<class SculptPanelDlg>	m_SculptDialog;
	unique_ptr<class PaintPanelDlg>		m_PaintDialog;
	CTabCtrl*						m_ModeTabs;
	CStatic*						m_FpsText;

	CMenu							m_Menu;
	
	CStatic*			m_RenderBox;

	float				m_deltaTime;
	int					m_frameCounter;

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

	
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	bool Update();
	bool Render();

private:

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

	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnTcnSelchangeTabMode(NMHDR* pNMHDR, LRESULT* pResult);
};
