#include "builtin.h"
extern resourceStruct * resources;

////////////////////////////////////////////////////////////
///////////////////////Not in builtin///////////////////////
////////////////////////////////////////////////////////////
int ipow(int base, int exp)
{
    int num = 1;
    while (exp != 0) {
        if ((exp & 1) == 1)
            num *= base;
        exp >>= 1;
        base *= base;
    }
    return num;
}

int octToDec(int oct)
{
    int dec = 0, i = 0;
    while(oct != 0) {
        dec += (oct%10) * ipow(8,i);
        ++i;
        oct/=10;
    }
    return dec;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

String readString() {
    int buffSize = LOCAL_BUFF_SIZE, count = 0, c;
	char *buff = (char*)memalloc(buffSize * sizeof(char));      //allocate memory for string

    while((c = getchar()) != EOF && c != '\n') {                //Read everything char by char until new line or EOF
        buff[count] = c;                                        //store char
        count++;                                                //increase char counter
        if(count+2 == buffSize) {                               //realloc if needed
            buffSize += LOCAL_BUFF_SIZE;
            buff = memrealloc(buff, buffSize);
        }
    }

    buff[count] = '\0';                                         //handle end of string manually
    //printf("[%s]--",buff);        //TODO delete
    return buff;
}

int readInt() {                                             //Will convert string into integer

    int num = 0, sign = 1;                                  //sign is 1 by default, it means +
    char* stringNum = readString();                         //make use of readString();
    char makeUseOf;                                         //get rid of warnings
    do{
        if(((*stringNum == '-') || (*stringNum == '+')) && num == 0 )           //first char may be sign, IDK if it's right
        {
             if(*stringNum == '-') sign *= -1;
        }else if(isdigit(*stringNum)) {                                         //if it's digit
            num *= 10;                                                          //multiply num value by 10 and add read digit
            num += *stringNum - '0';
        }else if(*stringNum != '\0' && *stringNum != EOF && *stringNum != 0){   //if it is not digit, print error, call memfree and exit
            fprintf(stderr,"Invalid sequence in readInt() function.\n");
            memfreeall();
            exit(7);
        }
        makeUseOf = *stringNum++;
        makeUseOf += makeUseOf;
    } while (*stringNum != '\0' && *stringNum != EOF && *stringNum != 0);

    return num*sign;
}

double readDouble() {                                                            //Will convert string into double
    double num = 0, mantissa = 0;
    char makeUseOf;                                                              //aka get rid of warnings var
    int intNum = 0, exponent = 0,sign = 1, eSign = 1, hasDot = 0, hasE = 0;      //few bool-ints to handle . e +
    char* stringNum = readString();                                              //make use of readString();

    do{
        if(((*stringNum == '-') || (*stringNum == '+')) && hasE == 1 ) {         //read sign of E, it is + by default
            //printf("exponent sign read, ");   //TODO: delete all printf's
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
            fprintf(stderr,"Invalid sequence in readDouble() function.\n");
            memfreeall();
            exit(7);
        }
        makeUseOf = *stringNum++;
        makeUseOf += makeUseOf;
    } while (*stringNum != '\0' && *stringNum != EOF && *stringNum != 0);

    double fullExp = 1;                             //if there's no exponent, just multiply the number by 1 (in return call)
    if(hasE) {                                      //simple pow-like function to calculate exponent
        for (int i = 0; i < exponent; i++) {
            if(eSign == 1) {                        //if whole exponent is greater than 0, multiply
                fullExp *= 10;
            }else {                                 //if whole exponent is negative, divide until exponent match fullExp number
                fullExp /= 10;
            }
        }
    }
    return ((intNum + mantissa)*fullExp)*sign;      //return number with mantissa but multiply it with calculated exponent (if any) and don't forget the sign
}

//value is the actual string or var name, option may be 0-> it is actual string || 1-> it is just name of variable
void print(char* value, int opt, thTable *htab, char* class) {
    thtabItem* tempItem;
    if(opt == 1){                                                           //value is variable
        tempItem = htabSearch(htab, value);                                 //find variable in local table
        if(tempItem == NULL) {
            if(!strstr(value,".")){		                                    //if called function is short identifier
                tempItem = htabSearchClass(resources->globalVarTable, value, class);    //find var in global table
            }
        }
        if(tempItem == NULL) {                                              //TempItem is still null
            fprintf(stderr, "Error in print function, variable not found.\n");
            memfreeall();
            exit(4);
        }else if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(7);
        }else {                                                             //variable was found so print it out the right way

            if(tempItem->varType == 28 ){
                printf("%d",tempItem->intValue);
            }else if(tempItem->varType == 23){
                printf("%g",tempItem->doubleValue);
            }else if(tempItem->varType == 30 ) {
                //printf("%s",tempItem->stringValue
                char preC = '~';
                char makeUseOf;
                while(*(tempItem->stringValue)) {
                    if(preC == '\\') {                                      //If there is a chance of escape sequence
                        if(isdigit(*(tempItem->stringValue))){              //first check for octal numbers
                            char num[] = "000";
                            num[0] = *(tempItem->stringValue);
                            makeUseOf = *(tempItem->stringValue)++;
                            makeUseOf = makeUseOf;
                            if(isdigit(*(tempItem->stringValue))){          //still finding if octal number is correct
                                num[1] = *(tempItem->stringValue);
                                makeUseOf = *(tempItem->stringValue)++;
                                makeUseOf = makeUseOf;
                                if(isdigit(*(tempItem->stringValue))){
                                    num[2] = *(tempItem->stringValue);
                                    char oct = octToDec(atoi(num));         //conver octal to dec so we cen print it as char
                                    printf("%c",oct);
                                }
                            }
                        }else if(*(tempItem->stringValue) == 'n') {         //there is \n , print new line
                            printf("\n");
                        }else if(*(tempItem->stringValue) == 't'){          //there is \t , print tab space
                            printf("\t");
                        }else if(*(tempItem->stringValue) == '\"'){         //there is \" , print ""
                            printf("\"");
                        }else if(*(tempItem->stringValue) == '\\'){         //there is \\ , print "\"
                            printf("\\");
                        }else {
                            //printf("%c",*(tempItem->stringValue));
                            //TODO: \something , should I EXIT?
                        }
                    }else if(*tempItem->stringValue == '\\'){
                        ;
                    }else {
                        printf("%c",*(tempItem->stringValue));
                    }

                    //prepreC = preC;
                    preC = *(tempItem->stringValue);
                    makeUseOf = *(tempItem->stringValue)++;
                    makeUseOf = makeUseOf;
                }
            }
        }
    }else { //value is just string
        char preC = '!';
        char makeUseOf;
        while(*value) {
            if(preC == '\\') {
                if(isdigit((*value))){
                    char num[] = "000";
                    num[0] = (*value);
                    makeUseOf = *(value)++;
                    makeUseOf = makeUseOf;
                    if(isdigit(*value)){
                        num[1] = *value;
                        makeUseOf = *value++;
                        makeUseOf = makeUseOf;
                        if(isdigit(*value)){
                            num[2] = *value;
                            char oct = octToDec(atoi(num));
                            printf("%c",oct);
                        }
                    }
                }else if((*value) == 'n') {
                    printf("\n");
                }else if((*value) == 't'){
                    printf("\t");
                }else if((*value) == '\"'){
                    printf("\"");
                }else if((*value) == '\\'){
                    printf("\\");
                }else {
                    printf("\\%c",*value);
                }
            }else if((*value) == '\\'){
                ;
            }else {
                printf("%c",*value);
            }

            preC = *value;
            makeUseOf = *(value)++;
            makeUseOf = makeUseOf;
        }
    }
}

