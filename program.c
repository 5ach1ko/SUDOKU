/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Jiawen Wang, ID=1266494
   Dated:     09/02/2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3		/* sudoku dimension, size of each inner square */
#define NDIG (NDIM*NDIM)
			/* total number of values in each row */
#define NGRP 3		/* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
			/* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
			/* total number of cells in the sudoku */

#define ERROR	(-1)	/* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM*NDIM] = {
	/* the first group of nine sets describe the sudoku's rows */
	{  0,  1,  2,  3,  4,  5,  6,  7,  8 },
	{  9, 10, 11, 12, 13, 14, 15, 16, 17 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },
	{ 27, 28, 29, 30, 31, 32, 33, 34, 35 },
	{ 36, 37, 38, 39, 40, 41, 42, 43, 44 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53 },
	{ 54, 55, 56, 57, 58, 59, 60, 61, 62 },
	{ 63, 64, 65, 66, 67, 68, 69, 70, 71 },
	{ 72, 73, 74, 75, 76, 77, 78, 79, 80 },
	/* the second group of nine sets describes the sudoku's columns */
	{  0,  9, 18, 27, 36, 45, 54, 63, 72 },
	{  1, 10, 19, 28, 37, 46, 55, 64, 73 },
	{  2, 11, 20, 29, 38, 47, 56, 65, 74 },
	{  3, 12, 21, 30, 39, 48, 57, 66, 75 },
	{  4, 13, 22, 31, 40, 49, 58, 67, 76 },
	{  5, 14, 23, 32, 41, 50, 59, 68, 77 },
	{  6, 15, 24, 33, 42, 51, 60, 69, 78 },
	{  7, 16, 25, 34, 43, 52, 61, 70, 79 },
	{  8, 17, 26, 35, 44, 53, 62, 71, 80 },
	/* the last group of nine sets describes the inner squares */
	{  0,  1,  2,  9, 10, 11, 18, 19, 20 },
	{  3,  4,  5, 12, 13, 14, 21, 22, 23 },
	{  6,  7,  8, 15, 16, 17, 24, 25, 26 },
	{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
	{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
	{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
	{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
	{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
	{ 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function 
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
	int s=0, d=0, c;
	for ( ; s<NSET; s++) {
		/* record the first set number each cell is part of */
		for (c=0; c<NDIM*NDIM; c++) {
			c2s[s2c[s][c]][d] = s;
		}
		if ((s+1)%(NGRP*NDIM) == 0) {
			d++;
		}
	}
#if 0
	/* this code available here if you want to see the array
	   c2s[][] that gets created, just change that 0 two lines back
	 gcc -Wall -o programprogram.c-lm  to a 1 and recompile */
	for (c=0; c<NCLL; c++) {
		printf("cell %2d: sets ", c);
		for (s=0; s<NGRP; s++) {
			printf("%3d", c2s[c][s]);
		}
		printf("\n");
	}
	printf("\n");
#endif
	return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
	return 1 + (c/(NDIM*NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
	return 1 + (c%(NDIM*NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
	return 1 + 3*(c/NSET) + (c/NDIM)%NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/


/****************************************************************/
/****************************************************************/

int print(int CELL[]);
int solve(int cell[]);

/* main program controls all the action*/
int
main(int argc, char *argv[]) {
    fill_c2s();
    int num, zero=0, CELL[81], i=0, err=0;
    /*Stage 01 print out a sudoku*/
    printf("\n");
    while (scanf("%d", &num) == 1) {
    
    /*This little array here is giving values for each cell*/
    
        CELL[i] = num;
        i += 1;
    }
    zero = print(CELL);
    printf("%2d cells are unknown\n", zero);
    printf("\n");
    /*End of Stage 1*/
    
    /*Stage 02 check a sudoku*/
    /*rep = how many times voilation happens*/
    
    int set, previous_set = 0, rep=0, distinct=0;
    for (set=0; set<NSET; set++) {
    
    /*This is a very big brain idea:*/
    /*Use the array index as the number 1~9 directly compare to the cell and 
    return the frequency for each index of the array.*/
    
        int freq[10] = {0,0,0,0,0,0,0,0,0,0};
        int cell, num;
        for (cell=0; cell<NDIG; cell++) {
            for (num=1; num<=NDIG; num++) {
                if (CELL[s2c[set][cell]] == num) {
                    freq[num] += 1;
                    break;
                }
            }
        }
        
        /*Define the set that has the violations and check if the violations 
        already happen in same set before.*/
        
        for (num=1; num<=NDIG; num++) {
            if (freq[num] > 1) {
            /*If the sudoku is not vaild then define this sudoku is ERROR which
            err variable now used here, which initially defined at line 180.*/
                err = 1;
                rep += 1;
                if (set != previous_set) {
                    distinct += 1;
                }
                if (0<=set && set<=8) {
                    printf("set %2d (row %d): %d instances of %d\n", 
                    set, set+1, freq[num], num);
                }
                else if (9<=set && set<=17) {
                    printf("set %2d (col %d): %d instances of %d\n", 
                    set, set-8, freq[num], num);
                }
                else if (18<=set && set<=26) {
                    printf("set %2d (sqr %d): %d instances of %d\n", 
                    set, set-17, freq[num], num);
                }
                previous_set = set;
            }
        }
    }
    /*Only print message if there are violations.*/
    if (rep >= 1) {
        printf("\n");
        printf("%d different sets have violations\n", distinct);
        printf("%d violations in total\n", rep);
    }
    /*End of Stage 2*/
    /*FINAL STAGE*/
    /*Only NONERROR sudokus will be solved otherwise stop running this stage*/
    
    int solved;
    if (err==0) {
        while (zero > 0) {
            solved = solve(CELL);
            
            /*If we can't use strategy one to solve anymore, then just stop it 
            anyway*/
            if (solved == 0) {
                break;
            }
            printf("\n");
        }
        zero = print(CELL);
        printf("%2d cells are unknown\n", zero);
        printf("\n");
        /*How is going? Is that fully solved?*/
        if (zero == 0) {
            printf("ta daa!!!\n");
        }
    }
    /*STAGE CLEAR! Algorithms are FUN!*/
	/* all done, so pack up bat and ball and head home */
	return 0;
}
    

int solve(int cell[]){
    fill_c2s();
    int i, set, index, num, solved = 0;
    int solvecell[81];
    /*Empty this shadow sudoku cell every time.*/
    for(i=0; i<NCLL; i++) {
        solvecell[i] = 0;
    }
    
    /*Similar to stage 02:*/
    /*Use the array index as the number 1~9 directly compare to the cell and 
    shutdown the possibilities of numbers can be selected.*/
    
    for(i=0; i<NCLL; i++) {
        if (cell[i] == 0) {
            int candidate[10] = {0,1,1,1,1,1,1,1,1,1}, possible = 9;
            for(set=0; set<NGRP; set++) {
                for(index=0; index<NDIG; index++) {
                    for(num=1; num<=9; num++) {
                        if (cell[s2c[c2s[i][set]][index]] == num && 
                            candidate[num] == 1) {
                            candidate[num] = 0;
                            possible -= 1;
                        }
                    }
                }
            }
            /*If we last with only 1 possibility, then this cell solved*/
            if (possible == 1) {
                for(num=1; num<=9; num++) {
                    if(candidate[num] == 1) {
                        /*And the number is the index of candidate array*/
                        solvecell[i] = num;
                        solved += 1;
                        break;
                    }
                }
            }
        }
    }
    /*Count how many cells are solved in the same time*/
    if (solved != 0) {
        printf("strategy one\n");
        for(i=0; i<NCLL; i++) {
            if (solvecell[i] != 0) {
                printf("row %d col %d must be %d\n",
                (i/9)+1, (i%9)+1, solvecell[i]);
                /*And put these numbers back to original sudoku*/
                cell[i] = solvecell[i];
            }
        }
    }
    return solved;
}


int print(int CELL[]) {
    int index=0, zero=0;
    while (index<NCLL) {
    if (CELL[index] == 0) {
        /*Here this zero is used to count unknowns*/
            zero += 1;
            if (index%9 == 8) {
                printf(".");
            }
            else {
                printf(". ");
            }
        }
        else {
        if (index%9 == 8) {
                printf("%d", CELL[index]);
            }
            else {
                printf("%-2d", CELL[index]);
            }
        }
        index += 1;
        /*Note this lead to an neseccery warning there, because this index += 1
        is used to detect the situation desctibed below, which tellsus if we 
        are reaching the square's end, a row's end or the sudoku's end*/
        
        /*Format: For cells generated, 
        3 for a "|" to split, 9 for a new line, 27 for a split line
        Note at the end of sudoku we don't need another split line*/
        if ((index)%3==0) {
            if ((index)%9==0) {
                printf("\n");
                if ((index)%27==0 && (index)%81!=0) {
                    printf("------+-------+------\n");
                }
            }
            else {
                printf("| ");
            }
        }
    }
    printf("\n");
    return zero;
}
/****************************************************************/
