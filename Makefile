all: clean centralCoord philosopher tokenRingAlgoirthm starter

centralCoord: soc.h philosopher.h centralCoord.c
	$(CC) -o startC philosopher.h soc.h centralCoord.c

philosopher: soc.h philosopher.h philosopher.c
	$(CC) -o philosopher philosopher.h soc.h philosopher.c

tokenNode: soc.h philosopher.h tokenRingAlgoirthm.c
	$(CC) -o tokenRingAlgoirthm philosopher.h soc.h tokenRingAlgoirthm.c

starter: soc.h philosopher.h tokenRingStarter.c
	$(CC) -o startTR philosopher.h soc.h tokenRingStarter.c

clean:
	rm -f startC
	rm -f startTR
	rm -f philosopher
	rm -f tokenRingAlgoirthm