int lengthOld(String s){    //simple implementation of strlen
    char *p = s;
    while(*p) {
        p++;
    }
    return p-s;
}

//String s is key, int stringOpt works as in print function (0/1)
int length(String s, int stringOpt, thTable *htab, char* class){
    if(stringOpt) {                 //please look at print() commentary
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		//if called function is short identifier
                tempItem = htabSearchClass(resources->globalVarTable, s, class);        //find global var
            }
        }
        if(tempItem == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in substr function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        s = tempItem->stringValue;
    }
    char *old = s;
    int offset = 0;
    char preC = '!';
    char makeUseOf;
    while(*s) {
        if(preC == '\\') {
            if(isdigit((*s))){
                char num[] = "000";
                num[0] = (*s);
                makeUseOf = *(s)++;
                makeUseOf = makeUseOf;
                if(isdigit(*s)){
                    num[1] = *s;
                    makeUseOf = *s++;
                    makeUseOf = makeUseOf;
                    if(isdigit(*s)){
                        num[2] = *s;
                        char oct = octToDec(atoi(num));
                        offset++;
                    }
                }
            }else if((*s) == 'n') {
                offset++;
            }else if((*s) == 't'){
                offset++;
            }else if((*s) == '\"'){
                offset++;
            }else if((*s) == '\\'){
                offset++;
            }else {
                offset++;
            }
        }
        preC = *s;
        makeUseOf = *(s)++;
        makeUseOf = makeUseOf;
    }

    char *p = old;        //same simple implementation of strlen
    while(*p) {
        p++;
    }
    return (p-old)-offset;
}

