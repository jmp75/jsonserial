##########################################
#
#  Makefile
#  (C) Eric Lecolinet 2017
#  http://www.telecom-paristech.fr/~elc/
#
##########################################

#
# jsonserial
#
JSONSERIAL = ../..

#
# C++11 compiler, compiler options, documentation
#
CPP = c++
CPPFLAGS = -std=c++11 -I${JSONSERIAL} -Wall -O2
DOXYGEN = doxygen

#
# Source files
#
HPPFILES = tests.hpp
CPPFILES = tests.cpp
SRCFILES = ${HPPFILES} ${CPPFILES} 

#
# Object files and program
#
OBJFILES = ${CPPFILES:%.cpp=%.o}
PROG = tests


#### RULES #################################

all: ${PROG}

${PROG}: depend ${OBJFILES} 
	${CPP} ${CPPFLAGS} -o ${PROG} ${OBJFILES}  

clean:
	-rm -f *.o ${PROG} *.json depend *.tar.gz 1>/dev/null 2>&1

doc: ${SRCFILES}
	${DOXYGEN} 

depend:
	${CPP} ${CPPFLAGS} -MM ${CPPFILES} > depend

.SUFFIXES: .cpp

.cpp.o:
	${CPP} ${CPPFLAGS} -c -o $@ $<

.PHONY: all clean clean-all doc tar depend

# Include dependencies
-include depend
