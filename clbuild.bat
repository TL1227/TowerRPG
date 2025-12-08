@ECHO OFF
cl src\*.cpp src\glad.c -I includes -std:c++17 -MD ^
-link -LIBPATH:lib ^
opengl32.lib ^
assimp-vc143-mtd.lib ^
fmod_vc.lib ^
freetype.lib ^
glfw3.lib
