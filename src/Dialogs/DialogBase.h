/**
 * Header name:   DialogBase.h

 * Description:   Common base class of all dialogs

 * Author:        Frank Dockhorn
 */



#ifndef DIALOG_BASE
#define DIALOG_BASE

#include "AppMessageHook.h"
#include "WindowUtils.h"
#include "QuadtreeApp.h"

template <class T> class DialogBase: public CDialogImpl<T>
{
public:
	BEGIN_MSG_MAP(DialogBase<T>)
		MESSAGE_HANDLER		(WM_MOVE, OnRePaint)		
		MESSAGE_HANDLER		(WM_ACTIVATE, OnRePaint)		
	END_MSG_MAP()
	DialogBase() 
	: m_parent(NULL), m_hIcon(NULL)
	{
	}
	void		setDialogSize		(	unsigned val) {m_size = val;}
	HWND		Create				(	HWND		hWndParent, LPARAM dwInitParam = NULL);
	INT_PTR	DoModal				(	HWND		hWndParent = ::GetActiveWindow(), 
											LPARAM	dwInitParam = NULL);
	LRESULT  OnInitDialog		(	UINT		uMsg, 
											WPARAM	wParam, 
											LPARAM	lParam, 
											BOOL&		bHandled);
	LRESULT  OnDestroy			(	UINT		uMsg, 
											WPARAM	wParam, 
											LPARAM	lParam, 
											BOOL&		bHandled);
	LRESULT  OnRePaint			(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void		setDialogFontSize	(HGLOBAL);
	bool		loadSysIcon(TCHAR* iconId, UINT ctrlId, UINT refCtrlId);

protected:
	HWND		m_parent;
	HICON		m_hIcon;
};

typedef HRESULT (WINAPI* LOADICONWITHSCALEDOWN_PTR) (HINSTANCE, PCWSTR, int, int, HICON*);
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

template<class T> LRESULT DialogBase<T>::OnInitDialog(UINT		uMsg, 
																		WPARAM	wParam, 
																		LPARAM	lParam, 
																		BOOL&		bHandled)
{
	AppMessageHook::InstallHook(m_hWnd);	
	WindowUtils::checkDWMStatusAndDisableNCRendering(m_hWnd); 
	return TRUE;		
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

template<class T> LRESULT DialogBase<T>::OnDestroy(UINT		uMsg, 
																	WPARAM	wParam, 
																	LPARAM	lParam, 
																	BOOL&		bHandled)
{   
	DestroyIcon(m_hIcon);
	AppMessageHook::UninstallHook(m_hWnd);
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

template<class T> LRESULT DialogBase<T>::OnRePaint(UINT uMsg, WPARAM wParam, LPARAM lParam, 
										 BOOL& bHandled)
{	
	if (uMsg == WM_ACTIVATE && wParam != WA_INACTIVE)
	{
		theApp.repaintScene(m_hWnd, false);
	}
	else if (uMsg == WM_MOVE)
	{
		theApp.repaintScene(m_hWnd);
	}	
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

template<class T> INT_PTR DialogBase<T>::DoModal(HWND hWndParent, LPARAM dwInitParam)
{
	BOOL result;

	ATLASSUME(m_hWnd == NULL);

	// Allocate the thunk structure here, where we can fail
	// gracefully.

	result = m_thunk.Init(NULL,NULL);
	if (result == FALSE) 
	{
		SetLastError(ERROR_OUTOFMEMORY);
		return -1;
	}

	_AtlWinModule.AddCreateWndData(&m_thunk.cd, this);
#ifdef _DEBUG
	m_bModal = true;
#endif //_DEBUG

	HMODULE	hModule	= ::GetModuleHandle(0);
	HRSRC		hrsrc		= ::FindResource(hModule, MAKEINTRESOURCE(T::IDD), RT_DIALOG);
	HGLOBAL	hglobal	= ::LoadResource(hModule, hrsrc);

	if (	hModule	== NULL 
		|| hrsrc		== NULL 
		|| hglobal	== NULL)
	{
		return -1;
	}

	setDialogFontSize(hglobal);

	HWND parent = m_parent = hWndParent;
	while (parent = ::GetParent(parent))
	{
		m_parent = parent;
	}

	::EnableWindow(m_parent, FALSE);
	INT_PTR res =  ::DialogBoxIndirectParam(_AtlBaseModule.GetResourceInstance(), 
											(LPCDLGTEMPLATE)hglobal, hWndParent, 
											T::StartDialogProc, dwInitParam);
	::EnableWindow(m_parent, TRUE);
	::FreeResource(hglobal);
	return res;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

template<class T> bool DialogBase<T>::loadSysIcon(TCHAR* iconId,
																	  UINT ctrlId,
																	  UINT refCtrlId)
{
	RECT rect;
	GetDlgItem(refCtrlId).GetWindowRect(&rect);
	ScreenToClient(&rect);
	LONG	refWidth		= rect.left * 0.4;
	LONG	lowerBound	= rect.left * 0.3;
	LONG	upperBound	= rect.left * 0.5;
	LONG	width			= GetSystemMetrics(SM_CXICON);
	LONG	height		= GetSystemMetrics(SM_CYICON);
	HICON hIcon			= NULL;

	if (height == 0 || width == 0)
	{
		return false;
	}
	float ratio			= (float)width / height;
	LONG	refHeight	= refWidth / ratio;

	width		= refWidth;
	height	= refHeight;

	LOADICONWITHSCALEDOWN_PTR pLoadIconWithScaleDown = NULL;

	hIcon = (HICON)LoadImage(NULL,
									iconId, 
									IMAGE_ICON, 
									width, 
									height, 
									LR_DEFAULTCOLOR | LR_SHARED);

	if (!hIcon)
	{
		return false;
	}
	
	ICONINFO		info;
	BITMAPINFO	bmi;
	memset(&info, 0, sizeof(ICONINFO));
	memset(&bmi, 0, sizeof(BITMAPINFO));
	GetIconInfo(hIcon, &info);
	GetObject(info.hbmColor, sizeof(BITMAPINFO), &bmi);
	
	if (	bmi.bmiHeader.biWidth < lowerBound 
		|| bmi.bmiHeader.biWidth > upperBound)
	{
		// try to get a properly scaled version of the icon
		HMODULE hComCtrl	= GetModuleHandle(L"comctl32.dll");
		HRESULT res			= S_FALSE;
		if (hComCtrl)
		{		
			pLoadIconWithScaleDown = (LOADICONWITHSCALEDOWN_PTR)::GetProcAddress(hComCtrl, 
				"LoadIconWithScaleDown");	
			if (pLoadIconWithScaleDown)
			{
				res = pLoadIconWithScaleDown(NULL, iconId, width, height, &m_hIcon);
			}		
		}

		if (m_hIcon == NULL && res != S_OK)
		{	
			m_hIcon = (HICON)CopyImage(hIcon, IMAGE_ICON, width, height, LR_DEFAULTSIZE);		
		}
	}
	DeleteObject(info.hbmColor);
	DeleteObject(info.hbmMask);

	if (m_hIcon)
	{
		hIcon = m_hIcon;
	}
	GetDlgItem(ctrlId).SendMessage(STM_SETICON, (WPARAM)hIcon, NULL);
	return hIcon != NULL;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


template<class T> HWND DialogBase<T>::Create(HWND hWndParent, LPARAM dwInitParam)
{
	BOOL result;

	ATLASSUME(m_hWnd == NULL);

	// Allocate the thunk structure here, where we can fail
	// gracefully.

	result = m_thunk.Init(NULL,NULL);
	if (result == FALSE) 
	{
		SetLastError(ERROR_OUTOFMEMORY);
		return NULL;
	}

	_AtlWinModule.AddCreateWndData(&m_thunk.cd, this);
#ifdef _DEBUG
	m_bModal = false;
#endif //_DEBUG

	HMODULE	hModule		= ::GetModuleHandle(0);
	HRSRC		hrsrc			= ::FindResource(hModule, MAKEINTRESOURCE(T::IDD), RT_DIALOG);
	HGLOBAL	hglobal		= ::LoadResource(hModule, hrsrc);
	
	if (	hModule	== NULL 
		|| hrsrc		== NULL 
		|| hglobal	== NULL)
	{
		return NULL;
	}

	setDialogFontSize(hglobal);

	HWND hWnd = ::CreateDialogIndirectParam(_AtlBaseModule.GetResourceInstance(), 
															(LPCDLGTEMPLATE)hglobal, hWndParent, 
															T::StartDialogProc, dwInitParam);

	ATLASSUME(m_hWnd == hWnd);
	::FreeResource(hglobal);
	return hWnd;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

template<class T> void DialogBase<T>::setDialogFontSize(HGLOBAL hglobal)
{
	LPVOID	lpResource	= ::LockResource(hglobal);
	bool		foundFont	= false;
	WORD		fontSize		= theApp.getDialogSize();
	
	if (lpResource == NULL)
	{
		UnlockResource(hglobal);
		return;		
	}
	WORD* resPointer		= (WORD*)lpResource;
	WORD* startPointer	= resPointer;

	resPointer += 6;

	if (*resPointer & DS_SETFONT)
	{	
		resPointer += 5;

		for (int i = 0; i < 3; i++)
		{	
			if ((*resPointer) == 0xffff)
			{			
				// There's one other element which specifies
				// the ordinal value of the resource as DWORD
				resPointer+=2;
			}
			else
			{
				// It's a string specifying the 
				// name of a resource
				while ((*resPointer++) != 0x0000);
			}
		}
		DWORD		oldProtect;
		BOOL		res	= true;
		SIZE_T	size	= (resPointer - startPointer) * 2;
		if ((res = VirtualProtect(startPointer, size, PAGE_READWRITE, &oldProtect)))
		{
			*resPointer = fontSize;
			VirtualProtect(startPointer, size, oldProtect, &oldProtect);
		}
	}
	UnlockResource(hglobal);
}

#endif