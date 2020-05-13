##########################################
#
#  Makefile
#  (C) Eric Lecolinet 2019
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


#### RULES #################################

all: contact_basic contact_shared_ptr contact_special_cases

contact_basic: contact_basic.cpp contact_basic.hpp
	${CPP} ${CPPFLAGS} -o contact_basic contact_basic.cpp

contact_shared_ptr: contact_shared_ptr.cpp contact_shared_ptr.hpp
	${CPP} ${CPPFLAGS} -o contact_shared_ptr contact_shared_ptr.cpp

contact_special_cases: contact_special_cases.cpp contact_special_cases.hpp
	${CPP} ${CPPFLAGS} -o contact_special_cases contact_special_cases.cpp

clean:
	-rm -f *.o  contact_basic contact_shared_ptr contact_special_cases *.json depend *.tar.gz 1>/dev/null 2>&1

.SUFFIXES: .cpp

.cpp.o:
	${CPP} ${CPPFLAGS} -c -o $@ $<

.PHONY: all clean

