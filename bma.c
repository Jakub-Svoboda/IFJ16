#include "bma.h"


int min(int a, int b) {
	if(a < b) {
		return(a);
	}
	else {
		return(b);
	}
}

int max(int a, int b) {
	if(a > b) {
		return(a);
	}
	else {
		return(b);
	}
}

void compute_jumps(char *P, int *CharJump) {
	int P_len = strlen(P);

	for(int i = 0; i < 256; i++) {
		CharJump[i] = P_len;
	}

	for(int i = 0; i < P_len; i++) {
		CharJump[(unsigned char)*(P + i)] = P_len - i - 1;
	}
}

void compute_match_jump(char *P, int *MatchJump) {
	int P_len = strlen(P);

	int m = P_len;
	int k = 0;
	int q = 0;
	int qq = 0;

	int Backup[P_len];

	for(k = 0; k < m; k++) {
		MatchJump[k] = 2 * m - k;
	}

	k = m - 1;
	q = m;

	while(k >= 0) {
		Backup[k] = q;
		
		while((q < m) && (P[k] != P[q])) {
			
			MatchJump[q] = min(MatchJump[q], m - k);
			q = Backup[q];
		}
		k--;
		q--;
	}

	for(k = 0; k < q; k++) {

		MatchJump[k] = min(MatchJump[k], m + q - k);
	}

	qq = Backup[q];

	while(q < m) {
		
		while(q <= qq) {
			
			MatchJump[q] = min(MatchJump[q], qq - q + m);
			q++;
		}
		qq = Backup[qq];
	}
}

int bma(char *P, char *T, int *CharJump, int *MatchJump) {
	int P_len = strlen(P);
	int T_len = strlen(T);

	int j = P_len - 1;
	int k = P_len - 1;

	compute_jumps(P, CharJump);
	compute_match_jump(P, MatchJump);

	while(j < T_len && k >= 0) {

		if(T[j] == P[k]) {
			j--;
			k--;
		}
		else {
			
			j = j + max(CharJump[(unsigned char)*(T + j)], MatchJump[k]);
//			j = j + MatchJump[k];
			k = P_len - 1;
		}	
	}

	if((k + 1) == 0) {
		return(j + 2);
	}
	else {
		return(T_len + 1);
	}
}

int main(int argc, char **argv) {
	char *T = "qhojojahoj";
	char *P = "ahoj";
	int P_len = strlen(P);
	int CharJump[256];
	int MatchJump[P_len];

	int res = bma(P, T, CharJump, MatchJump);
	
	printf("%d\n", res);

	return 0;
}











