/************************************************************************** 
 * Program for Systematic Search of Type I Sequences with Index 1

 * This program generates all possible Type I sequences according to
 * the systematic procedure described in Chapter 3. The value of
 * n may be changed accordingly

 * Generate multiple sequences 

 * Written by Dongbo Hu <dongbo@mail.med.upenn.edu> , 6/22/2005
 * quiet mode (-q, -l) added by Daniel Drucker <ddrucker@psych.upenn.edu> , 5/22/2007
 * total limit added by Daniel Drucker 12/18/2007
 * thanks to Ben Karel <ben@eschew.org> for dynamic setting of # terms (-n)
 **************************************************************************/
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "anyoption.h"

using namespace std;

bool quiet = 0;
int n = 17;
int total;
typedef int* seq_t;
seq_t seq;
int seed = time(NULL);

int check1(seq_t s, int curr, int beg, int pos);
int check2(seq_t s, int curr, int prev, int cbmark);
int check3(seq_t s, int index);
int getInit(int index);
int getRand(int index);

void printing(seq_t s);
void println(seq_t s);
void println_oneline(seq_t s);

void confirm(seq_t s);
void evaluate(seq_t s);
int searchCol(int inputNum, seq_t s, int col);
int get4(int** s);
int get5(int** s);
int atoi(string s) { stringstream ss(s); int n; ss >> n; return n; }
int main(int argc, char* argv[]) {

  // grab and parse command line 
  AnyOption *opt = new AnyOption();
  opt->addUsage( "Usage: ");
  opt->addUsage( " -h --help            Prints this help");
  opt->addUsage( " -l --line            Print terms on one line instead of a square matrix");
  opt->addUsage( " -q --quiet           minimal output format (implies -l)");
  opt->addUsage( " -n --length 17       Number of terms ");
  opt->addUsage( " -t --total 999    Stop after this many found. Note that many may be duplicates with small values of -n. Default is to just keep going.");

    
  opt->setFlag( "help", 'h');
  opt->setOption( "length", 'n');
  opt->setFlag( "line", 'l');
  opt->setFlag( "quiet", 'q');
  opt->setOption( "total", 't');

  opt->processCommandArgs (argc, argv);
  if( opt->getFlag( "help") || opt->getFlag('h')) { // print usage if no options
	 opt->printUsage();
	 delete opt;
	 return 1;
  }
  quiet = (opt->getFlag('q') || opt->getFlag("quiet"));

  if( opt->getValue( 'n' ) != NULL  || opt->getValue( "length" ) != NULL  )
	n = atoi(opt->getValue('n'));

  if( opt->getValue( 't' ) != NULL  || opt->getValue( "total" ) != NULL  )
	total = atoi(opt->getValue('t'));

  seq =(seq_t) malloc(sizeof(int) * n * n - 1);


  // For log purpose
  if (!quiet) system("date");
  if (!quiet) cout << endl;

  //initializate seq[]
  for (int j = 0; j < n * n + 1; j++)
    seq[j] = getRand(j); 
  /* now continue from n+2 to end of sequence */
  int j = n + 2;
  int bmark;
  unsigned long counter = 0;
  unsigned long success = 0;
  unsigned long startTime = time(NULL);
  while (j < n * n + 2) {
    // If sequence not found in 60 minutes, reinitialize it and search again
    if (time(NULL) - startTime >= 3600) {
		if (!quiet) {
		  cout << "############################################" << endl;
		  cout << "# Not found in 60 minutes, reinitialize ... " << endl;
		  cout << "############################################" << endl;
		}
      if (!quiet) system("date");
      for (int x = 0; x < n * n + 1; x++)
	seq[x] = getRand(x);
      j = n + 2;
      counter = 0;
      startTime = time(NULL);
      continue;
    }

    // if seq[n + 1] is equal to n, reinitialize the whole sequence and search again
    if (seq[n + 2] == n) {
		if (!quiet) {
		  cout << "############################################" << endl;
		  cout << "# seq[n+2] is equal to n, reinitialize ...  " << endl;
		  cout << "############################################" << endl;
		}
      if (!quiet) system("date");
      for (int x = 0; x < n * n + 1; x++)
	seq[x] = getRand(x);
      j = n + 2;
      counter = 0;
      startTime = time(NULL);
      continue;
    }
    // If search reaches the fixed elements, reinitialize the whole sequence and search again
    if (j < n + 2) {
		if (!quiet){
		  cout << "############################################" << endl;
		  cout << "# Fixed elements reached, reinitialize ...  " << endl;
		  cout << "############################################" << endl;
		}
      for (int x = 0; x < n * n + 1; x++)
	seq[x] = getRand(x);
      j = n + 2;
      counter = 0;
      startTime = time(NULL);
      continue;
    }

    counter++;
    /************************************************************
     * print good sequence and generate another one based on it
     ************************************************************/
    if (j == n * n + 1) {
      success++;
	if (total && (success > total)) {
		if (!quiet) cout << "Reached stated number of sequences (" << total << "), exiting." << endl;
		return(0);
	}
      if (!quiet) {
		  system("date");
		  cout << "counter = " << counter;
		  cout << ", Sequence #" << success << ": " << endl;
		}

		if (quiet || opt->getFlag('l') || opt->getFlag("line"))
		  println_oneline(seq);
		else
		  println(seq);

		if (!quiet)
		  cout << endl;

      confirm(seq);
      evaluate(seq);
      if (!quiet) cout << endl;

      /* Instead of starting new search based on the previous good sequence, we initialize 
       * each element again so that the next good sequence is not close to the previous one */
      for (int x = 0; x < n * n + 1; x++)
	seq[x] = getRand(x);

      j = n + 2;
      counter = 0;
      startTime = time(NULL);
      continue;
    }  // end of "j == n * n"

    bmark = (j - 1) / n * n + 1;
    // when the current position is not at the begininng of a new block
    if (j % n != 1) {
      // when current element is found earlier in same block
      if (check1(seq, seq[j], bmark, j) == 1) {	
	// If current element is less than n, increment by 1 and test it again
	if (seq[j] < n) {
	  seq[j]++;
	  continue;
	}
	/* If current element is equal to n and previous element is less than
	 * n, we reset the current element to 1, increment previous element 
	 * by 1, and go back one step and test it again.                            */ 
	if (seq[j - 1] < n) {
	  seq[j - 1]++;
	  seq[j] = getInit(j);
	  j--;
	  continue;
	}
	/* If both current and previous element are equal to n, the action depends on
	 * j's position in the current block. If j is not the 3rd element in current block,
	 * it must be after 3rd position. The reason is:
	 *
	 * (a) since j % n != 1, it couldn't be 1st.
	 *
	 * (b) If it is 2nd element in the block, this block will be: <n> <n> ...,
	 *     this means the last element in previous block is also <n>. If this is true, 
	 *     it wouldn't pass the previous step, because <n><n> combination already
	 *     showed up at the beginning of the second block. When <n><n> shows up 
	 *     in second block, it indicates the end of search and the whole program will exit.
	 *
	 * So if the current element is not the third one in the block, the element
	 * is right before the previous one must be less than n (otherwise the 
	 * previous step won't pass), increment seq[j-2] by 1, reset seq[j] and seq[j-1]
	 * back to 1, and set j-2 as current position for testing.                    */
	if (j % n != 3) {
	  j = j - 2;
	  seq[j]++;
	  seq[j + 1] = getInit(j + 1);
	  seq[j + 2] = getInit(j + 2);
	  continue;
	}
	/* If the current element is the third one in current block, this block will be like:
	 *
	 * m <n> <n> ... (m is less thann)
	 *
	 * so if m is incremented by 1, the first element in this block wouldn't match the 
	 * last one in prevoius block. If the last element in previous block is incremented by 1,
	 * this value wouldn't fit either, because m is the only number left for that position.
	 * So we have to go back to the SECOND last element in prevoius block (its index is j-4). 
	 *
	 * If seq[j-4] is less than n, increment it by 1, reset the next 4 elements 
	 * back to 1, set j-4 as current position for testing. */
	if (seq[j - 4] < n) {
	  j = j - 4;
	  seq[j]++;
	  seq[j + 1] = getInit(j + 1);
	  seq[j + 2] = getInit(j + 2);
	  seq[j + 3] = getInit(j + 3); 
	  seq[j + 4] = getInit(j + 4);
	  continue;
	}
	/* if seq[j-4] is equal to n, seq[j-5] must be less than n, 
	 * increment seq[j-5] by 1, reset the next 5 elements back to 1, 
	 * set j-2 as current position for testing.        */
	j = j - 5;
	seq[j]++;
	seq[j + 1] = getInit(j + 1);
	seq[j + 2] = getInit(j + 2);
	seq[j + 3] = getInit(j + 3); 
	seq[j + 4] = getInit(j + 4);
	seq[j + 5] = getInit(j + 5);
	continue;
      }

      /* When the current element doesn't show up earlier in current block
       * but the seq[j-1]--seq[j] pair is found earlier */
      if (check2(seq, seq[j], seq[j -  1], bmark) == 1) {
	/* if current element is less than n, 
	 * increment it by 1 and test same position again */
	if (seq[j] < n) {
	  seq[j]++;
	  continue;
	}
	/* If the current element is equal to n, the previous element must be less than 
	 * n, otherwise it wouldn't pass check1 test.
	 * Our action depends on j's position in current block:  
	 *
	 * if j is not 2nd element in current block, it must be after 2nd element. 
	 * (See reasons above)
	 * we increment the previous element, reset current element back to 1, 
	 * change current position to j-1 for test.                          */
	if (j % n > 2 || j % n == 0) {	
	  seq[j - 1]++;
	  seq[j] = getInit(j);
	  j = j - 1;
	  continue;
	}
	/* If j is 2nd element in current block, this block will be like:
	 *
	 * m <n> ...  (m is less than n)
	 * 
	 * So it wouldn't make sense if we increment m by 1 and test it, because it certainly 
	 * isn't good, because m+1 doesn't match the last element in previous block. We can't 
	 * increment the last element in previous element either, because as the last element,
	 * that one doesn't have any other choice. Once again we have to modify the SECOND last
	 * element in previous block (its index is j-3).	

	 * if seq[j-3] is less than n, increment it by 1, set three element 
	 * after it back to 1, change current position back to j-3 and test it. */
	if (seq[j - 3] < n) {
	  j = j - 3;
	  seq[j]++;
	  seq[j + 1] = getInit(j + 1);
	  seq[j + 2] = getInit(j + 2);
	  seq[j + 3] = getInit(j + 3); 
	  continue;
	}
	/* If seq[j-3] is equal to n, seq[j-4] must be less than n. 
	 * Increment seq[j-4] by 1, set the next 4 elements after it back to 1,
	 * then change current position back to j-3 and test it. */
	j = j - 4;
	seq[j]++;
	seq[j + 1] = getInit(j + 1);
	seq[j + 2] = getInit(j + 2);
	seq[j + 3] = getInit(j + 3); 
	seq[j + 4] = getInit(j + 4);
	continue;
      }
      /* If seq[j] is lucky enough to reach this point, it must be 
       * a good element, increment j by 1 to test the next element. */  
      j++;
      continue;
    }

    /* When current element is 1st in current block, if seq[j-1]--seq[j-1] pair
     * never shows up before, set seq[j] equal to seq[j-1], go back to next element.*/
    if (check3(seq, j) == 0) {
      seq[j] = seq[j - 1];
      j++;
      continue;
    }
    /* If seq[j-1]--seq[j-1] pair does show up earlier, we have to modify the SECOND
     * last element in previous block (its index is j-2). 
     * 
     * If seq[j-2] is less than n, increment it by 1, set the next 2 elements 
     * after it back to 1, then change current position back to j-2 and test it. */
    if (seq[j - 2] < n) {
      j = j - 2;
      seq[j]++;
      seq[j + 1] = seq[j + 2] = 1;
      continue;
    }
    /* If seq[j-2] is equal to n, seq[j-3] must be less than n. 
     * So we increment seq[j-3] by 1, set the next 3 elements after it back 
     * to 1, then change current position back to j-3 and test it. */
    j = j - 3;
    seq[j]++;
    seq[j + 1] = getInit(j + 1);
    seq[j + 2] = getInit(j + 2);
    seq[j + 3] = getInit(j + 3); 
    continue;      
  } // end of while (j < n * n + 2)

  if (!quiet)  cout << "Total # of sequences: " << success << endl;

  // For log purpose
  cout << endl;
  if (!quiet) system("date");

  return 0;
} // end of main

