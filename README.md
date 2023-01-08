# QuadtreeDemo
Demonstrates a quadtree for spatial indexing of triangles.

This C++ demo allows users to interactively explore a quadtree's behavior.

For this purpose the demo uses a quadtree data structure based on axis-aligned bounding boxes to speed up the search for triangles in a mesh. Depending on a search point all triangles of a quadtree's region are retrieved and relevant information about the search is shown.

Quadtree creation can be parametrized by three parameters:

- Maximum tree depth.
- Maximum number of triangles per region.
- Percentage of total number of triangles per region.

The OpenGL graphics library is used to render the results, regions as well as the underlying triangle mesh. Navigation (e.g. zooming, moving) is available by keyboard and mouse.

Example meshes are provided in wavefront *.obj format and can be opened from inside the program.

![quadtreedemo_screenshot](https://user-images.githubusercontent.com/119818594/210186422-5b50c950-29e8-49ec-9f0c-1e3b188c8b7e.png)


System requirements:
----------------------

- Minimum Operating system:   	Windows 7 SP1
- Required OpenGL version:	3.1

In case of an OpenGL error please update your graphics card drivers. 

Alternatively perform the following steps to use Mesa software rendering:

1.	Copy the file "opengl32.dll" from the MesaGL subfolder to the folder 
	where the file "QuadtreeDemo.exe" is located.
2.	Restart the demo.

Build instructions and dependencies:
----------------------

The project should compile and link with Visual Studio 2005 and later.
Project and solution files are provided for VS 2022, VS 2017 and VS 2005. 
Parts of the source code depend on Microsoft's WIN32/ATL libraries.
The freeglut library is used for OpenGL context creation and navigation control.
