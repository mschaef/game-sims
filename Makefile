
CCOPTS=-O3

TARGETS=bingo chutes risk-battle sorry

.PHONY: all
all: ${TARGETS}

bingo: bingo.c mt19937.c mt19937.h Makefile
	gcc ${CCOPTS} -o bingo mt19937.c bingo.c

chutes: chutes.c mt19937.c mt19937.h
	gcc ${CCOPTS} -o chutes chutes.c mt19937.c

risk-battle:risk-battle.c mt19937.c mt19937.h Makefile
	gcc ${CCOPTS} -o risk-battle mt19937.c risk-battle.c

sorry: sorry.c mt19937.c
	gcc ${CCOPTS} -o sorry sorry.c mt19937.c

.PHONY: clean
clean:
	rm -vf ${TARGETS}
	rm -vf *.o
