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
