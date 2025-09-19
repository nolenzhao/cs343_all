OPT := # -O2

CXX = u++					# compiler
CXXFLAGS = -quiet -g -Wall -Wextra -MMD ${OPT}
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

.SUFFIXES : .cfa								# CFA default rules
.cfa.o :
	${CXX} ${CXXFLAGS} -c $<

OBJECTS1 = # object files forming 1st executable with prefix "q1"
EXEC1 = filter

OBJECTS2 = # object files forming 2nd executable with prefix "q2"
EXEC2 = hotpotato

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
EXECS = ${EXEC1} ${EXEC2}			# all executables
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean
.ONESHELL :

all : ${EXECS}					# build all executables

${EXEC1} : ${OBJECTS1}				# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}				# link step 2nd executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXECS}
