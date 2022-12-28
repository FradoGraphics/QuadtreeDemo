/**
 * Class name:    MsgBoxDialog

 * Description:   Generic dialog used to show messages

 * Author:        Frank Dockhorn
 */


#ifndef MSGBOX_DIALOG
#define MSGBOX_DIALOG

#include "resource.h"
#include "DialogBase.h"

class MsgBoxDialog : public DialogBase<MsgBoxDialog>
{
public:
   enum { IDD = IDD_DIALOG_MSGBOX };

   BEGIN_MSG_MAP(MsgBoxDialog)
      MESSAGE_HANDLER		(WM_INITDIALOG, OnInitDialog)			
		MESSAGE_HANDLER		(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP			(DialogBase<MsgBoxDialog>)
		COMMAND_ID_HANDLER	(ID_ESCAPE, OnEscape)
		COMMAND_ID_HANDLER	(IDCANCEL, OnEscape)
		COMMAND_ID_HANDLER	(IDBUTTON1, OnButton1)
		COMMAND_ID_HANDLER	(IDBUTTON2, OnButton2)
   END_MSG_MAP()

	MsgBoxDialog					(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
	~MsgBoxDialog					();

   LRESULT  OnInitDialog		(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT  OnDestroy			(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  OnButton1			(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);;
	LRESULT  OnEscape				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnButton2			(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 

private:	
	UINT		m_uType;
	int		m_iButtonId[2];
	LPCTSTR	m_lpCaption;
	LPCTSTR	m_lpText;
};

#endif