/* a function to check if a letter already exists */
int check1(seq_t s, int curr, int beg, int pos)
{
  // check if symbol was repeated or not
  for (int k = beg; k < pos; k++) {
    if (s[k] == curr) 
      return 1;
  }
  return 0;
}

/* a function to check if a pair already exists in sequence */
int check2(seq_t s, int curr, int prev, int cbmark)
{
  for (int i = 0; i < cbmark; i++) {
    if (s[i] == prev && s[i+1] == curr)
	return 1; // pair already exists
  }
  return 0;       // otherwise pair does not exist yet
}

/* This function checks the first element in current block.
 * returns 0 if seq[j-1]--seq[j-1] pair is not found before j,
 * returns 1 otherwise.                                     */
int check3(seq_t s, int index)
{
  int block_num = (index - 1) / n - 1;
  for (int i = 0; i < block_num; i++) {
    if (seq[index - 1] == seq[i * n + 1])
      return 1;
  }

  return 0;
}

/* This function returns the initialized value for a certain index */
int getInit(int index)
{
  // seq[0] is always 1
  if (index == 0)
    return 1;
  // seq[1] ~ seq[n] is equal to the index
  if (index <= n)
    return index;
  // 1st element in second block (seq[n+1] is equal to n
  if (index == n + 1)
    return n;
  // The last element in second last block and all elements in last block (except the last one) should be 2
  if (index >= n * n - n && index != n * n)
    return 2;
  // All other elements should be 1
  return 1;
}

