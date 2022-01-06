all: type1seqgen.cpp anyoption.cpp
	g++ type1seqgen.cpp anyoption.cpp -o seqgen

clean:
	/bin/rm seqgen
