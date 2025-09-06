# Makefile (Corrigido)
TARGET = river_raid
OBJS = src/main.o src/graphics.o src/sprite.o src/player.o src/enemy.o src/scenario.o src/game.o

# Adicionamos o -Isrc diretamente aqui e removemos a linha INCDIR
CFLAGS = -Isrc -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
# Adicionei as bibliotecas que faltavam para as novas funções (pspdebug, display, etc)
LIBS = -lpng -lz -lm -lpspgum -lpspgu -lpspctrl -lpspdebug -lpspdisplay -lpspge

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = River Raid

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak