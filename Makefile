TARGET = river_raid
OBJS = main.o
VPATH = src

# --- Adicione as duas linhas abaixo ---
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = River Raid Clone

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS)
ASFLAGS = $(CFLAGS)

LIBS = -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpng -lz

PSPSDK := $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
