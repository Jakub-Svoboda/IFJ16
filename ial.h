#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void partition(char *, int, int, int *, int *);
void quick_sort(char *, int, int);

int min(int, int);
int max(int, int);
void compute_jumps(char *, int *);
void compute_match_jump(char *, int *);
int bma(char *, char *, int *, int *);