
/**
 * Class name:    WindowUtils

 * Description:   This helper class facilitates working with windows and dialogs. 

 * Author:        Frank Dockhorn
 */

#ifndef WINDOWDIALOG_UTILS
#define WINDOWDIALOG_UTILS

class WindowUtils
{
public:
	enum DlgSizes
	{
		VERY_SMALL,
		SMALL,
		NORMAL,
		LARGE,
		EXTRA_LARGE		
	};
	static BOOL checkDWMStatusAndDisableNCRendering(HWND hwnd);
	static const unsigned getDialogSize(unsigned idx);
	static const unsigned DLG_SIZE_MIN;
	static const unsigned DLG_SIZE_MAX;
	static const unsigned DLG_NUM_SIZES;	
private:
	static const unsigned s_numSizes[];
};

#endif