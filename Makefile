
CCOPTS=-O3
#CCOPTS=-O0 -g

TARGETS=bingo chutes risk-battle sorry yahtzee

.PHONY: all
all: ${TARGETS}

bingo: bingo.c mt19937.c mt19937.h Makefile
	gcc ${CCOPTS} -o bingo mt19937.c bingo.c

chutes: chutes.c mt19937.c mt19937.h Makefile
	gcc ${CCOPTS} -o chutes chutes.c mt19937.c

risk-battle:risk-battle.c mt19937.c mt19937.h common.c common.h Makefile
	gcc ${CCOPTS} -o risk-battle mt19937.c common.c risk-battle.c

sorry: sorry.c mt19937.c mt19937.h Makefile
	gcc ${CCOPTS} -o sorry sorry.c mt19937.c

yahtzee: yahtzee.c mt19937.c mt19937.h common.c common.h Makefile
	gcc ${CCOPTS} -o yahtzee mt19937.c common.c yahtzee.c

.PHONY: clean
clean:
	rm -vfr ${TARGETS}
	rm -vfr *.o *.dSYM
