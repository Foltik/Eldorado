CC = clang++

LIBFLAGS = -lGL -lGLEW -lglfw -l:k5.a -lSOIL -lassimp -lfreetype
FLAGS = -Wall -std=c++11 -I/usr/include/freetype2
DBGFLAGS = -Wall -ggdb3 -std=c++11 -I/usr/include/freetype2

BUILDDIR = build

SRCFILES = $(wildcard Eldorado/*.cpp)
SRCNODIR = $(SRCFILES:Eldorado/%=%)
OBJREL = $(addprefix $(BUILDDIR)/linux/release/obj/, $(SRCNODIR:.cpp=.o))
OBJDBG = $(addprefix $(BUILDDIR)/linux/debug/obj/, $(SRCNODIR:.cpp=.o))

all: builddirs release debug resource

builddirs:
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/linux
	mkdir -p $(BUILDDIR)/linux/release $(BUILDDIR)/linux/debug
	mkdir -p $(BUILDDIR)/linux/release/obj $(BUILDDIR)/linux/debug/obj

release: $(OBJREL)
	$(CC) $(OBJREL) -o $(BUILDDIR)/linux/release/eldorado $(LIBFLAGS)

$(BUILDDIR)/linux/release/obj/%.o: Eldorado/%.cpp
	$(CC) -c $(FLAGS) $< -o $@

debug: $(OBJDBG)
	$(CC) $(OBJDBG) -o $(BUILDDIR)/linux/debug/eldorado $(LIBFLAGS)

$(BUILDDIR)/linux/debug/obj/%.o: Eldorado/%.cpp
	$(CC) -c $(DBGFLAGS) $< -o $@

resource:
	cp -r Eldorado/shaders $(BUILDDIR)/linux/release
	cp -r Eldorado/shaders $(BUILDDIR)/linux/debug
	cp -r Eldorado/textures $(BUILDDIR)/linux/release
	cp -r Eldorado/textures $(BUILDDIR)/linux/debug
	cp -r Eldorado/models $(BUILDDIR)/linux/release
	cp -r Eldorado/models $(BUILDDIR)/linux/debug

clean:
	rm -rf build
