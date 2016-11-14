#include "bma.h"


int min(int a, int b) {			//Function returns smaller integer
	if(a < b) {
		return(a);
	}
	else {
		return(b);
	}
}

int max(int a, int b) {			//Function returns bigger integer 
	if(a > b) {
		return(a);
	}
	else {
		return(b);
	}
}

void compute_jumps(char *P, int *CharJump) {			//"Bad character rule"(first heuristics) Function computes maximal possible jump for each char
	int P_len = strlen(P);

	for(int i = 0; i < 256; i++) {			//Assigns maximum jump length for each char
		CharJump[i] = P_len;
	}

	for(int i = 0; i < P_len; i++) {			//For each char of pattern, compute maximum possible jump (how far can you skip to match current char in T with char in P)
		CharJump[(unsigned char)*(P + i)] = P_len - i - 1;
	}
}

void compute_match_jump(char *P, int *MatchJump) {			//"Good suffix rule"(second heuristics) Upon mismatch, function checks if matched suffix occurs in pattern P, if so, function shifts P until the other suffixes do match.
	int P_len = strlen(P);

	int m = P_len;
	int k = 0;
	int q = 0;
	int qq = 0;

	int Backup[P_len];

	for(k = 0; k < m; k++) {			//Fills array with maximum possible jump
		MatchJump[k] = 2 * m - k - 1;
	}

	k = m - 1;			//Index k points to the last character of pattern P
	q = m;			//Index q = pattern length

	while(k >= 0) {			//While k is not negative
		Backup[k] = q;			//Backup q on index k
		
		while((q < m) && (*(P + k) != *(P + q))) {			//While q < m and characters on these pointers are not equal
			
			MatchJump[q] = min(MatchJump[q], m - k - 1);			//Matchjump on index q = smaller value of MatchJump[q] and pattern length - k - 1
			q = Backup[q];			//Load value of q from backup
		}
		k--;			//Decrement index k
		q--;			//Decrement index q
	}

	for(k = 0; k <= q; k++) {			//For k = 0 to q

		MatchJump[k] = min(MatchJump[k], m + q - k);			//Matchjump on index k = smaller value of MatchJump[k] and pattern length + q - k
	}

	qq = Backup[q];			//qq = saved value on index q

	while(q < m - 1) {			//While q < pattern length - 1
		
		while(q <= qq) {			//While q is less than or equal to qq
			
			MatchJump[q] = min(MatchJump[q], qq - q + m);			//Matchjump on index q = smaller value of MatchJump[q] and qq - q + pattern length
			q++;			//Increment q
		}
		qq = Backup[qq];			//Load qq from backup
	}
}

int bma(char *P, char *T, int *CharJump, int *MatchJump) {			//Function returns index of match, or T length + 1
	int P_len = strlen(P);
	int T_len = strlen(T);

	int j = P_len - 1;
	int k = P_len - 1;

	compute_jumps(P, CharJump);			//Computes jumps for the first heuristic of bma
	compute_match_jump(P, MatchJump);			//Computes jumps for the second heuristic of bma

	while(j < T_len && k >= 0) {			//Checks each character (right to left) of P and compares it with characters in T

		if(T[j] == P[k]) {			//If characters do match, decrement indexes (to compare the next char)
			j--;
			k--;
		}
		else {			//If characters do not match, find the highest jump possible and jump
			
			j = j + max(CharJump[(unsigned char)*(T + j)], MatchJump[k]);
			k = P_len - 1;
		}	
	}

	if((k + 1) == 0) {			//If index in pattern P is -1 (Pattern was found), returns its index
		return(j + 1);
	}
	else {
		return(T_len + 1);			//Else returns T length + 1
	}
}

int main(int argc, char **argv) {			//For testing
	char *T = "qhojojahoj";
	char *P = "ahoj";
	int P_len = strlen(P);
	int CharJump[256];
	int MatchJump[P_len];

	int res = bma(P, T, CharJump, MatchJump);
	
	printf("%d\n", res);

	return 0;
}











