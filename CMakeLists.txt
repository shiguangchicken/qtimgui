# minimum require
cmake_minimum_required (VERSION 3.8.1)

# global solution
project(qtimgui_sln)

set(OpenGL_GL_PREFERENCE GLVND)

# goto subs
add_subdirectory(examples)
add_subdirectory(modules)
add_subdirectory(src)
