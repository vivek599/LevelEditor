
// LevelEditor.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLevelEditorApp:
// See LevelEditor.cpp for the implementation of this class
//

class CLevelEditorApp : public CWinApp
{
public:
	CLevelEditorApp();
	~CLevelEditorApp();

private:
	class CLevelEditorDlg* m_dlg;
	CShellManager* pShellManager;

// Overrides
public:
	virtual BOOL InitInstance();
	// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLevelEditorApp theApp;
