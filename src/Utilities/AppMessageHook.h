// AppMessageHook.h: interface for the AppMessageHook class.

//

//////////////////////////////////////////////////////////////////////


#if !defined(AFX_DIALOGMESSAGEHOOK_H__53812B4C_FBAD_4FD3_8238_85CD48CFE453__INCLUDED_)
#define AFX_DIALOGMESSAGEHOOK_H__53812B4C_FBAD_4FD3_8238_85CD48CFE453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <set>

typedef std::set<HWND> THWNDCollection;

// AppMessageHook makes it easy to properly
// process tab and accelerator keys in
// ATL modeless dialogs

class AppMessageHook  
{
public:
   // set a dialog message hook for the specified modeless dialog

   static HRESULT InstallHook		(HWND hWnd);
   static HRESULT UninstallHook	(HWND hWnd);
   static void    setGlutHwnd		(HWND hWnd)
   {
      s_hGlutHwnd = hWnd;         
   }
private:
   // the hook function

   static LRESULT CALLBACK GetMessageProc(int nCode, 
                                          WPARAM wParam, LPARAM lParam);

   // the hook handle
   static HHOOK				s_hHook;
   static HWND             s_hGlutHwnd;
   static HINSTANCE			s_hInstance;
   static HACCEL				s_hAccel;   
   static THWNDCollection	s_aWindows; // the set of HWNDs we are hooking
};

#endif
