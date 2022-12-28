/**
 * Class name:    QuadTreeData

 * Description:   Keeps necessary data for creating the quad tree.

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "QuadTreeData.h"
#include "StreamUtils.h"


const float		QuadTreeData::LOWER_RATIO_BOUND		= 0.1;
const float		QuadTreeData::UPPER_RATIO_BOUND		= 100.0;
const float		QuadTreeData::RATIO_DEFAULT_VALUE	= 2;
const int		QuadTreeData::LOWER_DEPTH_BOUND		= 1;
const int		QuadTreeData::UPPER_DEPTH_BOUND		= 10;
const int		QuadTreeData::DEPTH_DEFAULT_VALUE	= 7;
const int		QuadTreeData::NUMTRI_DEFAULT_VALUE	= 100;
const unsigned	QuadTreeData::LOWER_NUMTRI_BOUND		= 10;
const unsigned	QuadTreeData::UPPER_NUMTRI_BOUND		= 10000;
const TCHAR*	QuadTreeData::QUAD_CHAPTER				= _T("Quadtree Settings");

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTreeData::validateData()
{
	if (	m_ratio > UPPER_RATIO_BOUND 
		|| m_ratio < LOWER_RATIO_BOUND)
	{
		m_ratio = RATIO_DEFAULT_VALUE;
	}
	if (	m_sizeBound > UPPER_NUMTRI_BOUND 
		|| m_sizeBound < LOWER_NUMTRI_BOUND)
	{
		m_sizeBound = NUMTRI_DEFAULT_VALUE;
	}
	if (	m_maxDepth > UPPER_DEPTH_BOUND 
		|| m_maxDepth < LOWER_DEPTH_BOUND)
	{
		m_maxDepth = DEPTH_DEFAULT_VALUE;
	}
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadTreeData::resetInputData()
{
	m_maxDepth			= DEPTH_DEFAULT_VALUE; 
	m_sizeBound			= NUMTRI_DEFAULT_VALUE;
	m_ratio				= RATIO_DEFAULT_VALUE;
	m_enableSizeBound = true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadTreeData::readSettingsFromFile(const TCHAR* userdatPath)
{
	TCHAR	buf2[3][MAX_BUFSIZE_2];

	m_maxDepth			= (GetPrivateProfileInt(QUAD_CHAPTER, _T("MaxDepth"),
								m_maxDepth, userdatPath));
	m_sizeBound			= (GetPrivateProfileInt(QUAD_CHAPTER, _T("TriBound"),
								m_sizeBound, userdatPath));
	m_enableSizeBound	= (GetPrivateProfileInt(QUAD_CHAPTER, _T("EnableTriBound"), 
								m_enableSizeBound, userdatPath));	
	
	GetPrivateProfileString(QUAD_CHAPTER, _T("PercentageRatio"), _T("2.0"),
									buf2[0], MAX_BUFSIZE_2, userdatPath);	
	_stscanf_s(buf2[0], _T(PREC_SCAN_2), &m_ratio);
	validateData();
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool QuadTreeData::writeSettingsToFile(const TCHAR* userdatPath)
{
	validateData();

	tstringstream output;
	StreamUtils::resetStream(output);
	output << m_maxDepth;
	WritePrivateProfileString(QUAD_CHAPTER, _T("MaxDepth"), 
								output.str().c_str(), userdatPath);

	StreamUtils::resetStream(output);
	output << m_sizeBound;
	WritePrivateProfileString(QUAD_CHAPTER, _T("TriBound"), 
								output.str().c_str(), userdatPath);

	StreamUtils::resetStream(output);
	output << m_enableSizeBound;
	WritePrivateProfileString(QUAD_CHAPTER, _T("EnableTriBound"), 
								output.str().c_str(), userdatPath);

	StreamUtils::resetStream(output);	
	output << m_ratio;
	WritePrivateProfileString(QUAD_CHAPTER, _T("PercentageRatio"), 
								output.str().c_str(), userdatPath);	
	return true;
}
