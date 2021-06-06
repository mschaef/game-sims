
CCOPTS=-O3
#CCOPTS=-O0 -g

TARGETS=bingo chutes risk-battle sorry yahtzee

COMMON_SRC=mt19937.c common.c
COMMON_DEPS=mt19937.c mt19937.h common.c common.h Makefile

.PHONY: all
all: ${TARGETS}

bingo: bingo.c ${COMMON_DEPS}
	gcc ${CCOPTS} -o bingo bingo.c ${COMMON_SRC}

chutes: chutes.c ${COMMON_DEPS}
	gcc ${CCOPTS} -o chutes chutes.c  ${COMMON_SRC}

risk-battle: risk-battle.c ${COMMON_DEPS}
	gcc ${CCOPTS} -o risk-battle risk-battle.c ${COMMON_SRC}

sorry: sorry.c ${COMMON_DEPS}
	gcc ${CCOPTS} -o sorry sorry.c ${COMMON_SRC}

yahtzee: yahtzee.c ${COMMON_DEPS}
	gcc ${CCOPTS} -o yahtzee yahtzee.c ${COMMON_SRC}

.PHONY: clean
clean:
	rm -vfr ${TARGETS}
	rm -vfr *.o *.dSYM
