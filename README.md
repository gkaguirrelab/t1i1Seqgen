# t1i1Seqgen
Creation of Type 1 Index 1 sequences

To compile the software, `anyoption.cpp` must be present [LINK](https://github.com/hackorama/AnyOption).

To compile on Mac OS X with an Apple Silicon chip, set the terminal app to open in Rosetta (using the Get Info panel), navigate to the directory with these files, and the anyoption.cpp file, and then issue the `make` command.

Once you have compiled the code, you can examine the help options with `./seqgen -h`.

A typical call to the routine might be:
`./seqgen -l -n 7 -t 10`
which will provide a total of 10 counter-balanced sequences of 7 elements. The `-l` option forms the sequence output as a single line, as opposed to a matrix.

Do not attempt to call the routine with n<=5, as no valid Type 1 Index 1 sequences exist, and the code does not handle this situation gracefully...

More information regarding these sequences may be found here: https://cfn.upenn.edu/aguirre/wiki/doku.php?id=public:t1i1_sequences
