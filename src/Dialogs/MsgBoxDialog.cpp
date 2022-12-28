/**
 * Class name:    MsgBoxDialog

 * Description:   Generic dialog used to show messages

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "MsgBoxDialog.h"

extern QuadtreeApp theApp;
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

MsgBoxDialog::MsgBoxDialog(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) 
:	
	m_uType		(uType),
	m_lpCaption	(lpCaption),
	m_lpText		(lpText)	
{	
	m_iButtonId[0] = IDCANCEL;
	m_iButtonId[1] = IDOK;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

MsgBoxDialog::~MsgBoxDialog()
{	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT MsgBoxDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DialogBase<MsgBoxDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	int		showButton	= SW_HIDE;
	TCHAR*	iconId		= IDI_ERROR;
   LRESULT	res			= TRUE;

	RECT rect, rectOrig;
	RECT rectButton1, rectButton2;
	RECT rectDlg;

	CString text;
	if (m_uType & MB_YESNO)
	{
		showButton		= SW_SHOW;
		iconId			= IDI_ERROR;
		m_iButtonId[0] = IDYES;
		m_iButtonId[1] = IDNO;
		res				= FALSE;

		text.LoadString(IDS_YES);
		SetDlgItemText(IDBUTTON1, text);
		text.LoadString(IDS_NO);
		SetDlgItemText(IDBUTTON2, text);
		PostMessage(WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDBUTTON1).m_hWnd, TRUE);
			
	}
	if ((m_uType & MB_ICONWARNING) == MB_ICONWARNING)
	{
		iconId = IDI_WARNING;
	}
	else if ((m_uType & MB_ICONQUESTION) == MB_ICONQUESTION)
	{
		iconId = IDI_QUESTION;
	}
	loadSysIcon(iconId, IDC_STATIC_ICON, IDC_STATIC_MESSAGE);
	SetWindowText(m_lpCaption);		
	HDC hDC = GetDlgItem(IDC_STATIC_MESSAGE).GetDC();
	SelectObject(hDC, GetDlgItem(IDC_STATIC_MESSAGE).GetFont());
	
	LONG style = GetDlgItem(IDC_STATIC_MESSAGE).GetWindowLong(GWL_STYLE);
	GetDlgItem(IDC_STATIC_MESSAGE).SetWindowLong(GWL_STYLE, style | SS_EDITCONTROL);
	GetDlgItem(IDC_STATIC_MESSAGE).GetClientRect(&rect);
	rectOrig = rect;	
		
	// determine the actual size of the static field and rearrange controls accordingly
	CString text2(m_lpText);
	UINT format = DT_LEFT | DT_CALCRECT | DT_EDITCONTROL | DT_WORDBREAK;
	DrawText(hDC, text2, text2.GetLength(), &rect, format);
	
	int diffWidth	= rect.right - rectOrig.right;
	int diffHeight	= rect.bottom - rectOrig.bottom;

	if (diffWidth < 0)
	{
		diffWidth = 0;
	}
	if (diffHeight < 0)
	{
		diffHeight = 0;
	}
	
	GetWindowRect(&rectDlg);
	int widthDlg	= rectDlg.right - rectDlg.left + diffWidth;
	int heigthDlg	= rectDlg.bottom - rectDlg.top + diffHeight;
	
	GetDlgItem(IDBUTTON1).GetWindowRect(&rectButton1);
	GetDlgItem(IDBUTTON2).GetWindowRect(&rectButton2);
	ScreenToClient(&rectButton1);
	ScreenToClient(&rectButton2);
	rectButton1.top	+= diffHeight;
	rectButton1.left	+= diffWidth;
	rectButton2.top	+= diffHeight;
	rectButton2.left	+= diffWidth;

	GetDlgItem(IDC_STATIC_MESSAGE).SetWindowPos(0, 0, 0, rect.right, rect.bottom, SWP_NOMOVE);
	GetDlgItem(IDBUTTON1).SetWindowPos(0, rectButton1.left, rectButton1.top, 0, 0, SWP_NOSIZE);
	GetDlgItem(IDBUTTON2).SetWindowPos(0, rectButton2.left, rectButton2.top, 0, 0, SWP_NOSIZE);
	SetDlgItemText(IDC_STATIC_MESSAGE, text2);	
	GetDlgItem(IDBUTTON1).ShowWindow(showButton);
	SetWindowPos(0, 0, 0, widthDlg, heigthDlg, SWP_NOMOVE);
	CenterWindow(m_hWnd);
	if (m_uType & MB_SETFOREGROUND)
	{
		SetForegroundWindow(m_hWnd);
	}	
	return res;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT MsgBoxDialog::OnEscape(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDCANCEL);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT MsgBoxDialog::OnButton2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(m_iButtonId[1]);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT MsgBoxDialog::OnButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(m_iButtonId[0]);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT MsgBoxDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{   	
	return DialogBase<MsgBoxDialog>::OnDestroy(uMsg, wParam, lParam, bHandled);   
}

