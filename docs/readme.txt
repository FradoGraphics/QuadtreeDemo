System requirements:
----------------------

Minimum Operating system:   Windows 7 SP 1
Required OpenGL version:	3.1

In case of an OpenGL error please update your graphics card drivers. 

Alternatively perform the following steps to use Mesa software rendering:

1.	Copy the file "opengl32.dll" from the MesaGL subfolder to the folder 
	where the file "QuadtreeDemo.exe" is located.
2.	Restart the demo.

Build instructions:
----------------------

The project should compile and link with Visual Studio 2005 and later.
Project and solution files are provided for VS 2022. 
Parts of the source code depend on Microsoft's WIN32/ATL libraries.


Change log:
-------------

v2.34 - 2018-12-02

+	Added generic message box dialog.
!	Fixed a bug in input value validation.

v2.33 - 2018-10-14

!	Bugfix and maintenance release.

v2.32 - 2018-09-22

+	Added button to switch to mesa software rendering in case of an error.

v2.31 - 2018-04-14

+	Added MesaGL software driver to setup package. 

v2.30 - 2018-04-01

+	Changed file format to wavefront obj.
+	Added option to run demo with Mesa software driver.
+	Added spin controls for edit fields.
!	Optimized program behavior when high dpi seetings are encountered.

v2.26 - 2018-02-03

+	Added Visual Studio 2017 solution and project files.
!	Maintenance release with some bug fixes and code optimizations.

v2.25 - 2017-01-22

!	Redesigned info dialog.
!	Refactored source code by moving functionality into new classes.

v2.2 - 2015-05-30

+	Added Snapping mechanism for info dialog.
+	Different dialog sizes can be set in Help dialog.
+	Regions not adhering to quadtree parameters are highlighted in orange.
+	Added quadtree statistics.
+	Bounding box of mesh is displayed.  
+	Extended mesh info with length and width values of mesh.

v2.1 - 2015-04-06

+	Quadtree properties and camera position are saved for each mesh separately
	in a directory named "userdat".

v2.0 - 2015-03-29

+	Ported the program to OpenGL v3.1 with Shader support. Deprecated OpenGL
	functions have been replaced by the ones from The OpenGL Mathematics 
	library (GLM).
!	The apply button now behaves sensitively to edit field input by users.

v1.9 - 2014-02-19

+	Added relevant tree search statistics.
+ 	Quadtree creation can be parametrized by percentage of total number
	of triangles per region.

v1.8 - 2014-01-26 

+	Current search- and camera positions are saved when closing the program.

v1.7 - 2014-01-02 

+	Added partial UTF 16 unicode support. For example file names
	containing russian characters can be read. Freeglut itself
	does not support unicode.   
+	Last opened file and current quadtree settings are saved for
	later reuse.
+	Glut window positions and dimensions are saved as well.
+	Info dialog now follows glut window movement. 
!	A modified freeglut.dll is delivered.

v1.6 - 2013-11-17 

+	Added shortcut Shift+A for reset view function.
!	Updated to freeglut v2.8.1 with improved mouse wheel operation.

v1.5 - 2013-11-10 

!	Fine-tuned keyboard and mouse navigation for smoother operation.

v1.4 - 2013-10-27 

+	Added reset button for view.
!	Fixed minor display error when triangles of a region are highlighted.	

v1.3 - 2013-01-03 

!	Optimized property value input via keyboard. 
!	Fixed special case in which the quadtree algorithm didn't work.
	(Happened as soon as a quadtree region was completely inside a triangle.)	

v1.2 - 2012-07-11 

!	Fixed ctrl+tab between windows, resizing of OpenGL window and made 
	error messages modal.

v1.1 - 2012-04-29 

+ 	Added intersection tests for remaining bounding box axes. 
	This increases the accuracy when adding triangles to the correct region.


