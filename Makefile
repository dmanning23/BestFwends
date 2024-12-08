all:
	dasm *.dasm -f3 -v0 -oBestFwends.bin

run:
	stella BestFwends.bin