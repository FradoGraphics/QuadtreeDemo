// Copyright (C) 2018 Frank Dockhorn <Email: dexterity2000@gmail.com> 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// Description of the demonstration:

// This C++ demo uses a quadtree data structure based on axis-aligned 
// bounding boxes to speed up the search for triangles in a mesh. Depending 
// on a search point all triangles of a quadtree's region are retrieved and 
// relevant information about the search is shown.

// Quadtree creation can be parametrized by three parameters: 

// - Maximum tree depth.
// - Maximum number of triangles per region.
// - Percentage of total number of triangles per region.

// The OpenGL graphics library is used to render the results, regions as 
// well as the underlying triangle mesh. Navigation (e.g. zooming, moving) 
// is available by keyboard and mouse. Example meshes are provided in wavefront  
// obj format and can be opened from inside the program.


#include "stdafx.h"
#include "QuadTree.h"
#include <math.h>
#include "QuadtreeApp.h"


QuadtreeApp theApp;

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
   theApp.init(argc, argv);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void keyFunc(unsigned char k, int x, int y) 
{
   theApp.keyFunc(k, x, y);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void specialKeyFunc(int key, int x, int y)
{
   theApp.specialKeyFunc(key, x, y);   
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void mouseFunc(int button, int state, int x, int y)
{
   theApp.mouseFunc(button, state, x, y);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void resizeFunc(int width, int height)
{
   theApp.resizeFunc(width, height);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void motionFunc(int x, int y)
{
   theApp.motionFunc(x, y);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void errorFunc(const char* string, va_list ap)
{
	theApp.errorFunc(string, ap);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void drawFunc()
{
	theApp.drawScene();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void closeFunc()
{
	theApp.closeFunc();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void positionFunc(int x, int y)
{
	theApp.positionFunc(x, y);
}