/* This function generates most of elements in seq[] randomly */
int getRand(int index)
{
  // seq[0] is always 1
  if (index == 0)
    return 1;
  // seq[1] ~ seq[n] is equal to the index
  if (index <= n)
    return index;
  // 1st element in second block (seq[n+1] is equal to n
  if (index == n + 1)
    return n;

  // The rest of elements are generated randomly
  seed++;
  srand(seed);
  /* 2nd element in second block is special: do NOT set it to be n, 
   * otherwise search will start from the very beginning again. 
   * Instead, set it to be a random number between 1 and n-1 */
  int myRand;
  if (index == n + 2)
    myRand = rand() % (n - 1) + 1;
  else
    myRand = rand() % n + 1;
  return myRand;
}

/* a function for printing out sequencese */
void printing(seq_t s)
{
  for (int i = 0; i < n * n + 1; i++) {
    cout<< s[i] << ' ';
    if (i % n == 0)
      cout << ' ';
  }
  cout << endl;
}

/* another function for printing out sequencese */
void println(seq_t s)
{
  for (int i = 0; i < n * n + 1; i++) {
	 printf("%3d", s[i]);
    if (i % n == 0)
      cout << endl;
  }
}

/* another function for printing out sequencese */
void println_oneline(seq_t s)
{
  for (int i = 0; i < n * n + 1; i++)
    printf("%d,", s[i]);
}


