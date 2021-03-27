# Project: mapslang
# Makefile created by KeeperFX/dkfans team

# Instructions:
# To create the Windows executable run:
# make all
# To create the linux executable run:
# make all BUILD_FOR_LINUX=1 (not fully implemented)
# To create a debug executable, run with the command:
# make all DEBUG=1 (un-tested)
# To clean up map run:
# make clean
# To perform a normal clean, and a clean of libslang run:
# make clean-inc-libs

# Windows version can be built with:
# - Windows   : MSYS2 (MinGW 32-bit) - make
# - Windows   : MSYS2 (MinGW 32-bit) - mingw32-make
# - Windows   : MinGW (MSYS) - make
# - WSL/Linux : i686-w64-mingw32-gcc - make

# -static-libgcc is used to remove DLL dependancies (e.g. libgcc_s_dw2-1.dll, libwinpthread-1.dll, and libgcc_s_sjlj-1.dll)

# Host environment
ifneq (,$(findstring Windows,$(OS)))
  CROSS_EXEEXT = .exe
else
  CROSS_EXEEXT =
  CROSS_COMPILE = i686-w64-mingw32-
endif
# Executable file extensions on target environment
BUILD_FOR_LINUX ?= 0
ifeq ($(BUILD_FOR_LINUX), 1)
  EXEEXT =
  OTHER_EXEEXT = .exe
  LIBEXT = .so
  OTHER_LIBEXT = .dll
  DLL_FLAG =
  CROSS_COMPILE =
else
  EXEEXT = .exe
  OTHER_EXEEXT =
  LIBEXT = .dll
  OTHER_LIBEXT = .so
endif
# Allow debug executables to be created
DEBUG ?= 0

CURSES_URL         = https://github.com/wmcbrine/PDCurses/archive/master.zip
CURSES_ZIP         = ../lib/curses.zip
CURSES_DIR         = ../lib/curses
CURSES_H           = $(CURSES_DIR)/curses.h

ifeq ($(BUILD_FOR_LINUX), 1)
  CURSES_LIB         = $(CURSES_DIR)/sdl2/pdcurses.a
  PLATFORM_LFLAGS    = -lm -L../libadikted/bin -ladikted $(CURSES_DIR)/sdl2/pdcurses.a -lSDL2
  PLATFORM_FLAGS     = -Dunix=1
  RES                = 
else
  PLATFORM_LFLAGS    = -static-libgcc 
  PLATFORM_FLAGS     =
  RES                = obj/map_private.res
endif

ifeq ($(DEBUG), 1)
DEBUG_CFLAGS = -g
DEBUG_LFLAGS = -g
else
DEBUG_CFLAGS = -O2
DEBUG_LFLAGS =
endif

# Names of utility commands
SLANG_URL          = https://www.jedsoft.org/releases/slang/slang-2.3.2.tar.bz2
SLANG_NAME         = slang-2.3.2
SLANG_ARCHIVE      = ../lib/slang.tar.bz2
SLANG_BIN_FOLDER   = /lib/$(SLANG_NAME)/src/$(SLANG_OBJ_FOLDER)
WARNFLAGS          = -Wall -Wextra -Werror-implicit-function-declaration -Wno-conversion -Wno-traditional-conversion -Wno-sign-compare
# is it actually needed?
#PLATFORM_FLAGS     = -march=i486

CSTANDARD          = -std=gnu99
CC                 = $(CROSS_COMPILE)gcc$(CROSS_EXEEXT) $(CSTANDARD) $(WARNFLAGS)
CFLAGS             = -c $(DEBUG_CFLAGS) -I. -I$(CURSES_DIR) $(DLL_FLAG) $(PLATFORM_FLAGS) -DBUILD_SINGLE=1
LINK               = $(CROSS_COMPILE)gcc$(CROSS_EXEEXT)
WINDRES            = $(CROSS_COMPILE)windres$(CROSS_EXEEXT)
RC                 = map_private.rc
LFLAGS             = $(PLATFORM_LFLAGS)
LIBS               =

SRC                = \
input_kb.c \
main.c \
output_scr.c \
scr_actn.c \
scr_clm.c \
scr_cube.c \
scr_help.c \
scr_list.c \
scr_rwrk.c \
scr_slab.c \
scr_thing.c \
scr_tileset.c \
scr_txted.c \
scr_txtgen.c \
textmenu.c \
var_utils.c 

OBJ                = $(addprefix obj/,$(SRC:.c=.o))
LIB_OBJ            = $(addprefix obj/lib/,$(LIB_SRC:.c=.o))
BIN                = bin/map$(EXEEXT)
RM                 = rm -f
MKDIR              = mkdir -p
CP                 = cp -f
STRIP              = strip --strip-all

.PHONY: pre-build post-build all clean standard

all: make-directories pre-build $(SRC) $(BIN) post-build

standard: $(BIN)

make-directories:
	$(MKDIR) ../lib bin obj obj/lib

ifeq ($(BUILD_FOR_LINUX), 1)

create-package: fill-pkg-directory
	$(CP) ../libadikted/bin/libadikted.so pkg/

else

create-package: fill-pkg-directory

endif

strip-executables:
	$(STRIP) pkg/map$(EXEEXT)

fill-pkg-directory:
	$(MKDIR) pkg/docs
	$(CP) $(BIN) pkg/
	$(CP) map.hlp pkg/
	$(CP) map.ini pkg/
	$(CP) ../docs/* pkg/docs/

post-build: create-package

pre-build:

clean-inc-libs: clean
	cd ../libadikted && make clean
	if [ -e ../lib/$(SLANG_NAME) ]; then cd ../lib/$(SLANG_NAME) && make clean; fi

clean:
	$(RM) $(OBJ) $(LIB_OBJ) $(BIN) $(BIN:$(EXEEXT)=$(OTHER_EXEEXT)) $(RES)
	$(RM)r pkg/*

$(BIN): $(OBJ) $(LIB_OBJ) $(RES) ../libadikted/libadikted$(LIBEXT)
	$(LINK) -o $@ $(DEBUG_LFLAGS) $(OBJ) $(LIB_OBJ) $(RES) $(LIBS) $(LFLAGS) 

obj/input_kb.o: input_kb.c $(CURSES_H)
	@echo 'ZZ $@'
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

../libadikted/libadikted$(LIBEXT):
	cd ../libadikted && make BUILD_FOR_LINUX=$(BUILD_FOR_LINUX) DEBUG=$(DEBUG)

$(RES): $(RC)
	$(WINDRES) -i $< --input-format=rc -o $@ -O coff
  
$(CURSES_ZIP):
	curl -f -L -o $@ $(CURSES_URL)
  
$(CURSES_LIB) $(CURSES_H): $(CURSES_ZIP)
	echo $(CURSES_H) && file $(CURSES_H) $(CURSES_ZIP)
	cd ../lib && unzip -o $< && mkdir -p curses && mv -n PDCurses-master/* $(CURSES_DIR)
	cd $(CURSES_DIR)/sdl2 && make

$(SLANG_ARCHIVE):
	curl -f -L -o $@ $(SLANG_URL)

$(LIB_SLANG): $(SLANG_ARCHIVE)
	-tar -C ../lib/ -xf $<
	cd ../lib/$(SLANG_NAME) && $(SLANG_BUILDCOMMAND)
	-$(RM) ..$(SLANG_BIN_FOLDER)/libslang.a

$(LIB_ADIKTED):
	cd ../libadikted && make

