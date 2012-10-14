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
SOURCES  := src test
LIBRARY  := lib
INCLUDES := 

# The name of your project
# No SPACES AFTER THE NAME.
PROGNAME = proj-euler

################################################################################
# Do vars outside of BUILD directory
################################################################################

ifneq ($(BUILD),$(notdir $(CURDIR)))

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

BIN         := $(SRC:.c=)

VPATH       :=	$(foreach dir,$(SOURCES),../$(dir)) \
					$(foreach dir,$(LIBRARY),../$(dir))

# Some headers are in source and library directories
INCLUDES    += $(SOURCES) $(LIBRARY)

################################################################################
# These are various settings for the GCC compiler
################################################################################

CFLAGS   = -std=c99
# CFLAGS   += -Wall -Werror -pedantic 
CFLAGS   += -MMD -MP
CFLAGS   += $(addprefix -I $(CURDIR)/,$(INCLUDES))
CDEBUG   = -g 
CRELEASE = #-O2 
CC       = gcc 

################################################################################
# Export vars and change to BUILD directory
################################################################################

export

_cd:
	@echo "[MAKE] Entering $(BUILD)/"
	@cd $(BUILD) && $(MAKE) -f ../Makefile --no-print-directory 

.PHONY : _cd clean

else

################################################################################
# These are the targets for the build system
################################################################################

all : CFLAGS += $(CDEBUG) $(CRELEASE)
all : $(OFILES) lib.a $(BIN)
	@echo "[FINISH] Generated $(PROGNAME)"

.PHONY : all clean lib.a

$(BIN) : % : %.o lib.a
	@echo "[BINARY] Linking $@ "
	@$(CC) $< lib.a -o $@

lib.a : lib.a($(LIB_O))

.SECONDEXPANSION:
lib.a($(LIB_O)) : $$%
	@echo "[LIBRARY] Archiving $%"
	@ar cr lib.a $%

%.o : %.c 
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@
	
-include $(CFILES:%.c=%.d)

################################################################################
endif

clean :
	@echo "[CLEAN] Removing all compiled files"
	@cd $(BUILD); \
	 rm -f *.o *.a *.d
