/**
 * Class name:    OpenglDialog

 * Description:   Shows error message if OpenGL version is not supported

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "OpenglDialog.h"

extern QuadtreeApp theApp;
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenglDialog::OpenglDialog(int major_version, int minor_version) 
:	
	m_majorGLVersion			(major_version), 
	m_minorGLVersion			(minor_version)	
{
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenglDialog::~OpenglDialog()
{	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT OpenglDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DialogBase<OpenglDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	loadSysIcon(IDI_ERROR, IDC_STATIC_ICON, IDC_STATIC_OPENGL);
	
	CString text, message, link;
	text.LoadString(IDS_ERROR_OPENGL1);
	message.Format(text, m_majorGLVersion, m_minorGLVersion);
	SetDlgItemText(IDC_STATIC_OPENGL, message);
	text.LoadString(IDS_ERROR_OPENGL2);
	SetDlgItemText(IDC_STATIC_OPENGL2, text);
	text.LoadString(IDS_ERROR_OPENGL3);
	SetDlgItemText(IDC_STATIC_OPENGL3, text);
	text.LoadString(IDS_ERROR_OPENGL4);
	SetDlgItemText(IDC_STATIC_OPENGL4, text);
	
	CenterWindow(m_hWnd);
	SetForegroundWindow(m_hWnd);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT OpenglDialog::OnEscape(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDCANCEL);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT OpenglDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDOK);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT OpenglDialog::OnCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDCOPY_MESA);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT OpenglDialog::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDCANCEL);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT OpenglDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{   	
	return DialogBase<OpenglDialog>::OnDestroy(uMsg, wParam, lParam, bHandled);   
}

