# makefile for MPI_exercises/Intro

# You can change these.

CC     = mpicc
CFLAGS = -O -Wall -std=c99 -g

# You shouldn't change these unless you really know what you're doing.

MONTEPI_C   = mpi_montepi.c
MONTEPI_O   = mpi_montepi.o
MONTEPI_EXE = mpi_montepi

LIBS          =

EXECUTABLE    = ${MONTEPI_EXE}
EXECUTABLES   = ${EXECUTABLE}

default:        ${EXECUTABLES}

clean:
	rm -f ${EXECUTABLES} *.[oadD] *.[mM][oO][dD] *.err *.out

${MONTEPI_EXE}:	${MONTEPI_O}
		${CC} ${CFLAGS} -o ${MONTEPI_EXE} ${MONTEPI_O} ${LIBS}

${MONTEPI_O}:		${MONTEPI_C}
		${CC} ${CFLAGS} -c ${MONTEPI_C}
