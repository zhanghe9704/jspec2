# Compiler and compiler flags
CXX = g++
FC = gfortran


INTERFACE_INCDIR = include
INCDIR =  ../include

# Source directories
SRC_DIR = ../src
INTERFACE_DIR = src

# Source files
#CXX_SRC = $(wildcard $(INTERFACE_DIR)/*.cc)
CXX_SRC += $(wildcard $(SRC_DIR)/*.cc)
CXX_SRC := $(filter-out ../src/main.cc, $(CXX_SRC))
CXX_SRC := $(filter-out ../src/ui.cc, $(CXX_SRC))
FORTRAN_SRC = $(wildcard $(INTERFACE_DIR)/*.f90)

# Object files
CXX_OBJ = $(CXX_SRC:.cc=.o)
FORTRAN_OBJ = $(FORTRAN_SRC:.f90=.o)

CFLAGS = -O3 -Wall -std=c++14 -fPIC -I$(INCDIR) -I$(INTERFACE_INCDIR)
FFLAGS = -Wall -O2
LIBS =  -lm -lgsl -lgslcblas

DEPS = $(wildcard $(INCDIR)/*.h)


$(info $$INCDIR is [${INCDIR}])
#$(info $$CXX_SRC is [${CXX_SRC}])
$(info $$CXX_OBJ is [${CXX_OBJ}])
#$(info $$FORTRAN_SRC is [${FORTRAN_SRC}])
$(info $$FORTRAN_OBJ is [${FORTRAN_OBJ}])
$(info $$CFLAGS is [${CFLAGS}])

# Fortran module
MODULE_NAME = JSPEC

all: $(MODULE_NAME).mod 

#$(MODULE_NAME).mod: $(FORTRAN_OBJ)
#	$(FC) $(FFLAGS) -c -o $(INTERFACE_DIR)/$(MODULE_NAME).o $(FORTRAN_SRC)
#	ar rcs $(INTERFACE_DIR)/$(MODULE_NAME).a $(INTERFACE_DIR)/$(MODULE_NAME).o
#	cp $(INTERFACE_DIR)/$(MODULE_NAME).a .

#%.o: %.cc $(DEPS)
#	$(CXX) -c -o $@ $< $(CFLAGS)

#%.o: %.f90 
#	$(FC) -c -o $@ $< $(FFLAGS)

#clean:
#	rm -f $(CXX_OBJ) $(INTERFACE_DIR)/*.o $(INTERFACE_DIR)/*.a $(MODULE_NAME).mod $(MODULE_NAME).a



all: $(MODULE_NAME).mod $(MODULE_NAME).a

$(MODULE_NAME).mod: $(FORTRAN_OBJ)
	$(FC) $(FFLAGS) -c -o $(INTERFACE_DIR)/interface.o $(INTERFACE_DIR)/interface.f90

$(MODULE_NAME).a: $(INTERFACE_DIR)/fortran_interface.o $(MODULE_NAME).mod
	ar rcs $(MODULE_NAME).a $(INTERFACE_DIR)/fortran_interface.o $(MODULE_NAME).o

$(INTERFACE_DIR)/fortran_interface.o: $(INTERFACE_DIR)/fortran_interface.cc fortran_interface.h $(CXX_SRC)
	$(CXX) $(CFLAGS) -c -o $(INTERFACE_DIR)/fortran_interface.o /$(INTERFACE_DIR)fortran_interface.cc $(CXX_SRC)

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

%.o: %.f90 
	$(FC) -c -o $@ $< $(FFLAGS)

clean:
	rm -f $(CXX_OBJ) $(INTERFACE_DIR)/*.o $(INTERFACE_DIR)/*.a $(MODULE_NAME).mod $(MODULE_NAME).a

