/**
 * Class name:    OpenglDialog

 * Description:   Shows error message if OpenGL version is not supported

 * Author:        Frank Dockhorn
 */


#ifndef OPENGL_DIALOG
#define OPENGL_DIALOG

#include "resource.h"
#include "DialogBase.h"


class OpenglDialog : public DialogBase<OpenglDialog>
{
public:
   enum { IDD = IDD_DIALOG_OPENGL };

   BEGIN_MSG_MAP(OpenglDialog)
      MESSAGE_HANDLER		(WM_INITDIALOG, OnInitDialog)			
		MESSAGE_HANDLER		(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP			(DialogBase<OpenglDialog>)
		COMMAND_ID_HANDLER	(ID_ESCAPE, OnEscape)
		COMMAND_ID_HANDLER	(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER	(IDCOPY_MESA, OnCopy)
		COMMAND_ID_HANDLER	(IDOK, OnOK)
   END_MSG_MAP()

   OpenglDialog					(int major_version, int minor_version);
	~OpenglDialog					();

   LRESULT  OnInitDialog		(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT  OnDestroy			(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  OnCancel				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);;
	LRESULT  OnEscape				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnOK					(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnCopy				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 

private:
	int		m_majorGLVersion;
	int		m_minorGLVersion;
};

#endif