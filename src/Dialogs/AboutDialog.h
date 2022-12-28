/**
 * Class name:    AboutDialog

 * Description:   Shows version number, author and contact information

 * Author:        Frank Dockhorn
 */


#ifndef ABOUT_DIALOG
#define ABOUT_DIALOG

#include "resource.h"
#include "DialogBase.h"
#include <commctrl.h>
#include <Commdlg.h>
#include <ShellAPI.h>

class AboutDialog : public DialogBase<AboutDialog>
{
public:
   enum { IDD = IDD_ABOUT };

   BEGIN_MSG_MAP(AboutDialog)
		NOTIFY_HANDLER			(IDC_SYSLINK3, NM_CLICK, OnSysLinkNotify)
		NOTIFY_HANDLER			(IDC_SYSLINK1, NM_CLICK, OnSysLinkNotify)
		NOTIFY_HANDLER			(IDC_SYSLINK2, NM_CLICK, OnSysLinkNotify)
		MESSAGE_HANDLER		(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER		(WM_INITDIALOG, OnInitDialog)			
		CHAIN_MSG_MAP			(DialogBase<AboutDialog>)
		COMMAND_ID_HANDLER	(ID_ESCAPE, OnEscape)
		COMMAND_ID_HANDLER	(IDOK, OnOK)
		COMMAND_ID_HANDLER	(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER	(IDC_COMBO_FONT, OnComboSelect)			
   END_MSG_MAP()

   AboutDialog						();
	~AboutDialog					();

   LRESULT  OnInitDialog		(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
	LRESULT  OnDestroy			(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  OnCancel				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);;
	LRESULT  OnEscape				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnOK					(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT	OnSysLinkNotify	(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
	LRESULT	OnComboSelect		(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	USHORT	getDialogSize		();

protected:
	void initFontsAndColors		();
 	void setDialogSize			();
private:		
	tstring		m_version;
	const char* m_versionOpenGL;
	HFONT			m_hFont;
	USHORT		m_sizeNewVal;
};

#endif