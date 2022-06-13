CC=gcc
CCFLAGS = -std=c99 -Wall -Werror
MODULES = pgmCheckingValue.o pgmErrors.o pgmRead.o pgmWrite.o
MAINS = pgmEcho.o pgmComp.o pgma2b.o pgmb2a.o pgmReduce.o pgmTile.o pgmAssemble.o
TARGETS = pgmEcho pgmComp pgma2b pgmb2a pgmReduce pgmTile pgmAssemble

.c.o:
	$(CC) $(CCFLAGS) -c -o $@ $<

all: ${TARGETS} ${MAINS}

pgmEcho:	pgmEcho.o ${MODULES}
	${CC} ${CCFLAGS} -o pgmEcho pgmEcho.o ${MODULES}

pgmComp: pgmComp.o ${MODULES}
	${CC} ${CCFLAGS} -o pgmComp pgmComp.c ${MODULES}

pgma2b: pgma2b.o ${MODULES}
	${CC} ${CCFLAGS} -o pgma2b pgma2b.c ${MODULES}

pgmb2a: pgmb2a.o ${MODULES}
	${CC} ${CCFLAGS} -o pgmb2a pgmb2a.c ${MODULES}

pgmReduce: pgmReduce.o ${MODULES}
	${CC} ${CCFLAGS} -o pgmReduce pgmReduce.c ${MODULES}

pgmTile: pgmTile.o ${MODULES}
	${CC} ${CCFLAGS} -o pgmTile pgmTile.c ${MODULES}

pgmAssemble: pgmAssemble.o ${MODULES}
		${CC} ${CCFLAGS} -o pgmAssemble pgmAssemble.c ${MODULES}

clean:
	rm ${TARGETS} *.o
