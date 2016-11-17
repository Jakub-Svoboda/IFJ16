#include "builtin.h"

//extern resourceStruct* resources;


//////////////////////////////
//  AS SIMPLE AS IT CAN BE  //
//  SO PLEASE CHECK IT      //
//////////////////////////////


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
            memfreeall();
            exit(7);
        }
        *stringNum++;
    } while (*stringNum != '\0' && *stringNum != EOF && *stringNum != 0);

    return num*sign;                                                    //return number with right sign
}

double readDouble() {             //Will convert string into integer
    double num = 0, mantissa = 0;
    int intNum = 0, exponent = 0,sign = 1, eSign = 1, hasDot = 0, hasE = 0;
    char* stringNum = readString();     //make use of readString();

    do{
        if(((*stringNum == '-') || (*stringNum == '+')) && hasE == 1 ) {         //read sign of E, it is + by default
            //printf("exponent sign read, ");
            if(*stringNum == '-') eSign *= -1;
        }else if(((*stringNum == '-') || (*stringNum == '+')) && num == 0 ) {    //read sign of number
            //printf("num sign read, ");
            if(*stringNum == '-') sign *= -1;
        }else if(isdigit(*stringNum) && hasE == 0 && hasDot == 0) {              //read the integer part of number
            //printf("int read, ");
            intNum *= 10;
            intNum += *stringNum - '0';
        }else if(isdigit(*stringNum) && hasE == 0 && hasDot == 1) {              //read part of number past .
            mantissa += *stringNum - '0';
            mantissa *= 0.1;
            //printf("mantissa read %f, ",mantissa);
        }else if(isdigit(*stringNum) && hasE == 1) {                             //read part of number past e
            //printf("exponent read, ");
            exponent *= 10;
            exponent += *stringNum - '0';
        }else if(*stringNum == 'e' || *stringNum == 'E') {                       //check if number has exponent
            //printf("e read, ");
            hasE = 1;
        }else if(*stringNum == '.'){                                             //check if number has mantissa
            //printf("dot read, ");
            hasDot = 1;
        }else if(*stringNum != '\0' || *stringNum != EOF || *stringNum != 0){
            fprintf(stderr,"Invalid sequence in readInt() function.\n");
            memfreeall();
            exit(7);
        }
        *stringNum++;
    } while (*stringNum != '\0' && *stringNum != EOF && *stringNum != 0);

    double fullExp = 1;
    if(hasE) {                                      //simple pow-like function to calculate exponent
        for (int i = 0; i < exponent; i++) {
            if(eSign == 1) {
                fullExp *= 10;
            }else {
                fullExp /= 10;
            }
        }
    }

    return ((intNum + mantissa)*fullExp)*sign;                                                    //return number with right sign
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
        memfreeall();
        exit(10);
    }
    char *buff = (char*)memalloc(n * sizeof(char));					//ZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZ
    for(int x = 0; x < n; x++) {
        buff[x] = s[i+x];
    }
    return buff;
}

int compare(String s1, String s2) {
    while(*s1 && (*s1 == *s2))      //Don't know if its "Lexikograficky porovná" because I don't know what does it mean
    {                               //but this looks cool anyway so we'll just stick to it.
        s1++;
        s2++;
    }
    if((*s1 - *s2) < 0) {
        return(-1);
    }
    else if((*s1 - *s2) > 0) {
        return(1);
    }
    else {
        return(0);
    }
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
    char *str = (char*)memalloc(sizeof(char)*s_len);
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
