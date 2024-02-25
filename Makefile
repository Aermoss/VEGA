cc = g++
binaryDir := ./bin
objectDir := ./bin/obj
sourceDir := ./src
includeDirs := ./include ./include/imgui
libDirs := ./lib
executable := $(binaryDir)/main.exe
vendors := $(patsubst $(sourceDir)/vendor/%,%,$(wildcard $(sourceDir)/vendor/*))
sources := $(wildcard $(sourceDir)/*.cpp) $(foreach vendor,$(vendors),$(wildcard $(sourceDir)/vendor/$(vendor)/*.cpp))
objects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(sources)) $(foreach vendor,$(vendors),$(wildcard $(objectDir)/vendor/$(vendor)/*.o))
flags = -lopengl32 -lglfw3 -lgdi32 -static -static-libgcc -static-libstdc++
warnings := all

default: $(executable) run

$(executable): $(objects) | $(binaryDir)
	$(cc) $(foreach warn,$(warnings),-W$(warn)) $^ -o $@ $(foreach dir,$(includeDirs),-I$(dir)) $(foreach dir,$(libDirs),-L$(dir)) $(flags)

$(objectDir)/%.o: $(sourceDir)/%.cpp | $(objectDir)
	$(cc) $(foreach warn,$(warnings),-W$(warn)) $< -c -o $@ $(foreach dir,$(includeDirs),-I$(dir))

$(objectDir):
	mkdir bin && cd bin && mkdir obj && cd obj && mkdir vendor && cd vendor && $(foreach dir,$(vendors),mkdir $(dir) &&) cd ../../..

clean:
	rmdir /s /q bin

run: $(executable)
	$<