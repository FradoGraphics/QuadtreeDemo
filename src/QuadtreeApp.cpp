/**
 * Class name:    QuadtreeApp

 * Description:   The central class that implements the flow of control.
                  
                  It initializes window size, camera properties and clipping perspective.
                  All callback functions for user interaction are implemented here.
                  For these purposes the OpenGL freeglut library is used.
                  This class also loads mesh data and processes them for further rendering.

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "freeglut_std.h"
#include "freeglut_ext.h"
#include "QuadTree.h"
#include <math.h>
#include "QuadtreeApp.h"
#include "AppMessageHook.h"
#include "StreamUtils.h"
#include "WindowUtils.h"
#include "OpenGLDialog.h"
#include "MsgBoxDialog.h"
#include "QuadTreeData.h"
#include "QuadtreeInfoDialog.h"
#include "OpenGLRenderer.h"
#include "version.h"

#if defined FREEGLUT_RUNTIME_LINKING
	#include "FreeglutExtensions.h"
#endif

void errorFunc				(const char* string, va_list ap);
void keyFunc				(unsigned char k, int x, int y);
void specialKeyFunc		(int key, int x, int y);
void mouseFunc				(int button, int state, int x, int y);
void positionFunc			(int x, int y);
void resizeFunc			(int width, int height);
void motionFunc			(int x, int y);
void closeFunc				();
void drawFunc				();
void CALLBACK PollTimer	(HWND hwnd, UINT uMsg, UINT_PTR idTimer, DWORD dwTime);

extern QuadtreeApp theApp;

#pragma comment(lib, "comctl32.lib")

const TCHAR*	QuadtreeApp::INI_PATH			= _T(".\\QuadtreeDemo.ini");
const TCHAR*	QuadtreeApp::USER_PATH			= _T(".\\userdat");
const TCHAR*	QuadtreeApp::WND_CHAPTER		= _T("Window Settings");
const TCHAR*	QuadtreeApp::RENDER_CHAPTER   = _T("Render Settings");
const TCHAR*	QuadtreeApp::QUAD_CHAPTER		= _T("Quadtree Settings");
const TCHAR*	QuadtreeApp::SYSPARAM_CHAPTER	= _T("SysParam Settings");

const USHORT	QuadtreeApp::WindowInfo::DEFAULT_FONT_SIZE	= 10;
const int		QuadtreeApp::WindowInfo::SNAP_SENSITIVITY		= 40;
const int		QuadtreeApp::WindowInfo::INVALID_POSITION		= -32000;

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadtreeApp::WindowInfo::WindowInfo()
:	m_DlgSnapStatus		(POS_SNAP_RIGHT),
	m_dialogSizeValue		(DEFAULT_FONT_SIZE),
	m_newDialogSizeVal	(DEFAULT_FONT_SIZE),
	m_glutMax				(0),	
	m_resizeWnd				(false)	
{				
	m_glutFrameSize.x			= 
	m_glutFrameSize.y			= 
	m_dialogSize.x				= 
	m_glutBorderSize.x		=
	m_glutBorderSize.y		=
	m_dialogSize.y				= 0;
	m_glutClientSize.x		= 730;
	m_glutClientSize.y		= 550;		
	m_glutPos.x					= (GetSystemMetrics(SM_CXSCREEN) - m_glutClientSize.x) / 2;
	m_glutPos.y					= (GetSystemMetrics(SM_CYSCREEN) - m_glutClientSize.y) / 2;   
	m_infoPos.x					= m_glutPos.x + m_glutClientSize.x;	
	m_infoPos.y					= m_glutPos.y;	
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadtreeApp::SysParamInfo::SysParamInfo()
{
	readSysParamInfo();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::SysParamInfo::readSysParamInfo()
{		
	SystemParametersInfo(SPI_GETWORKAREA, 0, &m_rectWorkArea, 0);
	HDC dc = ::GetDC(NULL);
	m_dpiValue.x = GetDeviceCaps(dc, LOGPIXELSX);
	m_dpiValue.y = GetDeviceCaps(dc, LOGPIXELSY);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::SysParamInfo::getString(tstringstream& out)
{
	out << m_rectWorkArea.top		<< _T("_") << m_rectWorkArea.left  << _T("_")
		 << m_rectWorkArea.bottom	<< _T("_") << m_rectWorkArea.right << _T("_")
		 << m_dpiValue.x				<< _T("_") << m_dpiValue.y;

}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadtreeApp::QuadtreeApp() 
:  m_glutWnd					(-1),
   m_hwnd						(NULL),
   m_infoDlgHwnd				(NULL),	
	m_infoDialog				(NULL),	
	m_quadtree					(NULL),
	m_wndPosStatus				(POS_SNAP_RIGHT),
	m_isIniFileRead			(false),
	m_isUserdatFileRead		(false),
   m_leftMButtonDown			(false),
	m_isDataLoaded				(false),
	m_leftMButtonClicked		(false),
	m_useDefaultPosition		(true),
	m_useLegacyPaintMode		(true),
	m_repaintScene				(false),
	m_autoLoad					(true),
	m_isCompositionEnabled	(true),
	m_glutErrorMessage		(""),
	m_userdatPath				(_T(""))
{		
	m_defaultPath		= _T(".\\examples\\mesh_1.obj");	
	m_infoDialog		= new QuadtreeInfoDialog();
	m_renderer			= new OpenGLRenderer();
	m_quadtreeData		= new QuadTreeData();
	m_quadtree			= new QuadTree();
	initVersionStrings();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


QuadtreeApp::~QuadtreeApp() 
{ 	
	delete m_infoDialog;
	delete m_renderer;
	delete m_quadtreeData;
	delete m_quadtree;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::init(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC	= ICC_LINK_CLASS | ICC_STANDARD_CLASSES;
	if (!InitCommonControlsEx(&iccx))
	{
		CString message, message2;
		message.LoadString(IDS_ERROR_COMCTRL);
		message2.LoadString(IDS_ERROR_COMCTRL2);
		message += message2;
		showMessageBox(message, IDS_CAPTION_ERROR, 
			MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		return 0;
	}
	checkForMesaGL();
#ifdef FREEGLUT_RUNTIME_LINKING	
	if (!FreeglutExtensions::loadLibrary())
	{
		showInitErrorMessage("Freeglut could not be initialized.");
		return 0;
	}
#endif

	glutInit(&argc, argv);   
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitContextVersion(MAJOR_VERSION_GL, MINOR_VERSION_GL);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutInitErrorFunc(::errorFunc);
	tstring tempPath = m_defaultPath;
	readSettingsFromIni(tempPath);

	if (!m_isIniFileRead)
	{
		adaptToDisplayResolution();
	}
	if (!initWindowAndDialog())
	{
		showInitErrorMessage("GUI could not be initialized.");
		return 0;
	}
	setForegroundWindow();

	if (!m_renderer->checkOpenGLVersion())
	{      		
		processInitError(NULL, true);
		return 0;
	}
	if (!m_glutErrorMessage.empty())
	{
		processInitError(m_glutErrorMessage.c_str());
		return 0;
	}
	if(!m_renderer->buildShaderProgram())
	{
		processInitError(m_renderer->getInfoLog());
		return 0;
	}
	glutCloseFunc(::closeFunc);	

	m_renderer->initRenderStates();
	glutDisplayFunc(::drawFunc);
	glutReshapeFunc(::resizeFunc);
	glutPositionFunc(::positionFunc);	

	AppMessageHook::setGlutHwnd(m_hwnd);
	m_infoDialog->enableControls(false);
	if (!tempPath.empty())
	{
		if (!m_autoLoad)
		{
			CString text;		
			text.Format(IDS_AUTOLOAD, tempPath.c_str());
			if (showMessageBox(text, IDS_REOPEN, 
					MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				m_autoLoad = true;
			}
		}
		if (m_autoLoad)
		{
			updateFileStatus();
			load(tempPath.c_str());	
		}		
	}	
	m_wndPosStatus = m_wndInfo.m_DlgSnapStatus;
	glutMainLoop();

#if defined FREEGLUT_RUNTIME_LINKING
	FreeglutExtensions::freeLibrary();
#endif
	return 0;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::enableMesaModeAndRestart()
{
	TCHAR buf[MAX_PATH];
	if (!GetModuleFileName(NULL, buf, MAX_PATH))
	{
		displayLastErrorMessage(IDS_MODULE_FAILED);
		return;
	}
	tstring	str(buf);
	size_t	pos			= str.find_last_of(_T("/\\"));
	tstring	path			= str.substr(0, pos);
	tstring	pathDest		= path;
	tstring	pathSource	= path;
	pathDest		+= _T("\\opengl32.dll");	
	pathSource	+= _T("\\MesaGL\\opengl32.dll");	

	bool	resCopy	= false;	
	DWORD res		= ::GetFileAttributes(pathSource.c_str());
	if (res != INVALID_FILE_ATTRIBUTES)
	{
		resCopy = ::CopyFile(pathSource.c_str(), pathDest.c_str(), FALSE);		
	}
	
	if (!resCopy)
	{		
		displayLastErrorMessage(IDS_COPY_FAILED, pathSource.c_str());
		return;
	}

	// restart application
	int result = -1;
	result = _tspawnlp(_P_NOWAIT, buf, buf, NULL);
	if (result == -1)
	{
		TCHAR text[80];
		_tcserror_s(text, 80, _doserrno);
		CString message;
		message.Format(IDS_RESTART_FAILED, text);
		showMessageBox(message, _T("Error"), MB_OK | MB_ICONSTOP);	
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::checkForMesaGL()
{
	TCHAR buf[MAX_PATH];
	if (!GetModuleFileName(NULL, buf, MAX_PATH))
	{
		displayLastErrorMessage(IDS_MODULE_FAILED);
		return;
	}
	tstring	str(buf);
	size_t	pos			= str.find_last_of(_T("/\\"));
	tstring	path			= str.substr(0, pos);
	tstring	pathOpenGL	= path;
	tstring	pathEnv		= path;
	pathOpenGL += _T("\\opengl32.dll");	

	DWORD res = ::GetFileAttributes(pathOpenGL.c_str());
	if (res != INVALID_FILE_ATTRIBUTES)
	{
		typedef int (*pFuncPutenv) (const char*);
		pFuncPutenv putenv	= NULL;
		HINSTANCE	hModule	= GetModuleHandle(L"msvcrt.dll");
		if (hModule)
		{
			putenv = (pFuncPutenv)GetProcAddress(hModule, "_putenv");	
			if (putenv)
			{
				char buf[MAX_PATH];
				sprintf_s(buf, MAX_PATH, "MESA_GL_VERSION_OVERRIDE=%d.%dCOMPAT",
									MAJOR_VERSION_GL, MINOR_VERSION_GL);
				putenv(buf);
			}			
		}
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::motionFunc(int x, int y)
{   
	if (m_leftMButtonDown)
   {      
		glutSetCursor(GLUT_CURSOR_CYCLE);
		computeCursorPos(m_prevMousePos.x, m_prevMousePos.y);		
		m_prevMousePos = m_cursorPos2D;
		computeCursorPos(x, y);		

		vec2 delta			= m_cursorPos2D - m_prevMousePos;		
		vec3 camPos			= m_renderer->getCameraPos();
		camPos.x				-= delta.x;
		camPos.y				-= delta.y;
		m_prevMousePos.x	= (REAL)x;
		m_prevMousePos.y	= (REAL)y;
		updateCamera(camPos); 
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::resizeFunc(int width, int height)
{
	positionDialog(m_wndInfo.m_glutPos.x, m_wndInfo.m_glutPos.y);
	
	if (width == 0 || height == 0 || !m_isDataLoaded)
	{
		// intercept invalid client window size
		return;
	}

	m_wndInfo.m_glutClientSize.x  = width;
   m_wndInfo.m_glutClientSize.y	= height;
	m_wndInfo.m_resizeWnd			= true;
	TRACE(_T("+++ RESIZE +++\n"));
	setPerspective();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::setDialogSize(USHORT size)
{
	m_wndInfo.m_newDialogSizeVal	= size;
	m_useDefaultPosition				= (size != m_wndInfo.m_dialogSizeValue);	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

unsigned QuadtreeApp::getDialogSize()
{
	return m_wndInfo.m_dialogSizeValue; 
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

unsigned QuadtreeApp::getNewDialogSize()
{
	return m_wndInfo.m_newDialogSizeVal; 
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::updateCamera(vec3& cameraPos)
{
	m_renderer->updateCamera(cameraPos);
   m_renderer->drawScene();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::zoomToRegion()
{
	m_renderer->zoomToRegion();
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::resetView()
{
	m_renderer->resetView();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::mouseFunc(int button, int state, int x, int y)
{
	vec2 currentMousePos(x, y);
	vec3 currentCameraPos = m_renderer->getCameraPos();

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		m_leftMButtonDown		= false;
		m_leftMButtonClicked	= false;
		     
		if ((	m_startMousePos == currentMousePos) && computeCursorPos(x, y))
		{
			// find triangles based on search point coordinates
			m_renderer->setSearchPos(m_cursorPos2D);			
			findTriangles();
			m_renderer->drawScene();
		}
   }
   else if (button == 3 && state == GLUT_UP)
   {      
      computeCursorPos(x, y);      
		currentCameraPos += (m_cursorPos - currentCameraPos) * (REAL)0.1;
      updateCamera(currentCameraPos);
   }
   else if (button == 4 && state == GLUT_UP)
   {
      computeCursorPos(x, y);            
		currentCameraPos -= (m_cursorPos - currentCameraPos) * (REAL)0.1;
      updateCamera(currentCameraPos);
   }
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {            
      m_prevMousePos		=	currentMousePos;      
      m_leftMButtonDown	=	true;

		if (!m_leftMButtonClicked)
		{
			m_startMousePos		= currentMousePos;			
			m_leftMButtonClicked	= true;
		}
   }
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   {
      glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
      m_leftMButtonDown   = false;
   }

}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool QuadtreeApp::computeCursorPos(int x, int y)
{	
	// delegate object coordinate computation to renderer
	vec2	cursorPos2D;
	bool	isValidPos	= m_renderer->computeCursorPos(x, y, cursorPos2D);   
	m_cursorPos.x		= m_cursorPos2D.x = cursorPos2D.x;
   m_cursorPos.y		= m_cursorPos2D.y = cursorPos2D.y;
	return isValidPos;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::keyFunc(unsigned char k, int x, int y) 
{
   if (k == VK_ESCAPE) 
   {
      exitDemo();
   }
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::exitDemo()
{
   glutDestroyWindow(m_glutWnd);     
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::rebuildAndSearchTree()
{
	m_infoDialog->getValues(m_quadtreeData);
	buildTree();
	findTriangles();
	m_renderer->drawScene();		
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::buildTree()
{
	glutSetCursor(GLUT_CURSOR_WAIT);
	m_quadtree->cleanup();
	m_quadtree->createTree(m_quadtreeData);
	m_renderer->prepareTreeDisplay(m_quadtree->getBoundingBoxes(), 
											m_quadtree->getRegionsLeft());
	m_infoDialog->setDeviationPercent(m_quadtree->getDeviationPercent());
	m_infoDialog->setDeviationMedianPercent(m_quadtree->getDeviationMedianPercent());

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::findTriangles()
{	
	if (m_renderer->hasSearchPos())
	{			
		ResultData results;
		m_quadtree->findTriangles(m_renderer->getSearchPos(), results);		
		if (results.m_triangles)
		{
			m_renderer->updateSearchResults(results);
			m_infoDialog->updateSearchResults(m_renderer->getRenderData(),
														results.m_depth,
														m_quadtree->getRealMaxDepth());
		}
	}
	else
	{
		m_renderer->resetResults();
		m_infoDialog->resetResults();
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::specialKeyFunc(int key, int x, int y)
{
   vec3 cameraPos = m_renderer->getCameraPos();
	REAL delta		= cameraPos.z * 0.03;

	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
   {
		switch(key) 
		{
		case GLUT_KEY_UP:
			cameraPos.z -= delta;
			break;
		case GLUT_KEY_DOWN:
			cameraPos.z += delta;
			break;
		}
   }
   else
   {
		switch(key) 
		{
		case GLUT_KEY_LEFT:
			cameraPos.x -= delta;
			break;
		case GLUT_KEY_RIGHT:
			cameraPos.x += delta;
			break;
		case GLUT_KEY_UP:
			cameraPos.y += delta;
			break;
		case GLUT_KEY_DOWN:
			cameraPos.y -= delta;
			break;
		}
   }
   updateCamera(cameraPos);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::adaptToDisplayResolution()
{
	USHORT	val;
	int		resY = GetSystemMetrics(SM_CYSCREEN);

	if (resY <= 768)
	{
		val = WindowUtils::getDialogSize(WindowUtils::VERY_SMALL);
	}
	else if (resY <= 1200)
	{
		val = WindowUtils::getDialogSize(WindowUtils::NORMAL);
	}
	else if (resY <= 1440)
	{
		val = WindowUtils::getDialogSize(WindowUtils::EXTRA_LARGE);
	}
	else
	{
		val = WindowUtils::DLG_SIZE_MAX;
	}
	m_wndInfo.m_dialogSizeValue = val;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::setForegroundWindow()
{
	// Force glut window to the foreground.
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip;
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	for (int i = 0; i < 2; i++)
	{
		// Press the "ALT" key
		ip.ki.wVk = VK_MENU; 
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "ALT" key
		ip.ki.dwFlags = KEYEVENTF_KEYUP; 
		// send it twice
		SendInput(1, &ip, sizeof(INPUT));	
	}

	SetForegroundWindow(m_hwnd);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::processInitError(const char* txt, bool showOpenGL)
{
	if (txt)
	{
		showInitErrorMessage(txt);
	}
	else if (showOpenGL)
	{
		OpenglDialog dlg(	MAJOR_VERSION_GL,
								MINOR_VERSION_GL);
		if (dlg.DoModal(m_infoDlgHwnd) == IDCOPY_MESA)
		{
			// copy mesa opengl dll and restart application
			enableMesaModeAndRestart();
		}
	}
	m_infoDialog->DestroyWindow();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::showInitErrorMessage(const char* txt)
{
	astring message(txt);
	message += "\nThe program will exit after closing this message box.";
	showMessageBox(message.c_str(), "QuadtreeDemo Error", 
					MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::initGlutBorderSize()
{
	RECT rect, origRect;
	rect.bottom = 200;
	rect.left	= 100;
	rect.right	= 200;
	rect.top		= 100;
	origRect		= rect;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	m_wndInfo.m_glutBorderSize.y = origRect.top - rect.top;
	m_wndInfo.m_glutBorderSize.y += rect.bottom - origRect.bottom;

	m_wndInfo.m_glutBorderSize.x = origRect.left - rect.left;
	m_wndInfo.m_glutBorderSize.x += rect.right - origRect.right;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::initWindowAndDialog()
{
	if (doInitWindowAndDialog() == false)
	{
		return false;
	}

	m_wndInfo.m_newDialogSizeVal = m_wndInfo.m_dialogSizeValue;
	glutInitWindowSize(m_wndInfo.m_glutClientSize.x, m_wndInfo.m_glutClientSize.y); 
	glutInitWindowPosition(m_wndInfo.m_glutPos.x, m_wndInfo.m_glutPos.y);

	astring caption;
	createUniqueWindowCaption(caption);
	m_glutWnd			= glutCreateWindow(caption.c_str());
	m_hwnd				= FindWindowA(NULL, caption.c_str());

	if (m_hwnd == NULL)
	{
		return false;
	}

	ShowWindow(m_hwnd, SW_HIDE);
	setWindowIcon();
	m_isCompositionEnabled = WindowUtils::checkDWMStatusAndDisableNCRendering(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW);

	if (!m_isCompositionEnabled && m_useLegacyPaintMode)
	{
		m_repaintScene = true;
	}

	m_infoDlgHwnd = m_infoDialog->Create(m_hwnd);

	SetWindowText(m_hwnd, m_version.c_str());
	computeBorderDiffs();
	if (m_wndInfo.m_glutMax)
	{
		ShowWindow(m_hwnd, SW_MAXIMIZE);
	}
	// set the position of the info dialog explicitly
	positionDialog(m_wndInfo.m_glutPos.x, m_wndInfo.m_glutPos.y);
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::setWindowIcon()
{
	HICON hIcon;

	hIcon	= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_TITLE));
	SetClassLongPtr(m_hwnd, GCL_HICONSM, (LONG_PTR)hIcon);
	SetClassLongPtr(m_hwnd, GCL_HICON, (LONG_PTR)hIcon);
	SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(m_hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	DestroyIcon(hIcon);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::applyDialogSize()
{
	RECT  windowRect;   

	m_infoDialog->setValues(m_quadtreeData);
	m_infoDlgHwnd	= m_infoDialog->Create(NULL);
	
	::GetWindowRect(m_infoDlgHwnd, &windowRect);
	// get window rectangle dimensions of hidden info dialog
	m_wndInfo.m_dialogSize.x	= windowRect.right - windowRect.left;
	m_wndInfo.m_dialogSize.y	= windowRect.bottom - windowRect.top; 	

	m_infoDialog->DestroyWindow();

	if (m_useDefaultPosition)
	{		
		bool res = computeCenterPosition(1.34);

		if (res == false)
		{
			// fallback to smaller aspect ratio
			res = computeCenterPosition(1.0);
		}
		if (res == false)
		{
			return false;
		}
		m_wndPosStatus					= POS_SNAP_RIGHT;
		m_wndInfo.m_DlgSnapStatus	= POS_SNAP_RIGHT;
		m_useDefaultPosition			= false;
	}
	return true;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::doInitWindowAndDialog()
{
	bool res = false;

	initGlutBorderSize();

	if (m_wndInfo.m_dialogSizeValue > WindowUtils::DLG_SIZE_MAX)
	{
		// limit value to max dialog size
		m_wndInfo.m_dialogSizeValue = WindowUtils::DLG_SIZE_MAX;
	}
	if (m_wndInfo.m_dialogSizeValue < WindowUtils::DLG_SIZE_MIN)
	{
		// limit value to min dialog size
		m_wndInfo.m_dialogSizeValue = WindowUtils::DLG_SIZE_MIN;
	}

	do 
	{
		res = applyDialogSize();					
	}
	while (res == false && --m_wndInfo.m_dialogSizeValue >= WindowUtils::DLG_SIZE_MIN);	
	return res;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::createUniqueWindowCaption(astring& caption)
{	
	DWORD				id	= GetCurrentProcessId();
	astringstream	captionStream;
	
	captionStream << "[PID " << id << "] " << m_versionA.c_str();
	caption = captionStream.str().c_str();	
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::computeCenterPosition(float ratio)
{
	// get client window size
	int heightDlg						= m_wndInfo.m_dialogSize.y;	
	m_wndInfo.m_glutClientSize.y	= heightDlg;
	m_wndInfo.m_glutClientSize.y -= m_wndInfo.m_glutBorderSize.y;
	m_wndInfo.m_glutClientSize.x	= m_wndInfo.m_glutClientSize.y * ratio;

	// get entire with of program		
	int width	= m_wndInfo.m_dialogSize.x;
	width			+= m_wndInfo.m_glutBorderSize.x;
	width			+= m_wndInfo.m_glutClientSize.x;

	// compute center position of program	
	int workAreaX = m_sysParamInfo.m_rectWorkArea.right - m_sysParamInfo.m_rectWorkArea.left;
	int workAreaY = m_sysParamInfo.m_rectWorkArea.bottom - m_sysParamInfo.m_rectWorkArea.top;

	if (width > workAreaX || heightDlg > workAreaY)
	{
		return false;
	}

	m_wndInfo.m_glutPos.x	= (workAreaX - width) / 2;
	m_wndInfo.m_glutPos.y	= (workAreaY - heightDlg) / 2;
	m_wndInfo.m_glutPos.x	+= m_sysParamInfo.m_rectWorkArea.left;
	m_wndInfo.m_glutPos.y	+= m_sysParamInfo.m_rectWorkArea.top;
	m_wndInfo.m_glutMax		= 0;
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::load(const TCHAR* filename)
{   
	glutSetCursor(GLUT_CURSOR_WAIT);
	writeSettingsToFile();	

   if (!loadData(filename))
   {
		CString text;
		text.LoadString(IDS_LOAD_ERROR);
      text += filename;
		if (m_renderer->isInvalidFileFormat())
		{
			int lineNumber = m_renderer->getLineNumber();
			CString text2;
			text2.Format(IDS_INVALID_FILE_FORMAT, lineNumber);
			text += text2;
		}
      showMessageBox(text, _T("Error"), MB_OK | MB_ICONSTOP);	
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);   
      return;
   }

	tstring			title(m_version);
	title				+= _T(" - ");
	title				+= filename;
	m_filePath		=	filename;
	m_isDataLoaded	=	true;

	createUserdatPath();
	readSettingsFromFile();	
	buildTree();
	updateFileStatus();
   setPerspective();       	
   glutKeyboardFunc(::keyFunc);
   glutSpecialFunc(::specialKeyFunc);
   glutMouseFunc(::mouseFunc);
   glutMotionFunc(::motionFunc);
   SetForegroundWindow(m_hwnd);   
	SetWindowText(m_hwnd, title.c_str());	
	m_infoDialog->enableControls(true);
	m_infoDialog->updateFileInfo(m_renderer->getRenderData(), m_quadtreeData);

	if (m_isUserdatFileRead)
	{
		// Having opened a user data file we start a search for 
		// triangles and update the camera as well.
		findTriangles();	
		m_renderer->drawScene();
	}
	else
	{
		// The user data file could not be read
		// hence we reset the search and camera position.
		m_renderer->resetSearchPos();
		resetView();				
	}
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);   
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::createUserdatPath()
{	
	size_t pos			= m_filePath.find_last_of(_T("/\\"));
	tstring filename	= m_filePath.substr(pos+1);
	filename.replace(filename.length()-3, 3, _T("usr"));
	m_userdatPath		= USER_PATH;
	m_userdatPath		+= _T("\\");
	m_userdatPath		+= filename;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::showMessageBox(	HWND hwndParent, 
											UINT nText, 
											UINT nCaption, 
											UINT nType)
{
	CString text, caption;
	text.LoadString(nText);
	caption.LoadString(nCaption);
	return showMessageBox(hwndParent, text, caption, nType);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::showMessageBox(UINT nText, UINT nCaption, UINT nType)
{
	CString text, caption;
	text.LoadString(nText);
	caption.LoadString(nCaption);
	return showMessageBox(text, caption, nType | MB_SETFOREGROUND);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::showMessageBox(const char* text, 
										  const char* caption, UINT nType)
{
	USES_CONVERSION;
	tstring text2(A2T(text));
	tstring caption2(A2T(caption));

	return showMessageBox(m_infoDlgHwnd, text2.c_str(), caption2.c_str(),
								nType | MB_SETFOREGROUND);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::showMessageBox(LPCTSTR lpszText, UINT nCaption, UINT nType)
{
	CString caption;	
	caption.LoadString(nCaption);
	return showMessageBox(lpszText, caption, nType | MB_SETFOREGROUND);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::showMessageBox(	LPCTSTR	lpszText, 
											LPCTSTR	lpszCaption, 
											UINT		nType)
{
	return showMessageBox(m_infoDlgHwnd, lpszText, lpszCaption, nType | MB_SETFOREGROUND);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int QuadtreeApp::showMessageBox(	HWND		hwndParent, 
											LPCTSTR	lpszText, 
											LPCTSTR	lpszCaption, 
											UINT		nType)
{		
	MsgBoxDialog msgBox(lpszText, lpszCaption, nType | MB_SETFOREGROUND);
	return msgBox.DoModal(hwndParent);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::readSettingsFromFile()
{
	m_isUserdatFileRead			= false;
	const TCHAR* userdatPath	= m_userdatPath.c_str();

	if (!fileExists(userdatPath))
	{		
		m_quadtreeData->resetInputData();
		return;
	}
	m_quadtreeData->readSettingsFromFile(userdatPath);
	m_renderer->readSettingsFromFile(userdatPath);
	m_isUserdatFileRead = true;	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::initVersionStrings()
{
	USES_CONVERSION;
	tstring name(INTERNAL_NAME);
	tstring vers(A2T(VERSION_VERSION));
	m_version	= name + _T(" v") + vers;
	m_versionA	= T2A(m_version.c_str());
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const char* QuadtreeApp::getOpenGlVersion()
{
	return m_renderer->getVersion();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::errorFunc(const char* format, va_list arglist)
{		
	// we set up glut error message for later processing
	va_start(arglist, format);
	int		len = _vscprintf(format, arglist)+1;
	char*	buffer = new char[len]; 	
	vsprintf_s(buffer, len, format, arglist);
	m_glutErrorMessage = buffer;
	perror (buffer);
	va_end (arglist);
	delete[] buffer;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::closeFunc()
{
	m_infoDialog->getValues(m_quadtreeData);		
	writeSettingsToFile();
	writeSettingsToIni();
	m_renderer->cleanup();		
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::readSettingsFromIni(tstring& tempPath)
{		
	m_isIniFileRead = false;

	if (!fileExists(INI_PATH))
	{		
		return;
	}

	TCHAR buf[MAX_PATH];

	GetPrivateProfileString(QUAD_CHAPTER, _T("FilePath"), _T(""),
									buf, MAX_PATH, INI_PATH);
	

	tempPath					=  buf;
	m_autoLoad				=	GetPrivateProfileInt(QUAD_CHAPTER, _T("AutoLoad"), 
													m_autoLoad, INI_PATH);		
	m_useLegacyPaintMode	=  GetPrivateProfileInt(RENDER_CHAPTER, _T("UseLegacyPaintMode"), 
													m_useLegacyPaintMode, INI_PATH);
	bool useLines			=	GetPrivateProfileInt(RENDER_CHAPTER, _T("UseLinesForBaseMesh"), 
													m_renderer->useLinesForBaseMesh(), INI_PATH);	

	m_renderer->setLinesForBaseMesh(useLines);			

	tstringstream output;
	m_sysParamInfo.getString(output);	
	GetPrivateProfileString(SYSPARAM_CHAPTER, _T("DisplayProps"), _T(""), buf, MAX_PATH, INI_PATH);	

	if (_tcscmp(output.str().c_str(), buf) != 0)
	{
		// mismatch of system parameter info, so we invalidate window info and compute
		// new window and dialog positions later on.
		return;
	}

	m_wndInfo.m_glutMax					=	GetPrivateProfileInt(WND_CHAPTER, _T("GlutWndMax"), 
													m_wndInfo.m_glutMax, INI_PATH);	
	m_wndInfo.m_DlgSnapStatus			=	GetPrivateProfileInt(WND_CHAPTER, _T("DialogSnap"), 
													m_wndInfo.m_DlgSnapStatus, INI_PATH);	
	m_wndInfo.m_glutPos.x				=	GetPrivateProfileInt(WND_CHAPTER, _T("GlutPosX"), 
													m_wndInfo.m_glutPos.x, INI_PATH);	
	m_wndInfo.m_glutPos.y				=	GetPrivateProfileInt(WND_CHAPTER, _T("GlutPosY"), 
													m_wndInfo.m_glutPos.y, INI_PATH);	
	m_wndInfo.m_glutClientSize.x		=	GetPrivateProfileInt(WND_CHAPTER, _T("GlutWidth"), 
													m_wndInfo.m_glutClientSize.x, INI_PATH);	
	m_wndInfo.m_glutClientSize.y		=	GetPrivateProfileInt(WND_CHAPTER, _T("GlutHeight"), 
													m_wndInfo.m_glutClientSize.y, INI_PATH);
	m_wndInfo.m_infoPos.x				=	GetPrivateProfileInt(WND_CHAPTER, _T("InfoPosX"), 
													m_wndInfo.m_infoPos.x, INI_PATH);	
	m_wndInfo.m_infoPos.y				=	GetPrivateProfileInt(WND_CHAPTER, _T("InfoPosY"), 
													m_wndInfo.m_infoPos.y, INI_PATH);	
	m_wndInfo.m_dialogSizeValue		=	GetPrivateProfileInt(WND_CHAPTER, _T("DialogSize"), 
													m_wndInfo.m_dialogSizeValue, INI_PATH);	
	m_useDefaultPosition					=  GetPrivateProfileInt(WND_CHAPTER, _T("UseDefaultPosition"), 
													m_useDefaultPosition, INI_PATH);

	m_wndPosStatus					= POS_RESTORE;
	m_isIniFileRead				= true;	
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::computeBorderDiffs()
{
   RECT	rect, clientRect;

   ::GetWindowRect(m_hwnd, &rect);
   ::GetClientRect(m_hwnd, &clientRect);   

   m_wndInfo.m_glutFrameSize.x = (rect.right - rect.left) - 
											(clientRect.right - clientRect.left);
   m_wndInfo.m_glutFrameSize.y = (rect.bottom - rect.top) - 
											(clientRect.bottom - clientRect.top);	
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool QuadtreeApp::fileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&	
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool QuadtreeApp::directoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&	
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

tstring QuadtreeApp::getLastErrorAsString(const TCHAR* location)
{
	// get the error message, if any.
	DWORD errorID = ::GetLastError();
	if(errorID == 0)
		return _T(" No error message has been recorded.");

	LPTSTR messageBuffer	= NULL;
	size_t size				= FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
											| FORMAT_MESSAGE_FROM_SYSTEM 
											| FORMAT_MESSAGE_IGNORE_INSERTS,
											NULL, errorID, 
											MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
											(LPTSTR)&messageBuffer, 0, NULL);

	tstring txt(messageBuffer, size);
	LocalFree(messageBuffer);
	tstringstream message;

	if (location == NULL)
	{
		location = _T("n/a");
	}
	message	<< _T("\n\nError location: ") << location 
				<< _T("\nError number: ") << errorID 
				<< _T("\nError text: ") << txt; 
	return message.str();
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::displayLastErrorMessage(UINT nID, const TCHAR* path)
{
	CString message;
	message.LoadString(nID);
	displayLastErrorMessage(message.GetBuffer(), path);	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::displayLastErrorMessage(LPTSTR message, const TCHAR* path)
{
	tstring			errorMsg = getLastErrorAsString(path);
	tstringstream	text;
	text << message << errorMsg;
	showMessageBox(text.str().c_str(), _T("Error"), MB_OK | MB_ICONERROR);	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::updateFileStatus()
{
	if (!openIniFile())
	{
		return;
	}
	WritePrivateProfileString(QUAD_CHAPTER, _T("FilePath"), 
										m_filePath.c_str(), INI_PATH);
	WritePrivateProfileString(QUAD_CHAPTER, _T("AutoLoad"), _T("0"), INI_PATH);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::openIniFile()
{
	WORD wBOM = 0xFEFF;
	DWORD NumberOfBytesWritten;

	HANDLE hFile = ::CreateFile(INI_PATH, GENERIC_WRITE, 0, 
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		displayLastErrorMessage(IDS_ERROR_PROG_SETTINGS, INI_PATH);
		::CloseHandle(hFile);
		return false;
	}
	::WriteFile(hFile, &wBOM, sizeof(WORD), &NumberOfBytesWritten, NULL);
	::CloseHandle(hFile);
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::writeSettingsToIni()
{			
	if (!openIniFile())
	{
		return;
	}
	WINDOWPLACEMENT	wndPlacement;
	wndPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hwnd, &wndPlacement);

	POINT glutWndSize = m_wndInfo.m_glutClientSize;		
	POINT glutPos		= m_wndInfo.m_glutPos;
	POINT infoPos		= m_wndInfo.m_infoPos;

   bool maximized = false;
   if(wndPlacement.flags & WPF_RESTORETOMAXIMIZED)
   {
      maximized = true;

		// get window dimensions from the above data structure	
		glutPos.x		= wndPlacement.rcNormalPosition.left;
		glutPos.y		= wndPlacement.rcNormalPosition.top;
		glutWndSize.x	= wndPlacement.rcNormalPosition.right - wndPlacement.rcNormalPosition.left;
		glutWndSize.y	= wndPlacement.rcNormalPosition.bottom - wndPlacement.rcNormalPosition.top;	

		// subtract the frame dimensions to yield client area dimensions to be preserved in ini file
		glutWndSize.x  -= m_wndInfo.m_glutFrameSize.x;
		glutWndSize.y  -= m_wndInfo.m_glutFrameSize.y;
   }
	m_wndInfo.m_dialogSizeValue = m_wndInfo.m_newDialogSizeVal;

	WritePrivateProfileString(QUAD_CHAPTER, _T("AutoLoad"),
									_T("1"), INI_PATH);
	WritePrivateProfileString(QUAD_CHAPTER, _T("FilePath"), 
									m_filePath.c_str(), INI_PATH);

	tstringstream output;

	output << maximized;	
	WritePrivateProfileString(WND_CHAPTER, _T("GlutWndMax"), 
										output.str().c_str(), INI_PATH);	
	
	StreamUtils::resetStream(output);
	output << glutPos.x;
	WritePrivateProfileString(WND_CHAPTER, _T("GlutPosX"), 
										output.str().c_str(), INI_PATH);	
	
	StreamUtils::resetStream(output);
	output << glutPos.y;
	WritePrivateProfileString(WND_CHAPTER, _T("GlutPosY"), 
										output.str().c_str(), INI_PATH);	
	
	StreamUtils::resetStream(output);
	output << glutWndSize.x;
	WritePrivateProfileString(WND_CHAPTER, _T("GlutWidth"), 
										output.str().c_str(), INI_PATH);	
	
	StreamUtils::resetStream(output);
	output << glutWndSize.y;
	WritePrivateProfileString(WND_CHAPTER, _T("GlutHeight"), 
										output.str().c_str(), INI_PATH);

	StreamUtils::resetStream(output);
	output << m_wndPosStatus;
	WritePrivateProfileString(WND_CHAPTER, _T("DialogSnap"), 
										output.str().c_str(), INI_PATH);

	StreamUtils::resetStream(output);
	output << m_wndInfo.m_dialogSizeValue;
	WritePrivateProfileString(WND_CHAPTER, _T("DialogSize"), 
										output.str().c_str(), INI_PATH);

	StreamUtils::resetStream(output);
	output << infoPos.x;
	WritePrivateProfileString(WND_CHAPTER, _T("InfoPosX"),
										output.str().c_str(), INI_PATH);
	
	StreamUtils::resetStream(output);
	output << infoPos.y;
	WritePrivateProfileString(WND_CHAPTER, _T("InfoPosY"), 
										output.str().c_str(), INI_PATH);	

	StreamUtils::resetStream(output);
	output << m_useDefaultPosition;
	WritePrivateProfileString(WND_CHAPTER, _T("UseDefaultPosition"), 
										output.str().c_str(), INI_PATH);	

	StreamUtils::resetStream(output);
	output << m_useLegacyPaintMode;
	WritePrivateProfileString(RENDER_CHAPTER, _T("UseLegacyPaintMode"), 
										output.str().c_str(), INI_PATH);	

	StreamUtils::resetStream(output);
	output << m_renderer->useLinesForBaseMesh();
	WritePrivateProfileString(RENDER_CHAPTER, _T("UseLinesForBaseMesh"), 
										output.str().c_str(), INI_PATH);	

	StreamUtils::resetStream(output);
	m_sysParamInfo.readSysParamInfo();
	m_sysParamInfo.getString(output);

	WritePrivateProfileString(SYSPARAM_CHAPTER, _T("DisplayProps"), 
										output.str().c_str(), INI_PATH);	

}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::writeSettingsToFile()
{	
	if (m_userdatPath.empty())
	{
		return;
	}	
	if (!directoryExists(USER_PATH))
	{
		BOOL res = CreateDirectory(USER_PATH, NULL);
		if (res == FALSE)
		{			
			displayLastErrorMessage(IDS_ERROR_USERDAT, USER_PATH);
			return;			
		}
	}

	const TCHAR* filename	= m_userdatPath.c_str();
	HANDLE hFile				= ::CreateFile(filename, GENERIC_WRITE, 0, 
														NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{		
		displayLastErrorMessage(IDS_ERROR_QUADTREE_PROPS, m_userdatPath.c_str());
		::CloseHandle(hFile);
		return;
	}
	::CloseHandle(hFile);
	m_quadtreeData->writeSettingsToFile(filename);
	m_renderer->writeSettingsToFile(filename);
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::positionFunc(int x, int y)
{
	if (	x != WindowInfo::INVALID_POSITION
		&& y != WindowInfo::INVALID_POSITION)
	{
		// update current position
		m_wndInfo.m_glutPos.x = x;
		m_wndInfo.m_glutPos.y = y;	
	}

	positionDialog(x, y);	
	
	if (m_repaintScene)
	{	
		if (m_wndInfo.m_resizeWnd)
		{
			m_wndInfo.m_resizeWnd = false;
		}
		else
		{
			m_renderer->drawScene();
		}
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::repaintScene(HWND hwnd, bool checkForOverlaps)
{
	if (m_repaintScene && m_renderer->isValidOpenGLVersion())
	{	
		if (checkForOverlaps)
		{		
			static bool isOverlapRef = false;
			RECT			windowRect, rect;
			::GetWindowRect(m_hwnd, &windowRect);
			::GetWindowRect(hwnd, &rect);

			bool isOverlap = 	!(rect.right <= windowRect.left
								|| rect.left >= windowRect.right
								|| rect.top >= windowRect.bottom
								|| rect.bottom <= windowRect.top);
			
			if (isOverlap || isOverlapRef != isOverlap)
			{
				// info dialog overlaps with glut window so
				// we redraw our opengl scene to avoid "smearing" effects.			
				drawScene();
			}
			isOverlapRef = isOverlap;
		}
		else
		{
			drawScene();
		}
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::infoDlgPosChanged()
{
	RECT windowRect, infoDlgRect;
	::GetWindowRect(m_hwnd, &windowRect);
	::GetWindowRect(m_infoDlgHwnd, &infoDlgRect);

	if (	infoDlgRect.top != WindowInfo::INVALID_POSITION
		&& infoDlgRect.left != WindowInfo::INVALID_POSITION)
	{
		// update current position
		m_wndInfo.m_infoPos.x = infoDlgRect.left;
		m_wndInfo.m_infoPos.y = infoDlgRect.top;	
	}

	int diffRightX = abs(windowRect.right - infoDlgRect.left);
	int diffLeftX	= abs(windowRect.left - infoDlgRect.right);
	int diffY		= abs(windowRect.bottom - infoDlgRect.bottom);

	if (	diffRightX < WindowInfo::SNAP_SENSITIVITY 
		&& diffY < WindowInfo::SNAP_SENSITIVITY)
	{
		m_wndPosStatus = POS_SNAP_RIGHT;		
	}
	else if (	diffLeftX < WindowInfo::SNAP_SENSITIVITY 
				&& diffY < WindowInfo::SNAP_SENSITIVITY)
	{
		m_wndPosStatus = POS_SNAP_LEFT;		
	}
	else if (!::IsZoomed(m_hwnd))
	{
		m_wndPosStatus = POS_DEFAULT;
	}

	if (m_wndPosStatus & POS_SNAP)
	{			
		positionDialog(m_wndInfo.m_glutPos.x, m_wndInfo.m_glutPos.y);
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::positionDialog(int x, int y)
{	
	if (	x == WindowInfo::INVALID_POSITION 
		|| y == WindowInfo::INVALID_POSITION)
	{
		return;
	}
	
	if (m_wndPosStatus != POS_RESTORE)
	{
		if (::IsZoomed(m_hwnd))
		{
			// don't update position of the dialog if glut window is maximized
			return;
		}
	}
	
	int xCoord = m_wndInfo.m_infoPos.x;
	int yCoord = m_wndInfo.m_infoPos.y;

	if (m_wndPosStatus == POS_RESTORE && !::IsZoomed(m_hwnd))
	{
		// update status when being in restore status
		m_wndPosStatus = m_wndInfo.m_DlgSnapStatus;		
	}
	if (m_wndPosStatus & POS_SNAP)
	{
		// compute coordinates for info dialog when dialog is to snap 
		// to glut window
		RECT rectGlut;		
		::GetWindowRect(m_hwnd, &rectGlut);		
		yCoord = rectGlut.bottom - m_wndInfo.m_dialogSize.y;

		if (m_wndPosStatus == POS_SNAP_LEFT)
		{			
			xCoord	= x - m_wndInfo.m_dialogSize.x;		
		}
		else if (m_wndPosStatus == POS_SNAP_RIGHT)
		{
			xCoord	= rectGlut.right;		
		}
	}

	::SetWindowPos(m_infoDlgHwnd, HWND_TOP, xCoord, yCoord, 		 
		0, 0, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE);	

	// update info dialog position
	m_wndInfo.m_infoPos.x = xCoord;
	m_wndInfo.m_infoPos.y = yCoord;	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadtreeApp::loadData(const TCHAR* filename)
{   
	if (!m_renderer->loadData(filename))
	{
		return false;
	}
   m_quadtreeData->setBoundingBox(m_renderer->getBoundingBox());   
	m_quadtreeData->setMesh(&m_renderer->getMesh());   
   
   return true;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadtreeApp::setPerspective()
{   
   m_renderer->setPerspective( m_wndInfo.m_glutClientSize.x, 
										m_wndInfo.m_glutClientSize.y);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

BOOL QuadtreeApp::getOpenFileName(OPENFILENAME* ofn)
{
	BOOL res = FALSE;
	if (m_repaintScene)
	{
		::SetTimer(m_infoDlgHwnd, 1, 300, PollTimer);
		res = GetOpenFileName(ofn);
		::KillTimer(m_infoDlgHwnd, 1);
	}
	else
	{
		res = GetOpenFileName(ofn);
	}
	return res;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadtreeApp::drawScene()
{
	if (!m_isDataLoaded)
	{
		m_renderer->clearBuffer();
		return;
	}
	m_renderer->drawScene();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void CALLBACK PollTimer(HWND hwndParam, UINT uMsg, UINT_PTR idTimer, DWORD dwTime)
{
	static RECT rectRef = {0, 0, 0, 0};
	static HWND hWndRef = NULL;
	RECT rect;
	HWND hWnd = GetForegroundWindow();
	GetWindowRect(hWnd, &rect);

	bool res =		rectRef.bottom != rect.bottom
					||	rectRef.left	!= rect.left 
					||	rectRef.right	!= rect.right 
					||	rectRef.top		!= rect.top; 

	if (res || hWnd != hWndRef)
	{
		// redraw scene if position, size or hwnd of foreground window changes
		theApp.drawScene();
	}	
	rectRef	= rect;
	hWndRef	= hWnd;
}
