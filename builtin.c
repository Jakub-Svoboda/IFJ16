#include "builtin.h"

//extern resourceStruct* resources;


//////////////////////////////
//  AS SIMPLE AS IT CAN BE  //
//  SO PLEASE CHECK IT      //
//////////////////////////////



double readDouble() {
    return 0.0;
}

String readString() {
    int buffSize = LOCAL_BUFF_SIZE, count = 0, c;
	char *buff = (char*)memalloc(buffSize * sizeof(char));

    while((c = getchar()) != EOF && c != '\n') {                //Read everything until new line or EOF
        buff[count] = c;
        count++;
        if(count+2 == buffSize) {                               //realloc if needed
            buffSize += LOCAL_BUFF_SIZE;
            buff = memrealloc(buff, buffSize);
        }
    }

    buff[++count] = '\0';
    return buff;
}

int readInt() {             //Will convert string into integer
    int num = 0, sign = 1;
    char* stringNum = readString();     //make use of readString();

    do{
        if(((*stringNum == '-') || (*stringNum == '+')) && num == 0 )    //first char may be sign, IDK if it's right
        {
             if(*stringNum == '-') sign *= -1;
        }else if(isdigit(*stringNum)) {                                 //if it's digit
            num *= 10;
            num += *stringNum - '0';
        }else if(*stringNum != '\0' || *stringNum != EOF || *stringNum != 0){   //if it is not digit, print error and exit
            fprintf(stderr,"Invalid sequence in readInt() function.\n");
            exit(7);
        }
        *stringNum++;
    } while (*stringNum != '\0' && *stringNum != EOF && *stringNum != 0);

    return num*sign;                                                    //return number with right sign
}

/*char* printCreate(Token *t) {
    char* printSequence;
    int termI;
    double termD;                           //not working , will do later
    t = NULL;
    return "dd";
}*/

void print() {
    ;
}

int length(String s){
    char *p = s;
    while(*p) {
        p++;
    }
    return p-s;
}

String substr(String s, int i, int n) {
    if(length(s) < i+n || i < 0 || n < 0) {
        fprintf(stderr, "Substring error, invalid values\n");
        exit(10);
    }
    char *buff = (char*)memalloc(n * sizeof(char));					//ZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZ			//This also does not work with memalloc when i try it
    for(int x = 0; x < n; x++) {
        buff[x] = s[i+x];
    }
    return buff;
}

int compare(String s1, String s2) {
    s1 = s2;        // not working
    s2 = s1;        // just to get rid of warnings
    return 0;       // delete later
}

int find(String s, String search) {
	int P_len = length(search);
    int T_len = length(s);

    int CharJump[256];          //Max char
    int MatchJump[P_len];

    int res = bma(search, s, CharJump, MatchJump);

    if(res < T_len) {
        return(res);
    }
    else {
        return(-1);
    }
}

String sort(String s) {
	int s_len = length(s);
    char *str = (char*)memalloc(sizeof(char)*s_len);          //Does not work whem memalloc is used, don't know why..
    for(int x = 0; x < s_len; x++) {
        str[x] = s[x];
    }
    int left = 0;
    int right = strlen(str) - 1;
    quick_sort(str, left, right);

    return(str);
}

//testing purposes
/*
int main() {
	String k = "qhojojahoz";
	String l = "z";

    int res = find(k, l);
    printf("%d\n", res);

    String sorted = sort(k);
    printf("Serazeny retezec: {%s}\n", sorted);


    String s = "Kekel";
    printf("Zadarilo se a substr je: {%s}\n", substr(s, 0, 4));
    return 1;
}
*/
