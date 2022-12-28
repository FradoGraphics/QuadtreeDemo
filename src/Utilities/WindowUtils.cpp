/**
 * Class name:    WindowUtils

 * Description:   This helper class facilitates working with windows and dialogs. 

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "WindowUtils.h"
#include "dwmapi.h"


typedef HRESULT (WINAPI* GETWINDOWATTRIB_PTR) (HWND, DWORD, PVOID, DWORD);
typedef HRESULT (WINAPI* SETWINDOWATTRIB_PTR) (HWND, DWORD, LPCVOID, DWORD);
typedef HRESULT (WINAPI* ISCOMPOSITIONENABLED_PTR) (BOOL*);

const unsigned	WindowUtils::s_numSizes[] = {8, 9, 10, 11, 12, 13, 14, 15};
const unsigned	WindowUtils::DLG_SIZE_MIN	= 3;
const unsigned	WindowUtils::DLG_NUM_SIZES = sizeof(s_numSizes) / sizeof(unsigned);
const unsigned	WindowUtils::DLG_SIZE_MAX	= WindowUtils::s_numSizes[DLG_NUM_SIZES - 1];


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const unsigned WindowUtils::getDialogSize(unsigned idx)
{
	if (idx < 0)
	{
		return DLG_SIZE_MIN;
	}
	if (idx > (DLG_NUM_SIZES-1))
	{
		return DLG_SIZE_MAX;
	}
	return s_numSizes[idx];
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

BOOL WindowUtils::checkDWMStatusAndDisableNCRendering(HWND hwnd) 
{
	HRESULT	aResult		= S_OK;
	BOOL		isEnabled	= FALSE;

	// Load the dll and keep the handle to it
	// must load dynamically because this dll exists only in vista -- not in xp.
	// if this is running on XP then use old way.

	HMODULE dwmapiDllHandle = LoadLibrary(L"dwmapi.dll");  

	// not on Vista / Windows7 so no need to account for aero.
	if (dwmapiDllHandle) 
	{
		ISCOMPOSITIONENABLED_PTR pDwmIsCompositionEnabled;
		pDwmIsCompositionEnabled = (ISCOMPOSITIONENABLED_PTR)::GetProcAddress(dwmapiDllHandle, 
			"DwmIsCompositionEnabled");
		if(pDwmIsCompositionEnabled)
		{
			aResult = pDwmIsCompositionEnabled(&isEnabled);
		}
		BOOL s = SUCCEEDED(aResult);
		if (s && isEnabled)
		{
			DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;
			SETWINDOWATTRIB_PTR pDwmSetWindowAttribute;
			pDwmSetWindowAttribute = (SETWINDOWATTRIB_PTR)::GetProcAddress(dwmapiDllHandle,
				"DwmSetWindowAttribute");
			if(pDwmSetWindowAttribute) 
			{
				aResult = pDwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, 
					&ncrp, sizeof(ncrp));
			}
		}
	}
	FreeLibrary(dwmapiDllHandle);
	return isEnabled;
}