/* This function confirms an input sequence is good */
void confirm(seq_t s)
{
  bool status = true;
  // Initialize two-dimention pair
  int pair[n + 1][n + 1];
  for (int i = 0; i < n + 1; i++) {
    for (int j = 0; j < n + 1; j++) {
      if (i == 0 || j == 0)
	pair[i][j] = -1;
      else
	pair[i][j] = 0;
    }
  }
  // If a certain pair shows up, set pair[row][col] to 1;
  // If it is already 1, print out error message
  for (int k = 0; k < n * n; k++) {
    int row = seq[k];
    int col = seq[k + 1];
    if (pair[row][col] == 1) {
      printf("Pair showed earlier: %d--%d\n", row, col);
      status = false;
    }
    else
      pair[row][col] = 1;
  }

  // Check pair[][] agian to make sure all possible pairs show up
  for (int i = 1; i < n + 1; i++) {
    for (int j = 1; j < n + 1; j++) {
      if (pair[i][j] ==0) {
	printf("Pair not found: %d--%d\n", i, j);
	status = false;
      }
    }
  }
  // print out final status message
  if (!quiet){
	 if (status)
		cout << "Confirmation status: positive" << endl;
	 else
		cout << endl << "Confirmation status: negative" << endl;
  }
}

/* This function evaluates the sequence and prints out criterion 4 and criterion 5 value */
void evaluate(seq_t s)
{
  //int matrix[n][n];
    int** matrix = (int**) malloc(sizeof(int*) * n);
  //cout << "matrix is: " << endl;
  for (int i = 0; i < n; i++) {
  matrix[i] = (int*) malloc(sizeof(int) * n);
    for (int j = 0; j < n; j++) {
      int x = searchCol(i + 1, s, j + 1);
      matrix[i][j] = x;
      //cout << matrix[i][j] << " ";
    }
    //cout << endl;
  }

  if (quiet){
	 cout << get4(matrix) << "," << get5(matrix) << endl;
  } else {
	 cout << "Criterion 4: " << get4(matrix) << endl;
	 cout << "Criterion 5: " << get5(matrix) << endl;
  }


}

/* This function returns the number of times inputNum shows up in s[][]'s certain column */
int searchCol(int inputNum, seq_t s, int col)
{
  int counter = 0;
  for (int i = col; i < n * n + 1; i+= n) {
    if (s[i] == inputNum)
      counter++;
  }
  return counter;
}
  
/* This function returns criterion 4 value */
int get4(int** s)
{
  int sum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      sum+= (s[i][j] - 1) * (s[i][j] - 1);
  }
  
  return sum;
}

/* This function returns criterion 5 value */
int get5(int** s)
{
  int sum = 0;
  int lineSum = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      lineSum += (j + 1) * (s[i][j] - 1);
    }
    sum += lineSum * lineSum;
    lineSum = 0;
  }

  return sum;
}
