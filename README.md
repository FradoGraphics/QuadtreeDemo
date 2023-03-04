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

![QuadtreeDemo](https://user-images.githubusercontent.com/119818594/216846640-1bf46061-6b75-4206-a827-d5053b0b7fdb.gif)


System requirements:
----------------------

- Minimum Operating system:   	Windows 7 SP1
- Required OpenGL version:	3.2

In case of an OpenGL error please update your graphics card drivers. 

Alternatively perform the following steps to use Mesa software rendering:

1.	Copy the file "opengl32.dll" from the MesaGL subfolder to the folder 
	where the file "QuadtreeDemo.exe" is located.
2.	Restart the demo.

Build environment and dependencies:
----------------------

The project builds with Visual Studio 2022. A CMakeLists.txt file is provided for project file generation using CMake.
Parts of the source code depend on Microsoft's WIN32/ATL libraries. The freeglut library is used for OpenGL context creation and navigation control.
Legacy project and solution files are provided for VS 2017 and VS 2005 and can be found in the `Archive` subfolder of this repository. 
