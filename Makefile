##########################################
#
#  JsonSerial Makefile
#  (C) Eric Lecolinet 2019
#  http://www.telecom-paristech.fr/~elc/
#
##########################################


#
# C++11 compiler, compiler options, documentation
#
CPP = c++
CPPFLAGS = -std=c++11 -Wall -O2
DOXYGEN = doxygen

#
# Source files
#
HPPFILES = *.hpp
CPPFILES = 
SRCFILES = ${HPPFILES} ${CPPFILES} 

#
# Object files
#
OBJFILES = ${CPPFILES:%.cpp=%.o}


#### RULES #################################

#
# Default
#

all: 

clean:
	-rm -f *.o *.tar.gz depend core 1>/dev/null 2>&1
	-rm -f -r html 1>/dev/null 2>&1

tar:
	tar zcvf jsonserial.tar.gz ${SRCFILES}  COPYING* Makefile Doxyfile examples tests

doc: ${SRCFILES}
	${DOXYGEN} 

depend:
	${CPP} ${CPPFLAGS} -MM ${CPPFILES} > depend

.SUFFIXES: .cpp

.cpp.o:
	${CPP} ${CPPFLAGS} -c -o $@ $<

.PHONY: all clean clean-all doc tar depend

