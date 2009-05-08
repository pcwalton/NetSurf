# Sources
DIR_SOURCES := encoding.c ft.c glyphs.c intmetrics.c outlines.c utils.c

ifeq ($(FRONTEND),cli)
DIR_SOURCES := $(DIR_SOURCES) cli.c
else
DIR_SOURCES := $(DIR_SOURCES) toolbox.c
endif

include build/makefiles/Makefile.subdir
