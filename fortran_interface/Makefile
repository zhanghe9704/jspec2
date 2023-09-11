# Variables
CXX = g++
FC = gfortran
CXXFLAGS = -I../include -Wall -fPIC -std=c++11
FCFLAGS = -I. -J. -Wall -std=f2003
LDFLAGS = -L.

# Objects and Libraries
CPP_SOURCES = $(wildcard ../src/*.cc)
CPP_SOURCES += $(wildcard ./*.cc)
CPP_SOURCES := $(filter-out ../src/main.cc, $(CPP_SOURCES))
CPP_SOURCES := $(filter-out ../src/ui.cc, $(CPP_SOURCES))
CPP_OBJECTS = $(CPP_SOURCES:.cc=.o)
LIBRARY = libjspec.a
FORTRAN_OBJECTS = jspec.o
FORTRAN_MODULES = jspec.mod

all: $(LIBRARY) $(FORTRAN_OBJECTS) $(FORTRAN_MODULES)
	@echo "Fortran module and object files built."

$(FORTRAN_MODULES): $(FORTRAN_OBJECTS)

$(FORTRAN_OBJECTS): interface.f90 $(LIBRARY)
	$(FC) -o $@ -c $< $(LDFLAGS) $(FCFLAGS) -ljspec

$(LIBRARY): $(CPP_OBJECTS) $(FORTRAN_OBJECTS)
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(CPP_OBJECTS) $(FORTRAN_OBJECTS) $(LIBRARY) $(FORTRAN_MODULES)
	