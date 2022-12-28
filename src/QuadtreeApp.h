/**
 * Class name:    QuadtreeApp

 * Description:   The central class that implements the flow of control.
                  
                  It initializes window size, camera properties and clipping perspective.
                  All callback functions for user interaction are implemented here.
                  For these purposes the OpenGL freeglut library is used.
                  This class also loads mesh data and processes them for further rendering.

 * Author:        Frank Dockhorn
 */


#ifndef CQUADTREE_APP
#define CQUADTREE_APP


class QuadTreeData;
class OpenGLRenderer;
class QuadtreeInfoDialog;
class QuadTree;

class QuadtreeApp
{
public:
   QuadtreeApp();
   ~QuadtreeApp();
   int			init						(int argc, char* argv[]);
	int			showMessageBox			(	HWND hwndParent, 
													UINT nText, 
													UINT nCaption, 
													UINT nType);
	int			showMessageBox			(	HWND		hwndParent, 
													LPCTSTR	lpszText, 
													LPCTSTR	lpszCaption, 
													UINT		nType);
	unsigned		getDialogSize			();
	unsigned		getNewDialogSize		();
   void			drawScene				();
   void			resizeFunc				(int width, int height);
   void			motionFunc				(int x, int y);
	void			closeFunc				();
	void			errorFunc				(const char* string, va_list ap);
   void			mouseFunc				(int button, int state, int x, int y);
   void			specialKeyFunc			(int key, int x, int y);
   void			keyFunc					(unsigned char k, int x, int y);
	void			positionFunc			(int x, int y);
	void			repaintScene			(HWND hwnd, bool checkForOverlaps = true);
	void			infoDlgPosChanged		();
	void			rebuildAndSearchTree	();
	void			resetView				();
	void			zoomToRegion			();
   void			exitDemo					();
   void			load						(const TCHAR* file);
	void			setDialogSize			(USHORT size);
	bool			isDataLoaded			() {return m_isDataLoaded;}	
	BOOL			getOpenFileName		(OPENFILENAME* ofn);	
	tstring		getVersion				() {return m_version;}
	astring		getVersionA				() {return m_versionA;}
	const char* getOpenGlVersion		();

protected:
	class WindowInfo
	{
	public:
		WindowInfo();
		POINT			m_glutFrameSize;		// contains difference between window and client rectangle
		POINT			m_glutPos;				// contains glut window position
		POINT			m_infoPos;				// contains info dialog position
		POINT			m_glutClientSize;		// contains width, height of glut window client area
		POINT			m_dialogSize;			// contains width, height of info dialog
		POINT			m_glutBorderSize;		// contains border size of glut window
		int			m_glutMax;				// contains maximized state of glut window
		unsigned		m_DlgSnapStatus;		// contains info dialog snap status
		USHORT		m_dialogSizeValue;	// contains value for dialog size setting
		USHORT		m_newDialogSizeVal;	// contains value for new dialog size setting
		bool			m_resizeWnd;			// contains temporary status of window resize operation

		static const int		SNAP_SENSITIVITY;	
		static const int		INVALID_POSITION;
		static const USHORT	DEFAULT_FONT_SIZE;
	};

	class SysParamInfo
	{
	public:
		SysParamInfo();
		void readSysParamInfo();
		void getString(tstringstream& out);
		POINT			m_dpiValue;				// contains display resolution as dpi value
		RECT			m_rectWorkArea;		// contains dimensions of working area
	};
	enum WndPosStatus
	{
		POS_DEFAULT		= 0,
		POS_RESTORE		= 0x01,
		POS_MOVE			= 0x02,
		POS_SNAP			= 0x04,
		POS_SNAP_LEFT	= 0x08 | POS_SNAP,
		POS_SNAP_RIGHT = 0x10 | POS_SNAP,
	};
	
	void		enableMesaModeAndRestart	();
	void		checkForMesaGL					();
	void		setWindowIcon					();
	void		initGlutBorderSize			();
	void		initVersionStrings			();
	void		adaptToDisplayResolution	();
	void		setForegroundWindow			();
	void		displayLastErrorMessage		(UINT nID, const TCHAR* path = NULL);
	void		displayLastErrorMessage		(LPTSTR message, const TCHAR* path = NULL);
	void		processInitError				(const char* message, bool showOpenGL = false);
	void		showInitErrorMessage			(const char* txt);
	void		updateCamera					(vec3& cameraPos);
	void		createUserdatPath				();
	void		buildTree						();
	void		findTriangles					();
	void		writeSettingsToIni			();
	void		updateFileStatus				();
	void		writeSettingsToFile			();	
   void     computeBorderDiffs			();
   void		setPerspective					();   
	void		readSettingsFromIni			(tstring& tempPath);
	void		readSettingsFromFile			();
	void		readSysParamInfo				();
	void		positionDialog					(int x, int y);
	bool		openIniFile							();
   bool		computeCursorPos				(int x, int y);
	bool		openFileDialog					(tstring& chosenFile);	
   bool		loadData							(const TCHAR* file);
   bool		checkData						();
	bool		fileExists						(LPCTSTR szPath);
	bool		directoryExists				(LPCTSTR szPath);
	bool		initWindowAndDialog			();
	bool		applyDialogSize				();
	bool		doInitWindowAndDialog		();
	bool		computeCenterPosition		(float ratio);
	bool		createUniqueWindowCaption	(astring& caption);
	int		showMessageBox					(LPCTSTR lpszText, UINT nCaption, UINT nType);
	int		showMessageBox					(LPCTSTR	lpszText, LPCTSTR	lpszCaption, UINT nType);
	int		showMessageBox					(UINT nText, UINT nCaption, UINT nType);
	int		showMessageBox					(const char* text, const char* caption, UINT nType);

	tstring	getLastErrorAsString			(const TCHAR* location);
private:
	WindowInfo				m_wndInfo;	
	SysParamInfo			m_sysParamInfo;
   OpenGLRenderer*      m_renderer;
   int                  m_glutWnd;
	unsigned					m_wndPosStatus;
   HWND                 m_hwnd;
   HWND                 m_infoDlgHwnd;
   QuadtreeInfoDialog*  m_infoDialog;
   QuadTree*            m_quadtree;
   QuadTreeData*        m_quadtreeData;
   vec3						m_cursorPos; 
   vec2						m_prevMousePos; 
   vec2						m_startMousePos;
	Vec2D						m_cursorPos2D; 
   bool                 m_leftMButtonDown;
	bool						m_leftMButtonClicked;
	bool						m_isDataLoaded;
	bool						m_isIniFileRead;
	bool						m_isUserdatFileRead;
	bool						m_useDefaultPosition;
	bool						m_useLegacyPaintMode;
	bool						m_autoLoad;
	bool						m_repaintScene;
	bool						m_isCompositionEnabled;
	tstring					m_filePath;
	tstring					m_defaultPath;
	tstring					m_userdatPath;
	tstring					m_version;
	astring					m_versionA;
	astring					m_glutErrorMessage;	
	
	static const TCHAR*	INI_PATH;
	static const TCHAR*	USER_PATH;
	static const TCHAR*	WND_CHAPTER;
	static const TCHAR*	RENDER_CHAPTER;
	static const TCHAR*	QUAD_CHAPTER;
	static const TCHAR*	SYSPARAM_CHAPTER;
	static const TCHAR*	ERRORVBO;
};

#endif