//string s is a string to substr, char* iNum can be integer in form or string or var name, rest is same as in print()
String substr(String s, int stringOpt, char* iNum, int iOpt, char* nNum, int nOpt, thTable *htab, char* class) {
    int i=0, n=0;
    i = atoi(iNum); //this may cause problems, maybe strtol is better
    n = atoi(nNum);
    if(stringOpt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		//if called function is short identifier
                tempItem = htabSearchClass(resources->globalVarTable, s, class);        //find global var
            }
        }
        if(tempItem == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in substr function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        s = tempItem->stringValue;
    }
    if(iOpt) {
        thtabItem* tempItem2;
        tempItem2 = htabSearch(htab, iNum);         //find local var
        if(tempItem2 == NULL) {
            if(!strstr(iNum,".")){		//if called function is short identifier
                tempItem2 = htabSearchClass(resources->globalVarTable, iNum, class);        //find global var
            }
        }
        if(tempItem2 == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in substr function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        i = tempItem2->intValue;
    }
    if(nOpt) {
        thtabItem* tempItem3;
        tempItem3 = htabSearch(htab, nNum);         //find local var
        if(tempItem3 == NULL) {
            if(!strstr(nNum,".")){		//if called function is short identifier
                tempItem3 = htabSearchClass(resources->globalVarTable, nNum, class);        //find global var
            }
        }
        if(tempItem3 == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in substr function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        n = tempItem3->intValue;
    }

    if(lengthOld(s) < i+n || i < 0 || n < 0) {
        fprintf(stderr, "Substring error, invalid values\n");
        memfreeall();
        exit(10);
    }
    char *buff = (char*)memalloc(n * sizeof(char));
    for(int x = 0; x < n; x++) {
        buff[x] = s[i+x];
    }
    return buff;
}

//please for details check functions above
int compare(String s1, int s1Opt, String s2, int s2Opt, thTable *htab, char* class) {
    if(s1Opt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s1);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s1,".")){		//if called function is short identifier
                tempItem = htabSearchClass(resources->globalVarTable, s1, class);        //find global var
            }
        }
        if(tempItem == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in compare function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        s1 = tempItem->stringValue;
    }
    if(s2Opt) {         //it is variable
        thtabItem* tempItem2;
        tempItem2 = htabSearch(htab, s2);         //find local var
        if(tempItem2 == NULL) {
            if(!strstr(s2,".")){		//if called function is short identifier
                tempItem2 = htabSearchClass(resources->globalVarTable, s2, class);        //find global var
            }
        }
        if(tempItem2 == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in compare function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        s2 = tempItem2->stringValue;         //set s2 as value of variable
    }

    while(*s1 && (*s1 == *s2))              //while first string has chars, and char of 1st and 2nd string match, pass it
    {
        s1++;
        s2++;
    }
    if((*s1 - *s2) < 0) {                   //if ord. value of s2 char is bigger than ord. value of s1 char, return -1
        return(-1);
    }
    else if((*s1 - *s2) > 0) {              //else if ord. value of s2 char is lesser than ord. value of s1 char, return 1
        return(1);
    }
    else {                                  //else strings do match so return 0 if they are equal/same
        return(0);
    }
}

//please for details check functions above
int find(String s, int stringOpt, String search, int searchOpt, thTable *htab, char* class) {
    if(stringOpt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		//if called function is short identifier
                tempItem = htabSearchClass(resources->globalVarTable, s, class);        //find global var
            }
        }
        if(tempItem == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in find function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        s = tempItem->stringValue;
    }
    if(searchOpt) {
        thtabItem* tempItem2;
        tempItem2 = htabSearch(htab, search);         //find local var
        if(tempItem2 == NULL) {
            if(!strstr(search,".")){		//if called function is short identifier
                tempItem2 = htabSearchClass(resources->globalVarTable, search, class);        //find global var
            }
        }
        if(tempItem2 == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in find function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        search = tempItem2->stringValue;
    }
	int P_len = lengthOld(search);
    int T_len = lengthOld(s);

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

//please for details check functions above
String sort(String s, int stringOpt, thTable *htab, char* class) {
    if(stringOpt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		//if called function is short identifier
                tempItem = htabSearchClass(resources->globalVarTable, s, class);        //find global var
            }
        }
        if(tempItem == NULL) {      //TempItem is still null
            fprintf(stderr, "Error in substr function, variable not found.\n");
            memfreeall();
            exit(4);
        }
        s = tempItem->stringValue;
    }

	int s_len = lengthOld(s);
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
