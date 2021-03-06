# Builds static library libphysics.a

LIBNAME = libphysics.a
SRCDIR = ./
ROOTDIR = ../../
SRCFILES = $(wildcard *.cc)

# Coimpiler settings

CXX = g++
CXXFLAGS = -I $(SRCDIR) -I $(ROOTDIR) -ansi -pedantic-errors -Wall -Wextra
CXXFLAGS += -MP -MMD -std=c++14
DBGFLAGS = -ggdb3 -DDEBUG -O0 -pg -no-pie
RLSFLAGS = -O2

##############################################################################

# Build library (release version)

$(LIBNAME) : $(SRCFILES:%.cc=%.o)
	ar crs $(LIBNAME) $(SRCFILES:%.cc=%.o)
	ranlib $(LIBNAME)

%.o : %.cc %.h
	$(CXX) $(CXXFLAGS) $(RLSFLAGS) -c $< -o $@

# Build library (debug version)

debug	: $(SRCFILES:%.cc=%dbg.o)
	ar crs $(LIBNAME) $(SRCFILES:%.cc=%dbg.o)
	ranlib $(LIBNAME)

%dbg.o : %.cc %.h
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) -c $< -o $@

##############################################################################

# Include dependicies files early generated by gcc with -MP -MMD opts

-include $(SRCFILES:%.cc=%.d)

# Clean garbage

clean :
	rm -f *.o *.d $(LIBNAME)