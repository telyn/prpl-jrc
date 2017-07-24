LIBNAME = libjrc.so
ARCH = ""
ifeq ($(ARCH),i686)
	ARCHFLAGS = -m32
else ifeq ($(ARCH),x86_64)
	ARCHFLAGS = -m64
else
	ARCHFLAGS = 
endif

all: $(LIBNAME)

C_SRCS = plugin.c

C_OBJS = $(C_SRCS:.c=.o)
PKG_CONFIG = pkg-config
STRIP = strip
CC = gcc
LD = CC
CFLAGS_PURPLE = $(shell $(PKG_CONFIG) --cflags purple)
CFLAGS ?= \
	  $(ARCHFLAGS) \
	  -O2 \
	  -Wall \
	  -Wno-unused-function
CFLAGS += \
	  -fPIC \
	  -DPIC \
	  -DPURPLE_PLUGINS \
	  $(CFLAGS_PURPLE)

LIBS_PURPLE = $(shell $(PKG_CONFIG) --libs purple)
LDFLSGS ?= $(ARCHFLAGS)
LDFLAGS += -shared -pipe

PLUGIN_DIR_PURPLE:=$(shell $(PKG_CONFIG) --variable=plugindir purple)
DATA_ROOT_DIR_PURPLE:=$(shell $(PKG_CONFIG) --variable=datarootdir purple)

$(LIBNAME): $(C_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS_PURPLE)

%.o: %.c
	$(CC) -c $(CFLAGS) -std=c99 -o $@ $<

.PHONY: strip
strip: $(LIBNAME)
	$(STRIP) --strip-unneeded $(LIBNAME)

.PHONY: install
install: $(LIBNAME)
	install $(LIBNAME) $(DESTDIR)$(PLUGIN_DIR_PURPLE)/$(LIBNAME)

.PHONY: clean
clean:
	-rm -f *.o
	-rm -f $(LIBNAME)
