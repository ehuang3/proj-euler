################################################################################
# Project Euler
# Name:  Eric Huang
# Notes: 
# 
################################################################################

# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# LIBRARY is a list of directories containing library code
# INCLUDES is a list of directories containing header files
BUILD    := build
SOURCES  := src
LIBRARY  := lib
INCLUDES := 

# The name of your project
# No SPACES AFTER THE NAME.
PROGNAME = proj-euler

################################################################################
# Automatically build a list of files to compile
################################################################################

# SRC is list of .c files in SOURCES
# LIBFILES is list of .c files in LIBRARY
SRC         := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
LIB         := $(foreach dir,$(LIBRARY),$(notdir $(wildcard $(dir)/*.c)))

SRC_O       := $(SRC:.c=.o)
LIB_O       := $(LIB:.c=.o)

OFILES      := $(SRC:.c=.o) $(LIB:.c=.o)

BIN         := $(SRCFILES:.c=)

VPATH       :=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBRARY),$(CURDIR)/$(dir)) \
					$(BUILD)

# Some headers are in source and library directories
INCLUDES    += $(SOURCES) $(LIBRARY)

################################################################################
# These are various settings for the GCC compiler
################################################################################

CFLAGS   = -Wall -Werror -std=c99 -pedantic 
CFLAGS   += -MMD -MP
CFLAGS   += $(addprefix -I $(CURDIR)/,$(INCLUDES))
CDEBUG   = -g 
CRELEASE = -O2 
CC       = gcc 

################################################################################
# These are the targets for the GBA build system
################################################################################

all : CFLAGS += $(CRELEASE) $(CDEBUG)
all : $(SRC_O) 
	@echo "[FINISH] Generated $(PROGNAME)"

.PHONY : all clean

$(SRC_O) : %.o : %.c 
	@echo "[LINK] Linking objects and library to create $<"
	@cd $(BUILD); \
	 $(CC) $< lib.a $@ -o $(@:.o=)

lib : $(LIB_O)
	@echo "[LIBRARY] Archiving library"
	@cd $(BUILD); \
	 ar rs lib.a $(LIBFILES:.c=.o) 

%.o : %.c 
	@echo "[COMPILE] Compiling $<"
	@cd $(BUILD); \
	 $(CC) $(CFLAGS) -c $< -o $@

clean :
	@echo "[CLEAN] Removing all compiled files"
	@cd $(BUILD); \
	 rm -f *
	
-include $(addprefix $(CURDIR)/$(BUILD)/, $(CFILES:%.c=%.d))
