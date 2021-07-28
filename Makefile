##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.13.0-B3] date: [Thu May 20 10:51:48 MSK 2021]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = Easy_DataBase


######################################
# building variables
######################################
# use cpp?
USE_CPP = 1
# debug build?
DEBUG = 1
# optimization for C
C_OPT = -Og
# optimization for CPP
CPP_OPT = -Og -fno-exceptions -fno-rtti


#######################################
# paths
#######################################
# Build path
BUILD_DIR = Bin

######################################
# source
######################################

# Source file extension
C_FILE_EXTENSION = .c
CPP_FILE_EXTENSION = .cpp

# C sources
C_SOURCES =  \


# CPP sources
CPP_SOURCES = \
Core/Src/main.cpp  \
Easy_DataBase/Src/easy_db.cpp \
Easy_DataBase/Src/easy_db_datetime.cpp



#######################################
# binaries
#######################################
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC_CPP = $(GCC_PATH)/g++
CC = $(GCC_PATH)/gcc
AS = $(GCC_PATH)/gcc -x assembler-with-cpp
CP = $(GCC_PATH)/objcopy
SZ = $(GCC_PATH)/size
else
CC_CPP = g++
CC = gcc
AS = gcc -x assembler-with-cpp
CP = objcopy
SZ = size
endif

 
#######################################
# CFLAGS
#######################################

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \

# CPP defines
CPP_DEFS =  \

# AS includes
AS_INCLUDES = 

# C includes
C_CPP_INCLUDES =  \
-I Core/Inc  \
-I Easy_DataBase/Inc


# compile gcc flags
C_ASFLAGS = $(AS_DEFS) $(AS_INCLUDES) $(C_OPT) -Wall

C_FLAGS = $(C_DEFS) $(C_CPP_INCLUDES) $(C_OPT) -Wall

CPP_ASFLAGS = $(AS_DEFS) $(AS_INCLUDES) $(CPP_OPT) -Wall

CPP_FLAGS = $(CPP_DEFS) $(C_CPP_INCLUDES) $(CPP_OPT) -Wall

ifeq ($(DEBUG), 1)
C_FLAGS += -g -gdwarf-2
CPP_FLAGS += -g -gdwarf-2
endif


# Generate dependency information
C_FLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

CPP_FLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = 

# libraries
LIBS = 
LIBDIR = 
LDFLAGS = $(LIBDIR) $(LIBS)

# default action: build all
all: $(BUILD_DIR)/$(TARGET)


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:$(C_FILE_EXTENSION)=.o)))
vpath %$(C_FILE_EXTENSION) $(sort $(dir $(C_SOURCES)))

ifeq ($(USE_CPP), 1)
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:$(CPP_FILE_EXTENSION)=.o)))
vpath %$(CPP_FILE_EXTENSION) $(sort $(dir $(CPP_SOURCES)))
endif

$(BUILD_DIR)/%.o: %$(C_FILE_EXTENSION) Makefile | $(BUILD_DIR) 
	$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:$(C_FILE_EXTENSION)=.lst)) $< -o $@


$(BUILD_DIR)/%.o: %$(CPP_FILE_EXTENSION) Makefile | $(BUILD_DIR) 
	$(CC_CPP) -c $(CPP_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:$(CPP_FILE_EXTENSION)=.lst)) $< -o $@

ifeq ($(USE_CPP), 1)
$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CC_CPP) $(OBJECTS) $(LDFLAGS) -o $@
else
$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
endif


	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	RMDIR /S /Q $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***