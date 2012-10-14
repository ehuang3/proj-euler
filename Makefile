################################################################################
# Project Euler
# Name:  Eric Huang
# Notes: c++ version of makefile 
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
EXT	    	:= cpp
# SRC is list of .c files in SOURCES
# LIBFILES is list of .c files in LIBRARY
SRC         := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.$(EXT))))
LIB         := $(foreach dir,$(LIBRARY),$(notdir $(wildcard $(dir)/*.$(EXT))))

SRC_O       := $(SRC:.$(EXT)=.o)

LIB_O       := $(LIB:.$(EXT)=.o)

OFILES      := $(SRC:.$(EXT)=.o) $(LIB:.$(EXT)=.o)

BIN         := $(SRC:.$(EXT)=)

VPATH       := $(foreach dir,$(SOURCES),../$(dir)) \
	       $(foreach dir,$(LIBRARY),../$(dir))

# Some headers are in source and library directories
INCLUDES    += $(SOURCES) $(LIBRARY)

################################################################################
# These are various settings for the GCC compiler
#################################################################################
CFLAGS   =  -std=c++0x
CWARNING =  -Wall -Weffc++ -pedantic  \
				-pedantic-errors -Wextra  -Wall -Wcast-align \
				-Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion \
				-Wdisabled-optimization \
				-Wfloat-equal  -Wformat  -Wformat=2 \
				-Wformat-nonliteral -Wformat-security  \
				-Wformat-y2k \
				-Wimport  -Winit-self  -Winline \
				-Winvalid-pch \
				-Wunsafe-loop-optimizations  -Wlong-long -Wmissing-braces \
				-Wmissing-field-initializers -Wmissing-format-attribute   \
				-Wmissing-noreturn \
				-Wpacked  -Wpadded -Wparentheses  -Wpointer-arith \
				-Wredundant-decls -Wreturn-type \
				-Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector \
				-Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
				-Wswitch-enum -Wtrigraphs  -Wuninitialized \
				-Wunknown-pragmas  -Wunreachable-code -Wunused \
				-Wunused-function  -Wunused-label  -Wunused-parameter \
				-Wunused-value  -Wunused-variable  -Wvariadic-macros \
				-Wvolatile-register-var  -Wwrite-strings
				#-Wimplicit
				#-Wmissing-include-dirs
				#-Waggregate-return
				#-Werror
#CFLAGS  +=  $(CWARNING)
CFLAGS  +=  -MMD -MP
CFLAGS  +=  $(addprefix -I $(CURDIR)/,$(INCLUDES))
CDEBUG   =  -g
CLIBS    =  -lcppunit
CRELEASE =  #-O #-O2 
CC       =  g++ 

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
	@$(CC) $< lib.a -o $@ $(CLIBS)

lib.a : lib.a($(LIB_O))

.SECONDEXPANSION:
lib.a($(LIB_O)) : $$%
	@echo "[LIBRARY] Archiving $%"
	@ar cr lib.a $%

%.o : %.$(EXT) 
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(CFILES:%.$(EXT)=%.d)

################################################################################
endif

clean :
	@echo "[CLEAN] Removing all compiled files"
	@cd $(BUILD); \
		 rm -f *.o *.a *.d
