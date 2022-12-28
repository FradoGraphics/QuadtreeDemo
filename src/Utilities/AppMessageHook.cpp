// AppMessageHook.cpp: implementation of the AppMessageHook class.

//

//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "AppMessageHook.h"
#include "resource.h"


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

HWND					AppMessageHook::s_hGlutHwnd     = NULL;
HHOOK					AppMessageHook::s_hHook         = NULL;
HINSTANCE			AppMessageHook::s_hInstance     = NULL;
HACCEL				AppMessageHook::s_hAccel        = NULL;
THWNDCollection	AppMessageHook::s_aWindows;

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

// Hook procedure for WH_GETMESSAGE hook type.
// This function is more or less a combination of MSDN KB articles
// Q187988 and Q216503. See MSDN for additional details

LRESULT CALLBACK AppMessageHook::GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   LPMSG lpMsg				= (LPMSG)lParam;
   bool	glutNavigation	= false;

	if (lpMsg->hwnd == s_hGlutHwnd)
	{
		if (lpMsg->message == WM_MOUSEMOVE)
		{
			SetFocus(s_hGlutHwnd);			
			glutNavigation = true;
		}
		else if (lpMsg->message == WM_KEYDOWN)
		{
			switch (lpMsg->wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
			case VK_SHIFT:
				glutNavigation = true;
				break;
			}
		}
	}

	if (!glutNavigation)
	{
		if ((nCode >= 0) && PM_REMOVE == wParam &&			
			(lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST))
		{
			HWND hWnd;
			THWNDCollection::iterator it = s_aWindows.begin();

			// check each window we manage to see if the message is meant for them
			while (it != s_aWindows.end())
			{
				hWnd = *it;
				bool isWinEnabled    = ::IsWindow(hWnd) && ::IsWindowEnabled(hWnd);
				bool res1            = false;
				bool res2            = isWinEnabled && ::TranslateAccelerator(hWnd, s_hAccel, lpMsg);
				if (!res2)
				{
					res1 = isWinEnabled && ::IsDialogMessage(hWnd, lpMsg);
				}
				if (res1 || res2)
				{
					// The value returned from this hookproc is ignored, and it cannot
					// be used to tell Windows the message has been handled. To avoid
					// further processing, convert the message to WM_NULL before
					// returning.

					lpMsg->hwnd = NULL;
					lpMsg->message = WM_NULL;
					lpMsg->lParam = 0L;
					lpMsg->wParam = 0;

					break;
				}
				it++;
			}
		}
	}
   
   // Passes the hook information to the next hook procedure in
   // the current hook chain.
   return ::CallNextHookEx(s_hHook, nCode, wParam, lParam);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

HRESULT AppMessageHook::InstallHook(HWND hWnd)
{
   // make sure the hook is installed
   if (s_hHook == NULL)
   {
      s_hHook = ::SetWindowsHookEx(WH_GETMESSAGE,
												GetMessageProc,
												_AtlBaseModule.m_hInst,
												GetCurrentThreadId());

      // is the hook set?
      if (s_hHook == NULL)
      {
         return E_UNEXPECTED;
      }
   }

   // add the window to our list of managed windows
   if (s_aWindows.find(hWnd) == s_aWindows.end())
      s_aWindows.insert(hWnd);

   s_hInstance  = GetModuleHandle(NULL);
   s_hAccel     = ::LoadAccelerators(s_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1)); 

   return S_OK;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

HRESULT AppMessageHook::UninstallHook(HWND hWnd)
{
   HRESULT hr = S_OK;

   // was the window found?
   if (s_aWindows.erase(hWnd) == 0)
      return E_INVALIDARG;

   // is this the last window? if so, then uninstall the hook
   if (s_aWindows.size() == 0 && s_hHook)
   {
      if (!::UnhookWindowsHookEx(s_hHook))
         hr = HRESULT_FROM_WIN32(::GetLastError());

      s_hHook = NULL;
   }
   return hr;
}