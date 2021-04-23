# OpenGLRenderer
OpenGL Renderer with realtime dynamic lighting

This is an incomplete proof of concept of an OpenGL shader software renderer.

1. Some code is based on Tiny Renderer by Dmitry V. Sokolov some remnants of the code like the OBJ model loader are still within, looking at moving away and using my own model loader and 3D models. https://github.com/ssloy/tinyrenderer
2. Collada DAE Parser is ported from Java to C++ and is based on public domain work here: https://github.com/TheThinMatrix/OpenGL-Animation


To use:

1. Run UnpackProjectFiles.bat. What this does is unpack vc_lib.zip in external\wxWidgets-3.1.5 to wxWidgets-3.1.5/vc_lib. This is wxWidgets 3.1.5 statically compiled and is only needed to build the level editor.

2. Open OpenGLRenderer.sln in Visual Studio 2019 and Compile in Debug or Release for x86 (32 bit only build at this stage).
