# t1i1Seqgen
Creation of Type 1 Index 1 sequences

## Code quick start
To compile the software, `anyoption.cpp` must be present [LINK](https://github.com/hackorama/AnyOption).

To compile on Mac OS X with an Apple Silicon chip, set the terminal app to open in Rosetta (using the Get Info panel), navigate to the directory with these files, and the anyoption.cpp file, and then issue the `make` command.

Once you have compiled the code, you can examine the help options with `./seqgen -h`.

A typical call to the routine might be:
`./seqgen -l -n 7 -t 10`
which will provide a total of 10 counter-balanced sequences of 7 elements. The `-l` option forms the sequence output as a single line, as opposed to a matrix.

Do not attempt to call the routine with n<=5, as no valid Type 1 Index 1 sequences exist, and the code does not handle this situation gracefully...

## What is a T1I1 Sequence?

A Type-1, Index-1 (T1I1) sequence of n labels has a length of n<sup>2</sup>+1 elements. The name of the sequence was given by [Finney and Outhwaite in 1956](http://adsabs.harvard.edu/abs/1955Natur.176..748F) and refers to the first-order counterbalancing of the sequence, including the repetition of elements. [Aletta Nonyane and Chris Theobald](ttp://www.bioss.ac.uk/staff/cmt/designseq.html) have [described an algorithm](http://dx.doi.org/10.1348/000711006X114568) for the production of T1I1 sequences with 6 or more labels. 

T1I1 sequences have the property of presenting permuted blocks of the labels over the course of the sequence. An example T1I1 sequence for 6 labels is:

  1
  1  2  3  4  5  6
  6  1  4  3  5  2
  2  5  3  1  6  4
  4  2  6  5  1  3
  3  6  2  4  1  5
  5  4  6  3  2  1

## Useful Properties of T1I1 sequences with regard to BOLD fMRI

The n labels of the T1I1 sequence can be assigned to n stimuli (alternatively, n-1 stimuli and a "null" trial) and determine the order of presentation of stimuli in a continuous-carry over, BOLD fMRI experiment. T1I1 sequences offer several advantages in this context:

  * The sequence is first-order counterbalanced, allowing the "direct" effect of each stimulus to be measured independently of "carry-over" effects (see [Aguirre 2007](https://pubmed.ncbi.nlm.nih.gov/17376705/) )
  * The stimuli are presented in permuted blocks, rendering the neural effects relatively insensitive to order effects across the entire experiment. In other words, the T1I1 sequence has relatively little power at very low frequencies, thus avoiding the elevated (1/f) noise range of BOLD fMRI.
  * Unlike m-sequences, a T1I1 sequence can be found for any number of labels greater than five.
  * Many fundamentally different T1I1 sequences can be generated for a given number of labels, and evaluated according to balance and design criteria.

There are, however, some potentially undesirable properties of T1I1 sequences for this application:

  * Although the order of stimulus presentation within “blocks” is highly stochastic, there is some minimal regularity in a T1I1 sequence that might be detectable by the subject (each stimulus is guaranteed to appear twice within 2n − 2 trials, and there is a stimulus repetition every n trials)
  * Only first-order counterbalancing is available
  * No T1I1 sequences are available for n=3, 4, or 5

## How to Create a T1I1 sequence

T1I1 sequences can be generated exhaustively for a given n, or randomly sampled from the potentially vast space of possible sequences. Dongbo Hu, working with code povided by Nonyane and Theobald wrote the command-line program available here for the production of random T1I1 sequences:

<code>
seqgen
Usage: 
 -h --help            Prints this help
 -l --line            Print terms on one line instead of a square matrix
 -q --quiet           minimal output format (implies -l)
 -n --length 17       Number of terms 
 -t --total 999       Stop after this many found. Note that many may be duplicates with small values of -n.
                      Default is to just keep going.
</code>

##Evaluating the "quality" of a T1I1 sequence

T1I1 sequences may differ in the degree to which the ordering of the stimuli within blocks is evenly distributed (e.g., label 6 occurs equally often in all possible positions within a block) and any trends in the ordering of stimuli are minimized. While these criteria cannot be perfectly satisfied, different sequences depart from this ideal to different degrees. Nonyane and Theobald offer metrics for these departures, and sequences may be selected based upon minimization of these scores.

Sequences may be sought that maximize the orthogonality between labels and position within permuted blocks, and minimize the tendency for there to be "trends" in the order of labels across blocks. These are referred to as Criterion 4 and Criterion 5, and Nonyane and Theobald offer equations to measure these properties.

Sequences may also differ in their [Efficiency](http://www.ncbi.nlm.nih.gov/pubmed/10547338) for use with a BOLD fMRI experiment. This is a measure of the ability of a particular experimental design to create temporal patterns of neural activity that can pass through the hemodynamic response function (HRF) to be represented in the BOLD fMRI signal. The Efficiency of a T1I1 sequence can only be measured with respect to an assumed shape of HRF and some hypothesized magnitude of neural response to the different stimuli to be used.

## Optimal stimulus duration in a T1I1 sequence, carry-over experiment

Due to the low-pass temporal filtering of the HRF, experiments that concentrate power at high temporal frequencies will have decreased Efficiency. Consequently, the longer the duration of stimulus presentation within a T1I1, carry-over experiment, the more Efficient the study will be for the detection of direct effects. Conversely, the magnitude of "carry-over" (e.g., neural adaptation) effects may be attenuated by long stimulus presentations. In practice, we have found a 1.5 second inter-stimulus-interval to work well in these applications.

## Selectively increasing the duration of "null" trials in a T1I1 sequence

The efficiency of a continuous carry-over experiment for detection of the "direct effect" of a stimulus as compared to a null-trial is markedly improved by increasing the duration of the null-trial label each time it appears, as opposed to increasing the number of labels assigned to the null-trial.

## Concatenation of sequences

To increase the total number of observations in a study, it may be desirable to present more than one T1I1 sequence of stimuli. As a T1I1 sequence begins and ends with the same label, one may simply repeat the sequence and still retain the counterbalancing properties. Even better, one may concatenate different T1I1 sequences. To do so, select sequences that all have the same initial label.

It is often the case that a single BOLD fMRI "scan" may be shorter in length than the total duration of stimuli to be presented. In such cases, the complete, concatenated sequence may be broken into sections corresponding to scans of (e.g.) several minutes. The scan boundaries, however, hamper the accurate measurement of the hemodynamic response to the stimulus sequence. At the start of a scan, several seconds are required for the BOLD response to build to a “steady state”, and at the end of a scan, the hemodynamic response to the last presented stimuli continues after data collection has ceased. These effects are eliminated by re-presenting 10 elements of the sequence (15-s worth) at the initiation of each scan, and then trimming those periods from the data in subsequent processing. In the case of the first scan, stimuli from the end of the sequence are presented first. Because this trimming applies to the start and end of the scanning session, the initial, extra duplication stimulus can be dropped, and the final fMRI data represent the entire sequence of stimuli, with the delayed and dispersed BOLD fMRI response to the neural sequence “wrapping around” from end to start.

