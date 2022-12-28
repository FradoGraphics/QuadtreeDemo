/**
 * Class name:    QuadtreeInfoDialog

 * Description:   Allows the user to modify quad tree properties such as depth 
                  and maximum number of triangles per region. Furthermore meshes
                  in *.tri format can be opened.

 * Author:        Frank Dockhorn
 */


#ifndef QUADTREEINFO_DIALOG
#define QUADTREEINFO_DIALOG

#include "resource.h"
#include "DialogBase.h"

class RenderData;
class QuadTreeData;
class QuadtreeApp;


class QuadtreeInfoDialog : public DialogBase<QuadtreeInfoDialog>
{
public:
	enum { IDD = IDD_DIALOG_INFO };

	BEGIN_MSG_MAP(QuadtreeInfoDialog)
		MESSAGE_HANDLER		(WM_CTLCOLORSTATIC, OnCtrlColor)
		MESSAGE_HANDLER		(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER		(WM_DESTROY, OnDestroy)		
		MESSAGE_HANDLER		(WM_EXITSIZEMOVE, OnExitMove)		
		CHAIN_MSG_MAP			(DialogBase<QuadtreeInfoDialog>)
		COMMAND_ID_HANDLER	(IDAPPLY, OnApply)
		COMMAND_ID_HANDLER	(IDRESET_VIEW, OnResetView)
		COMMAND_ID_HANDLER	(IDOPEN_FILE, OnOpenFile)      
		COMMAND_ID_HANDLER	(IDABOUT, OnAbout)      
		COMMAND_ID_HANDLER	(ID_ESCAPE, OnEscape)
		COMMAND_ID_HANDLER	(ID_ZOOM_TO_REGION, OnZoomToRegion)
		COMMAND_ID_HANDLER	(ID_TOGGLE_WINDOWS, OnToggleWindows)
		COMMAND_ID_HANDLER	(ID_RESET_VIEW, OnResetView)
		COMMAND_ID_HANDLER	(IDC_RADIO_ABS_NUM, OnRadioNum)
		COMMAND_ID_HANDLER	(IDC_RADIO_RATIO, OnRadioRatio)	
		COMMAND_CODE_HANDLER	(EN_CHANGE, OnChange)
	END_MSG_MAP()

	QuadtreeInfoDialog						();
	~QuadtreeInfoDialog						();
	LRESULT  OnInitDialog					(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT	OnCtrlColor						(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT  OnExitMove						(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  OnDestroy						(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  OnApply							(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnResetView						(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnZoomToRegion					(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnEscape							(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnToggleWindows				(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT  OnOpenFile						(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT  OnAbout							(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT	OnRadioNum						(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT	OnRadioRatio					(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT	OnChange							(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void		setDepthHint					(bool val);
	void		setDeviationPercent			(REAL value);
	void		setDeviationMedianPercent	(REAL value);
	void		setValues						(const QuadTreeData* data);
	void		getValues						(QuadTreeData* data);
	void		updateSearchResults			(const RenderData* data, char depth, char finalDepth);
	void		updateFileInfo					(const RenderData* data, const QuadTreeData* treedata);
	void		enableControls					(bool val);
	void		resetResults					();

protected:

	bool		validateAndFormatData		();   
	bool		openFileDialog					(TCHAR* tempPath, DWORD size);
	BOOL		enableParentWindows			(BOOL val);
	void		showMessageBox					(LPCTSTR lpszText, LPCTSTR lpszCaption = _T(""), 
																						UINT nType = MB_OK);
	void		showMessageBox					(UINT nText, UINT nCaption, UINT nType);
	void		activateInfoDialog			();
	void		initFontsAndColors			();
	void		initControls					();
	void		updateControls					();
	HBRUSH	updateBackgroundColor		(UINT id, HDC hdc);

private:
	HBRUSH			m_hBrushRed;
	HBRUSH			m_hBrushGreen;
	HFONT				m_hFont;
	HFONT				m_hFontArrow;
	POINT				m_cursor;
	POINT				m_cursorInfo;
   unsigned			m_numTris;
	unsigned			m_currentNumTris;
   int				m_depth;   	
	bool				m_isSizeBoundEnabled;
	bool				m_isInitialized;
	WORD				m_currentRadioID;
	WORD				m_lastChangedControl;
	REAL				m_ratio;	
	REAL				m_currentRatio;
   HWND				m_glutHwnd;   
	tstring			m_arrowSymbol;
};

#endif