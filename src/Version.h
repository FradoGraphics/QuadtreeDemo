/**
* File name:      version.h

* Description:    Keeps program version. 

* Author:         Frank Dockhorn
*/

#define MajorVersion  2
#define MinorVersion  34
#define PatchLevel    1
#define SubPatchLevel 4

#define CompanyName		Frank Dockhorn
#define FileDescription QuadtreeDemo
#define ProductName		QuadtreeDemo
#define InternalName		Quadtree Demo
#define LegalCopyright	Copyright (C) 2023

#define _STR_OP_(s) #s
#define _LSTR_OP_(s) L#s
#define _VER_STRING_2(param) _STR_OP_(param)
#define _VER_STRING_3(param) _LSTR_OP_(param)
#define COMPANY_NAME			_VER_STRING_3(CompanyName)
#define FILE_DESCRIPTION	_VER_STRING_3(FileDescription)
#define PRODUCT_NAME			_VER_STRING_3(ProductName)
#define INTERNAL_NAME		_VER_STRING_3(InternalName)
#define LEGAL_COPYRIGHT		_VER_STRING_3(LegalCopyright)

#define _VER_STRING(maj, min, pl, spl) _STR_OP_(maj) "." _STR_OP_(min) "." _STR_OP_(pl) "." _STR_OP_(spl)
#define VERSION_VERSION _VER_STRING(MajorVersion, MinorVersion, PatchLevel, SubPatchLevel)

