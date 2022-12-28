/**
 * Class name:    AboutDialog

 * Description:   Shows version number, author and contact information

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "AboutDialog.h"
#include "QuadtreeApp.h"

extern QuadtreeApp theApp;

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


AboutDialog::AboutDialog() 
:	
	m_sizeNewVal				(theApp.getNewDialogSize()),
	m_hFont						(NULL),
	m_versionOpenGL			(NULL)
{	
	m_version			= theApp.getVersion();
	m_versionOpenGL	= theApp.getOpenGlVersion();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

AboutDialog::~AboutDialog()
{
	::DeleteObject(m_hFont);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT AboutDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DialogBase<AboutDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	initFontsAndColors();
	SetDlgItemText(IDC_STATIC_ABOUT, m_version.data());
	string version(m_versionOpenGL);
	string subVersion = version.substr(0, 30);
	SetDlgItemTextA(m_hWnd, IDC_STATIC_VERSION, subVersion.data());
	CString text;
	text.LoadString(IDS_AUTHOR);
	SetDlgItemText(IDC_STATIC_AUTHOR, text);	
	text.LoadString(IDS_LINK_EMAIL);
	SetDlgItemText(IDC_SYSLINK3, text);
	text.LoadString(IDS_LINK_LICENSE);
	SetDlgItemText(IDC_SYSLINK1, text);
	text.LoadString(IDS_LINK_WEBSITE);
	SetDlgItemText(IDC_SYSLINK2, text);
	setDialogSize();
	CenterWindow(m_hWnd);

	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void AboutDialog::initFontsAndColors()
{
	LOGFONT	lf;
	CWindow& wnd		= GetDlgItem(IDC_STATIC_KEY_NAV);
	CWindow& wnd2		= GetDlgItem(IDC_STATIC_MOUSE_NAV);
	CWindow& wnd3		= GetDlgItem(IDC_STATIC_ABOUT);
	CWindow& wnd4		= GetDlgItem(IDC_STATIC_DLG_SIZE);

	HFONT		hFont		= wnd.GetFont();	
	GetObject(hFont, sizeof(LOGFONT), &lf);
	lf.lfWeight			= FW_SEMIBOLD;	
	m_hFont				= CreateFontIndirect(&lf);

	if (m_hFont)
	{
		wnd.SetFont(m_hFont);
		wnd2.SetFont(m_hFont);
		wnd3.SetFont(m_hFont);
		wnd4.SetFont(m_hFont);
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

USHORT AboutDialog::getDialogSize()
{
	return m_sizeNewVal;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void AboutDialog::setDialogSize()
{
	TCHAR		buf[10];
	int		index	= -1;

	_stprintf_s(buf, _T("%d"), theApp.getDialogSize());
	GetDlgItem(IDC_STATIC_ACTIVE_FONT).SendMessage(WM_SETTEXT, 0, (LPARAM)buf);

	for (unsigned i = 0; i < WindowUtils::DLG_NUM_SIZES; i++)
	{		
		_stprintf_s(buf, _T("%d"), WindowUtils::getDialogSize(i));
		GetDlgItem(IDC_COMBO_FONT).SendMessage(CB_ADDSTRING, 0, (LPARAM)buf);
		if (m_sizeNewVal == WindowUtils::getDialogSize(i))
		{
			index = i;
		}
	}		
	if (index > -1)
	{
		GetDlgItem(IDC_COMBO_FONT).SendMessage(CB_SETCURSEL, index, 0);
	}
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT AboutDialog::OnEscape(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDCANCEL);
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT AboutDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDOK);
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT AboutDialog::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	EndDialog(IDCANCEL);
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT AboutDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{   
	return DialogBase<AboutDialog>::OnDestroy(uMsg, wParam, lParam, bHandled);   
}



/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT	AboutDialog::OnComboSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{ 	
	if (wNotifyCode == CBN_SELENDOK)
	{
		int index = GetDlgItem(IDC_COMBO_FONT).SendMessage(CB_GETCURSEL, 0, 0);
		m_sizeNewVal = WindowUtils::getDialogSize(index);
	}
	return TRUE; 
} 

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT	AboutDialog::OnSysLinkNotify(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
{ 
	PNMLINK pClick = (PNMLINK)pNMHDR;

	switch (pNMHDR->code) 
	{ 
		case NM_CLICK: 
		case NM_RETURN: 
		{ 
			PNMLINK pNMLink = (PNMLINK)pNMHDR; 
			LITEM item = pNMLink->item; 
			ShellExecute(NULL, _T("open"), item.szUrl, NULL, NULL, SW_SHOWNORMAL); 
			break; 
		} 
	} 
	return TRUE; 
} 


