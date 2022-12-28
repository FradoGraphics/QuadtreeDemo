/**
 * Class name:    QuadtreeInfoDialog

 * Description:   Allows the user to modify quad tree properties such as depth 
                  and maximum number of triangles per region. Furthermore meshes
                  in *.tri format can be opened.

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "freeglut_std.h"
#include "freeglut_ext.h"
#include "QuadTreeData.h"
#include "QuadtreeInfoDialog.h"
#include "AboutDialog.h"
#include "RenderData.h"
#include "StreamUtils.h"
#include "QuadtreeApp.h"


extern QuadtreeApp theApp;

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


QuadtreeInfoDialog::QuadtreeInfoDialog() 
: 
			m_depth					(QuadTreeData::DEPTH_DEFAULT_VALUE),
			m_numTris				(QuadTreeData::NUMTRI_DEFAULT_VALUE),
			m_ratio					(QuadTreeData::RATIO_DEFAULT_VALUE),
			m_isSizeBoundEnabled	(true),
			m_isInitialized		(false),
			m_currentRadioID		(0),
			m_lastChangedControl	(0),
			m_currentNumTris		(0),
			m_currentRatio			(0.0),
			m_arrowSymbol			(_T("^")),
			m_hFont					(NULL),
			m_hFontArrow			(NULL),
			m_hBrushGreen			(NULL),
			m_hBrushRed				(NULL)
{
	 m_cursor.x = m_cursorInfo.x = 0;
	 m_cursor.y = m_cursorInfo.y = 0;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadtreeInfoDialog::~QuadtreeInfoDialog()
{
	DeleteObject(m_hFont);
	DeleteObject(m_hFontArrow);
	DeleteObject(m_hBrushGreen);
	DeleteObject(m_hBrushRed);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, 
                                     BOOL& bHandled)
{	
	m_glutHwnd = GetParent();
	DialogBase<QuadtreeInfoDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	
	if (m_glutHwnd == NULL)
	{
		// skip time consuming initialization steps
		ShowWindow(SW_HIDE);	
		return TRUE;
	}

	m_isInitialized = false;
	initFontsAndColors();
	initControls();
	m_isInitialized = true;
   return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnExitMove(UINT uMsg, WPARAM wParam, LPARAM lParam, 
													  BOOL& bHandled)
{	
	theApp.infoDlgPosChanged();
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::updateControls()
{
	tstringstream input;
	input << std::fixed << setprecision(2) << m_ratio;
	GetDlgItem(IDC_EDIT_RATIO).SendMessage(WM_SETTEXT, 0, (LPARAM)input.str().c_str());
	SetDlgItemInt(IDC_STATIC_TRIANGLES_TOTAL, 0, FALSE);

	resetResults();

	// Set the initial value
	GetDlgItem(IDC_SPIN_DEPTH).SendMessage(UDM_SETPOS, 0L, MAKELONG(m_depth, 0));
	// Set the initial value
	GetDlgItem(IDC_SPIN_SIZE).SendMessage(UDM_SETPOS, 0L, MAKELONG(m_numTris, 0));

	if (m_isSizeBoundEnabled)
	{
		m_currentRadioID = IDC_RADIO_ABS_NUM;
		CheckRadioButton(IDC_RADIO_ABS_NUM, IDC_RADIO_RATIO, IDC_RADIO_ABS_NUM);
		GetDlgItem(IDC_EDIT_RATIO).EnableWindow(FALSE);	
		GetDlgItem(IDC_EDIT_SIZE).EnableWindow(TRUE);
	}
	else
	{
		m_currentRadioID = IDC_RADIO_RATIO;
		CheckRadioButton(IDC_RADIO_ABS_NUM, IDC_RADIO_RATIO, IDC_RADIO_RATIO);
		GetDlgItem(IDC_EDIT_SIZE).EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO).EnableWindow(TRUE);
	}	
	GetDlgItem(IDAPPLY).EnableWindow(FALSE);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::initControls()
{
	HWND hSpinDepth = GetDlgItem(IDC_SPIN_DEPTH);
	HWND hEditDepth = GetDlgItem(IDC_EDIT_DEPTH);

	SendMessage(hSpinDepth, UDM_SETBUDDY, (WPARAM)hEditDepth, 0);
	SendMessage(hSpinDepth, UDM_SETRANGE, 0L, MAKELONG(QuadTreeData::UPPER_DEPTH_BOUND, 
																		QuadTreeData::LOWER_DEPTH_BOUND));

	HWND hSpinSize = GetDlgItem(IDC_SPIN_SIZE);
	HWND hEditSize = GetDlgItem(IDC_EDIT_SIZE);

	SendMessage(hSpinSize, UDM_SETBUDDY, (WPARAM)hEditSize, 0);
	SendMessage(hSpinSize, UDM_SETRANGE, 0L, MAKELONG(QuadTreeData::UPPER_NUMTRI_BOUND, 
																	  QuadTreeData::LOWER_NUMTRI_BOUND));

	updateControls();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::initFontsAndColors()
{
	LOGFONT	lf;

	CWindow& wnd		= GetDlgItem(IDC_STATIC_NUM_TRIANGLES);
	CWindow& wnd1		= GetDlgItem(IDC_STATIC_ARROW);
	CWindow& wnd2		= GetDlgItem(IDC_STATIC_PERCENT);
	CWindow& wnd3		= GetDlgItem(IDC_STATIC_CURRENT_DEPTH);
	CWindow& wnd4		= GetDlgItem(IDC_STATIC_MAX_DEPTH);
	CWindow& wnd5		= GetDlgItem(IDC_STATIC_CONFORMITY);
	CWindow& wnd6		= GetDlgItem(IDC_STATIC_SEARCHRESULTS);
	CWindow& wnd7		= GetDlgItem(IDC_STATIC_MESH_INFO);
	CWindow& wnd8		= GetDlgItem(IDC_STATIC_QUADTREE_PARAM);
	CWindow& wnd9		= GetDlgItem(IDC_STATIC_QUADTREE_RESULTS);
	CWindow& wnd10		= GetDlgItem(IDC_STATIC_MEDIAN);

	HFONT		hFont		= wnd.GetFont();	
	GetObject(hFont, sizeof(LOGFONT), &lf);
	lf.lfWeight			= FW_SEMIBOLD;	
	m_hFont				= CreateFontIndirect(&lf);
	lf.lfHeight			= -18;
	m_hFontArrow		= CreateFontIndirect(&lf);

	if (m_hFont)
	{
		wnd.SetFont(m_hFont);
		wnd2.SetFont(m_hFont);
		wnd3.SetFont(m_hFont);
		wnd4.SetFont(m_hFont);
		wnd5.SetFont(m_hFont);
		wnd6.SetFont(m_hFont);
		wnd7.SetFont(m_hFont);
		wnd8.SetFont(m_hFont);
		wnd9.SetFont(m_hFont);
		wnd10.SetFont(m_hFont);
	}
	if (m_hFontArrow)
	{
		wnd1.SetFont(m_hFontArrow);
	}

	m_hBrushRed		= CreateSolidBrush(RGB(255, 190, 190));
	m_hBrushGreen	= CreateSolidBrush(RGB(190, 255, 190));
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void	QuadtreeInfoDialog::setValues(const QuadTreeData* data)
{	
	m_depth							= data->getMaxDepth();
	m_ratio							= data->getRatio();
	m_numTris						= data->getSizeBound();
	m_isSizeBoundEnabled			= data->isSizeBoundEnabled();	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void	QuadtreeInfoDialog::getValues(QuadTreeData* data)
{
	data->setMaxDepth(m_depth);
	data->setRatio(m_ratio);
	data->setSizeBound(m_numTris);

	if (IsDlgButtonChecked(IDC_RADIO_ABS_NUM))
	{
		data->setSizeBoundState(true);
	}		
	else
	{
		data->setSizeBoundState(false);
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{   
   return DialogBase<QuadtreeInfoDialog>::OnDestroy(uMsg, wParam, lParam, bHandled);   
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnEscape(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
   theApp.exitDemo();
   return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnToggleWindows(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
   HWND hWnd = GetActiveWindow();
   if (hWnd == m_hWnd)
   {
		::GetCursorPos(&m_cursorInfo);
		if (m_cursor.x && m_cursor.y)
		{
			// restore previous cursor position
			::SetCursorPos(m_cursor.x, m_cursor.y);
		}		
		SetForegroundWindow(m_glutHwnd);
   }
   else if (hWnd == m_glutHwnd)
   {
		activateInfoDialog();
   }
   return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::activateInfoDialog()
{	
	RECT	rc;
	POINT point;
	GetWindowRect(&rc);
	// save previous cursor position
	::GetCursorPos(&m_cursor);

	if (m_cursorInfo.x && m_cursorInfo.y && PtInRect(&rc, m_cursorInfo))
	{
		// restore previous cursor position
		::SetCursorPos(m_cursorInfo.x, m_cursorInfo.y);
	}		
	else
	{
		// compute new cursor position		
		point.x = (rc.right + rc.left) / 2;
		point.y = (rc.bottom + rc.top) / 2;		
		::SetCursorPos(point.x, point.y);
	}
	SetForegroundWindow(m_hWnd);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::resetResults()
{
	m_currentRatio		= 0.0;
	m_currentNumTris	= 0;
	SetDlgItemText(IDC_STATIC_CURRENT_DEPTH, _T("-"));
	SetDlgItemText(IDC_STATIC_MAX_DEPTH, _T("-"));
	SetDlgItemInt(IDC_STATIC_NUM_TRIANGLES, 0, FALSE);
	SetDlgItemInt(IDC_STATIC_PERCENT, 0, FALSE);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeInfoDialog::updateSearchResults(const RenderData* data,
															char					depth,
															char					finalDepth)
{
	tstringstream input;

	size_t totalNumTris	= data->getMesh().getIndexArray().size() / 3; 
	m_currentNumTris		= (unsigned)data->getIndexResults()->size(); 
	m_currentRatio			= ((REAL)data->getIndexResults()->size() / totalNumTris) * 100.0;
	input << std::fixed << setprecision(3) << m_currentRatio;

	// round value up to three decimal places
	roundValue(m_currentRatio, THREE);
	
	if (finalDepth == 0)
	{
		SetDlgItemText(IDC_STATIC_CURRENT_DEPTH, _T("-"));
		SetDlgItemText(IDC_STATIC_MAX_DEPTH, _T("-"));
	}
	else
	{
		SetDlgItemInt(IDC_STATIC_CURRENT_DEPTH, (UINT)depth, FALSE);
		SetDlgItemInt(IDC_STATIC_MAX_DEPTH, (UINT)finalDepth, FALSE);
	}
	SetDlgItemInt(IDC_STATIC_NUM_TRIANGLES, (UINT)data->getIndexResults()->size(), FALSE);
	SetDlgItemText(IDC_STATIC_PERCENT, input.str().c_str());
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::enableControls(bool val)
{
	if (val == false)
	{
		GetDlgItem(IDC_EDIT_SIZE).EnableWindow(val);
		GetDlgItem(IDC_EDIT_RATIO).EnableWindow(val);
	}
	GetDlgItem(IDC_RADIO_ABS_NUM).EnableWindow(val);
	GetDlgItem(IDC_RADIO_RATIO).EnableWindow(val);
	GetDlgItem(IDRESET_VIEW).EnableWindow(val);
	GetDlgItem(IDC_EDIT_DEPTH).EnableWindow(val);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeInfoDialog::updateFileInfo(const RenderData*	data,
													 const QuadTreeData* treedata)
{
	tstringstream input;
	setValues(treedata);
	updateControls();
	size_t totalTris	= data->getMesh().getIndexArray().size() / 3;
	REAL length			= data->getBoundingBox()[1].x - data->getBoundingBox()[0].x;
	REAL width			= data->getBoundingBox()[1].y - data->getBoundingBox()[0].y;
	SetDlgItemInt(IDC_STATIC_TRIANGLES_TOTAL, (UINT)totalTris, FALSE);	
	input << std::fixed << setprecision(2) << length;
	SetDlgItemText(IDC_STATIC_LENGTH, input.str().c_str());
	StreamUtils::resetStream(input);
	input << std::fixed << setprecision(2) << width;
	SetDlgItemText(IDC_STATIC_WIDTH, input.str().c_str());
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void	QuadtreeInfoDialog::setDepthHint(bool val)
{
	if (val == true)
	{		
		SetDlgItemText(IDC_STATIC_ARROW, m_arrowSymbol.c_str());
	}
	else
	{
		SetDlgItemText(IDC_STATIC_ARROW, _T(""));
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::setDeviationPercent(REAL value)
{
	tstringstream input;
	input << std::fixed << setprecision(3) << value;
	tstring text((input.str().c_str()));
	SetDlgItemText(IDC_STATIC_CONFORMITY, input.str().c_str());
	setDepthHint(value == 0.0 ? false : true);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::setDeviationMedianPercent(REAL value)
{
	tstringstream input;
	input << std::fixed << setprecision(3) << value;
	tstring text((input.str().c_str()));
	SetDlgItemText(IDC_STATIC_MEDIAN, input.str().c_str());
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) 
{
	if (validateAndFormatData() && theApp.isDataLoaded())
	{			
		// save focus before disabling apply button	
		SendMessage(WM_NEXTDLGCTL, (WPARAM)GetDlgItem(m_lastChangedControl).m_hWnd, TRUE);
		GetDlgItem(IDAPPLY).EnableWindow(FALSE);	

		m_isSizeBoundEnabled = IsDlgButtonChecked(IDC_RADIO_ABS_NUM);
		theApp.rebuildAndSearchTree();
	}		
   return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnRadioNum(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) 
{
	if (m_isInitialized && m_currentRadioID != wID)
	{
		m_currentRadioID = wID;
		GetDlgItem(IDC_EDIT_SIZE).EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RATIO).EnableWindow(FALSE);
		OnChange(wNotifyCode, wID, hWndCtl, bHandled);
	}
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (	m_isInitialized 			
			&& (wID == IDC_EDIT_RATIO
			|| wID == IDC_EDIT_SIZE
			|| wID == IDC_EDIT_DEPTH
			|| wID == IDC_RADIO_ABS_NUM
			|| wID == IDC_RADIO_RATIO))
	{
		m_lastChangedControl	= wID;
		GetDlgItem(IDAPPLY).EnableWindow(TRUE);
	}
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnRadioRatio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) 
{
	if (m_isInitialized && m_currentRadioID != wID)
	{
		m_currentRadioID = wID;
		GetDlgItem(IDC_EDIT_SIZE).EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO).EnableWindow(TRUE);	
		OnChange(wNotifyCode, wID, hWndCtl, bHandled);
	}
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnResetView(WORD wNotifyCode, WORD wID, HWND hWndCtl, 
													 BOOL& bHandled) 
{
	if (!theApp.isDataLoaded())
	{
		return TRUE;
	}	   
	theApp.resetView();
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnZoomToRegion(WORD wNotifyCode, WORD wID, HWND hWndCtl, 
														 BOOL& bHandled) 
{
	if (!theApp.isDataLoaded())
	{
		return TRUE;
	}	   
	theApp.zoomToRegion();
	return TRUE;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeInfoDialog::validateAndFormatData()
{		
	TCHAR	buf[10];

	REAL		ratio		= 1.0;
	int		depth		= GetDlgItemInt(IDC_EDIT_DEPTH, NULL, FALSE);
	unsigned	numTris	= GetDlgItemInt(IDC_EDIT_SIZE, NULL, FALSE);
	BOOL		res		= GetDlgItemText(IDC_EDIT_RATIO, buf, 10);
	int		res2		= _stscanf_s(buf, _T(PREC_SCAN_2), &ratio);	
	bool		res3		= res == 0 || res2 == 0 || res2 == EOF;
	
	DecimalPlaces ratioPrec = TWO;
	roundValue(ratio, ratioPrec);

	// check percentage and triangle value
	if (IsDlgButtonChecked(IDC_RADIO_RATIO))
	{				
		if (	res3  
			|| ratio > QuadTreeData::UPPER_RATIO_BOUND 
			|| ratio < QuadTreeData::LOWER_RATIO_BOUND)
		{			
			// error	
			showMessageBox(IDS_ERROR_INVALID_PERCENT, IDS_ERROR_PROPS_INPUT, MB_OK | MB_ICONWARNING);
			tstringstream input;
			input << std::fixed << setprecision(ratioPrec) << m_ratio;
			GetDlgItem(IDC_EDIT_RATIO).SendMessage(WM_SETTEXT, 0, (LPARAM)input.str().c_str());
			SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_RATIO).m_hWnd, TRUE);					
			return false;
		}
		m_ratio = ratio;
	}
	else 
	{		
		if (	numTris < QuadTreeData::LOWER_NUMTRI_BOUND 
			|| numTris > QuadTreeData::UPPER_NUMTRI_BOUND)
		{		
			// error		
			showMessageBox(IDS_ERROR_INVALID_NUM_TRIS, IDS_ERROR_PROPS_INPUT, MB_OK | MB_ICONWARNING);			
			GetDlgItem(IDC_SPIN_SIZE).SendMessage(UDM_SETPOS, 0L, MAKELONG(m_numTris, 0));
			SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SIZE).m_hWnd, TRUE);
			return false;
		}
		m_numTris = numTris;
	}

	// check tree depth value
	if (	depth < QuadTreeData::LOWER_DEPTH_BOUND 
		|| depth > QuadTreeData::UPPER_DEPTH_BOUND)
   {      
		// error		
		showMessageBox(IDS_ERROR_INVALID_DEPTH, IDS_ERROR_PROPS_INPUT, MB_OK | MB_ICONWARNING);			
		GetDlgItem(IDC_SPIN_DEPTH).SendMessage(UDM_SETPOS, 0L, MAKELONG(m_depth, 0));
      SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_DEPTH).m_hWnd, TRUE);	      
		return false;
   }		
	m_depth = depth;

	if (m_lastChangedControl == IDC_EDIT_RATIO)
	{	
		// display reformatted value in edit field
		tstringstream input;
		input << std::fixed << setprecision(ratioPrec) << m_ratio;
		GetDlgItem(IDC_EDIT_RATIO).SendMessage(WM_SETTEXT, 0, (LPARAM)input.str().c_str());
	}

   return true;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::showMessageBox(UINT nText, UINT nCaption, UINT nType)
{
	theApp.showMessageBox(m_hWnd, nText, nCaption, nType);
	activateInfoDialog();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeInfoDialog::showMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	theApp.showMessageBox(m_hWnd, lpszText, lpszCaption, nType);
	activateInfoDialog();
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnOpenFile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
   // call file open dialog 
	TCHAR tempPath[MAX_PATH];	
   if (!openFileDialog(tempPath, MAX_PATH))
   {         
      return -1;
   }   
   theApp.load(tempPath);
	SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDAPPLY).m_hWnd, TRUE);
   return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


LRESULT QuadtreeInfoDialog::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AboutDialog dlg;
	if (dlg.DoModal(m_hWnd) == IDOK)
	{
		USHORT res = dlg.getDialogSize();
		theApp.setDialogSize(res);
	}
	return TRUE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

BOOL QuadtreeInfoDialog::enableParentWindows(BOOL val)
{
	BOOL res1 = ::EnableWindow(m_hWnd, val);
	BOOL res2 = ::EnableWindow(m_glutHwnd, val);	
	return res1 && res2;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

LRESULT QuadtreeInfoDialog::OnCtrlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
	HDC	hdc	= (HDC)wParam;     	
	UINT	id		= ::GetDlgCtrlID((HWND)lParam); 
	return (LRESULT)updateBackgroundColor(id, hdc);    
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

HBRUSH QuadtreeInfoDialog::updateBackgroundColor(UINT id, HDC hdc)
{
	LOGBRUSH logBrush;
	HBRUSH	currentBrush = NULL;

	if (m_currentRadioID == IDC_RADIO_RATIO && id == IDC_STATIC_PERCENT)
	{
		if (m_currentRatio > 0.0)
		{
			currentBrush = m_hBrushRed;
			if (m_currentRatio <= m_ratio)
			{
				currentBrush = m_hBrushGreen;
			}
		}
	}
	else if (m_currentRadioID == IDC_RADIO_ABS_NUM && id == IDC_STATIC_NUM_TRIANGLES)
	{
		if (m_currentNumTris > 0)
		{		
			currentBrush = m_hBrushRed;
			if (m_currentNumTris <= m_numTris)
			{
				currentBrush = m_hBrushGreen;
			}
		}
	}	

	if (currentBrush)
	{	
		GetObject( currentBrush, sizeof(LOGBRUSH), &logBrush);
		::SetBkColor(hdc, logBrush.lbColor);  
	}

	return currentBrush;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool QuadtreeInfoDialog::openFileDialog(TCHAR* tempPath, DWORD size)
{
   OPENFILENAME ofn;				     

   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize      = sizeof(ofn);
   ofn.hwndOwner        = m_hWnd;
   ofn.lpstrFile        = tempPath;
   ofn.lpstrFile[0]     = '\0';
   ofn.nMaxFile         = size;
   ofn.lpstrFilter      = _T("Wavefront obj (*.obj)\0*.obj\0\0");
   ofn.nFilterIndex     = 1;
   ofn.lpstrFileTitle   = NULL;
   ofn.nMaxFileTitle    = 0;
   ofn.lpstrInitialDir  = _T(".\\");   
   ofn.Flags            = OFN_PATHMUSTEXIST 
                        | OFN_FILEMUSTEXIST 
                        | OFN_ENABLEHOOK 
                        | OFN_EXPLORER 
                        | OFN_ENABLESIZING
								| OFN_NOCHANGEDIR;


   // Display the Open dialog box. 
   enableParentWindows(FALSE);
   BOOL res = theApp.getOpenFileName(&ofn);
	enableParentWindows(TRUE);
	
   if (res == TRUE) 
   {                  
      return true;
   }   
   return false;
}



