cc = g++
cflags = -lopengl32 -lglfw3 -lgdi32 -static -static-libgcc -static-libstdc++
src_files = src/*.cpp src/stb/*.cpp src/glad/*.c
include_dirs = -Iinclude
lib_dirs = -Llib -Llib/GLFW
output_file = bin/main

default:
	xcopy shaders bin\shaders /E /H /C /I /Y
	xcopy res bin\res /E /H /C /I /Y
	$(cc) -o $(output_file) $(src_files) $(include_dirs) $(lib_dirs) $(cflags)
	$(output_file)