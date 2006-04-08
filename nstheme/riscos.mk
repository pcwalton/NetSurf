CC = gcc

PLATFORM_CFLAGS_RISCOS = -INSLibs:include -IOSLib:
LDFLAGS_RISCOS = OSLib:o.oslib32

RUNIMAGE = !NSTheme/!RunImage
