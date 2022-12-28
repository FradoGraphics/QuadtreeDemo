/**
 * Class name:		FreeglutExtensions

 * Description:   Loads needed Freeglut extension functions 

 * Author:        Frank Dockhorn
 */


#ifndef FREEGLUT_EXTENSIONS
#define FREEGLUT_EXTENSIONS


class FreeglutExtensions
{
public:
	static	bool loadLibrary();
	static	void freeLibrary();
private:
	static HMODULE s_hFreeglutDll;
};

#endif
