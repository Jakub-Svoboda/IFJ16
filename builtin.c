/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    builtin.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

//TODO: check fprintfs

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

int lengthOld(String s){    //simple implementation of strlen
    char *p = s;
    while(*p) {
        p++;
    }
    return p-s;
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


int binToDec(int bin)			//function to convert binary numbers to decimal
{
    int dec = 0, i = 0;
    while(bin != 0) {
        dec += (bin%10) * ipow(2,i);
        ++i;
        bin/=10;
    }
	return dec;			//but return it as string
}

int hexadecToDecI(char* hexadec)
{
    int dec = 0, i = 0, count = 0, quiet;
	char* temp = hexadec;
	//printf("kek\n" );
	while(*temp != '\0'){			//get position of end of string
		;
		quiet = *temp++;
	}
	quiet = quiet;
	quiet = *temp--;
    while(temp != hexadec) {
        //printf("once %c ",*temp);
		if(*temp == 'A') {
			i = 10;
		}else if(*temp == 'B'){
			i = 11;
		}else if(*temp == 'C'){
			i = 12;
		}else if(*temp == 'D'){
			i = 13;
		}else if(*temp == 'E'){
			i = 14;
		}else if(*temp == 'F'){
			i = 15;
		}else {
			i = (*temp) - '0';
		}
		dec += i * ipow(16,count);
        ++count;
		temp--;
    }
    if(*temp == 'A') {
        i = 10;
    }else if(*temp == 'B'){
        i = 11;
    }else if(*temp == 'C'){
        i = 12;
    }else if(*temp == 'D'){
        i = 13;
    }else if(*temp == 'E'){
        i = 14;
    }else if(*temp == 'F'){
        i = 15;
    }else {
        i = (*temp) - '0';
    }
    dec += i * ipow(16,count);
    ++count;
    temp--;

	return dec;				//return parsed hexadecimal number
}

//function to convert all combinations of hexadecimal number to double or integer and return it as string
//char* hexadec is string in hexadecimal form
double hexadecToDecD(char* hexadec, int isDouble, int hasDot, int hasHaxE)
{
    int dec = 0, i = 0, count = 0, expSign = 1, expNum = 0, quiet;
	double decDouble = 0, mantissa = 0;
	char* temp = hexadec;
	if(isDouble == 0) {					//number is integer
		while(*temp != '\0'){			//get position of end of string
			;
			quiet = *temp++;
		}
		quiet = quiet;
		quiet = *temp--;
	    while(temp != hexadec) {
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			dec += i * ipow(16,count);
	        ++count;
			temp--;
	    }
        if(*temp == 'A') {
            i = 10;
        }else if(*temp == 'B'){
            i = 11;
        }else if(*temp == 'C'){
            i = 12;
        }else if(*temp == 'D'){
            i = 13;
        }else if(*temp == 'E'){
            i = 14;
        }else if(*temp == 'F'){
            i = 15;
        }else {
            i = (*temp) - '0';
        }
        dec += i * ipow(16,count);
        ++count;
        temp--;
        decDouble = (double)dec;
	}else if(isDouble == 1 && hasDot == 1 && hasHaxE != 1) {
		//This hexadecimal number is invalid because it has . but no exponent
		memfreeall();
		fprintf(stderr, "Invalid sequence in readDoble().\n");
		exit(7);
	}else if(isDouble == 1 && hasDot == 1 && hasHaxE == 1) {
		//hexadecimal number with . and exponent how it should be
		while(*temp != '.'){		//get the position of .
			;
			quiet = *temp++;
		}
		char* dot = temp;
		quiet = *temp--;
		while(temp != hexadec) {	//parse integer part
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			decDouble += i * ipow(16,count);
			++count;
			temp--;
		}
        if(*temp == 'A') {
            i = 10;
        }else if(*temp == 'B'){
            i = 11;
        }else if(*temp == 'C'){
            i = 12;
        }else if(*temp == 'D'){
            i = 13;
        }else if(*temp == 'E'){
            i = 14;
        }else if(*temp == 'F'){
            i = 15;
        }else {
            i = (*temp) - '0';
        }
        decDouble += i * ipow(16,count);
        ++count;
        temp--;
		count = 0; 		//reset counter
        printf("%c charuj", *dot);
		dot++; 			//skip the dot character
		while(*dot != 'p' && *dot != 'P'){ 	//parse mantissa
            printf("%c charuj", *dot);
			if(*dot == 'A') {
				i = 10;
			}else if(*dot == 'B'){
				i = 11;
			}else if(*dot == 'C'){
				i = 12;
			}else if(*dot == 'D'){
				i = 13;
			}else if(*dot == 'E'){
				i = 14;
			}else if(*dot == 'F'){
				i = 15;
			}else {
				i = (*dot) - '0';
			}

            ++count;
            mantissa += i * (1.0/(double)ipow(16,count));		//calculation
            printf("%lf\n", mantissa);
			dot++;
		}
        //printf("%s\n", "dot kekel");

		dot++;				//now dot points to p or P so go ahead and find sign
		if(*dot == '+') {
			expSign = 1;
            dot++;
		}else if(*dot == '-') {
			expSign = -1;
            dot++;
		}else {

        }

		while(*dot != '\0') {					//read rest, decimal digits
	        expNum += (*dot) - '0';
			dot++;
			if(*dot != '\0') {
				expNum *= 10;
			}
	    }

		decDouble = decDouble + mantissa;		//link it together
		if(expSign == 1) {						//and apply exponent
			decDouble *= pow(2, expNum);
		}else if(expSign == -1) {
			decDouble *= pow(2, -expNum);
		}

	}else if(isDouble == 1 && hasDot != 1 && hasHaxE == 1) {
		//hexadecimal number without . but with exponent
        //printf("KEKE\n");
		while(*temp != 'p' && *temp != 'P'){	//get the position of p or P
			;
			quiet = *temp++;
		}
		char* dot = temp;  						//dot is not currently dot..
		quiet = *temp--;
		while(temp != hexadec) {				//parse the integer part of number
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			dec += i * ipow(16,count);
			++count;
			temp--;
		}
        if(*temp == 'A') {
            i = 10;
        }else if(*temp == 'B'){
            i = 11;
        }else if(*temp == 'C'){
            i = 12;
        }else if(*temp == 'D'){
            i = 13;
        }else if(*temp == 'E'){
            i = 14;
        }else if(*temp == 'F'){
            i = 15;
        }else {
            i = (*temp) - '0';
        }
        dec += i * ipow(16,count);
        ++count;
        temp--;

		count = 0; 					//reset counter
		//printf(" dot je [%c]",*dot);
		dot++; 						//skip the p or P
		//printf(" a pak  [%c]",*dot);
		if(*dot == '+') {			//get sign
			expSign = 1;
			dot++;
		}else if(*dot == '-') {
			expSign = -1;
			dot++;
		}else {
			expSign = 1;
		}

		//printf(" nakonec je [%c]",*dot);
		while(*dot != '\0') {		//read the exponent
	        expNum += (*dot) - '0';
			dot++;
			if(*dot != '\0') {
				expNum *= 10;
			}
	    }
		//printf(" expnum je %d ", expNum);

		decDouble = (double)dec;
		if(expSign == 1) {			//pretty the same as above..
			decDouble *= pow(2, expNum);
		}else if(expSign == -1) {
			decDouble *= pow(2, -expNum);
		}
		//printf(" a dec double je {%lf} ",decDouble);
	}

	return decDouble;				//return parsed hexadecimal
}

char* replaceOctals(char *original) {
    int buffSize = lengthOld(original);
	char *buff = (char*)memalloc(buffSize * sizeof(char)+1);      //allocate memory for string

    int pos = 0;
    char preC = '~';                                            //previous char
    char makeUseOf;
    char* backup = original;
    while(*original) {
        if(preC == '\\') {                                      //If there is a chance of escape sequence
            if(isdigit(*original)){              //first check for octal numbers
                char num[] = "000";
                num[0] = *original;
                makeUseOf = *(original)++;
                makeUseOf = makeUseOf;
                if(isdigit(*original)){          //still finding if octal number is correct
                    num[1] = *original;
                    makeUseOf = *(original)++;
                    makeUseOf = makeUseOf;
                    if(isdigit(*original)){
                        num[2] = *original;
                        char oct = octToDec(atoi(num));         //conver octal to dec so we cen print it as char
                        //printf("%c",oct);
                        buff[pos] = oct;
                        pos++;
                        fflush(stdout);
                    }else {
                        //TODO: error maybe?
                        buff[pos] = *(original);
                        pos++;
                    }
                }else {
                    //TODO: error maybe?
                    buff[pos] = *original;
                    pos++;
                }

            }else {
                buff[pos] = '\\';
                pos++;
                buff[pos] = *(original);
                pos++;
            }
        }else {
            if(*original != '\\') {
                buff[pos] = *(original);
                pos++;

            }
        }
        if(preC == '\\'){ preC = '~';}                          //this will handle errors with strings like \\t\\ etc
        else {preC = *(original);}

        makeUseOf = *(original)++;
        makeUseOf = makeUseOf;
    }
    buff[pos] = '\0';
    original = backup;
    return buff;
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
    buff = replaceOctals(buff);
    return buff;
}

int readInt() {                                             //Will convert string into integer
    int buffSize = LOCAL_BUFF_SIZE, position = 0;
	char *buff = (char*)memalloc(buffSize * sizeof(char));      //allocate memory for string

    int num = 0, sign = 1, binInt = 0, octInt = 0, hexadecInt = 0;                                  //sign is 1 by default, it means +
    char* stringNum = readString();                         //make use of readString();
    char makeUseOf;                                         //get rid of warnings
    char* temp = stringNum;

    if(*temp == '0') {		//if the number starts with 0, it can be binary, octal or hexadecimal
        temp++;
        if(*temp == 'b') {						//read the b but don't store it
            temp++;
            if(*temp == '0' || *temp == '1') {
                buff[position] = *temp;
                position++;
                if(position+2 == buffSize) {
                    buffSize += BUFFER_SIZE;
                    buff = memrealloc(buff, buffSize);
                }
                temp++;
                while(*temp == '0' || *temp == '1' || *temp == '_') {
                    binInt = 1;
                    if(*temp == '_') {
                        temp++;
                        if(*temp == '_') {
                            fprintf(stderr,"Invalid sequence in readInt() function.\n");
                            memfreeall();
                            exit(7);
                        }
                        temp--;
                    }else {
                        buff[position] = *temp;
                        position++;
                        if(position+2 == buffSize) {
                            buffSize += BUFFER_SIZE;
                            buff = memrealloc(buff, buffSize);
                        }
                    }
                    temp++;
                }
                if(*temp != '\0') {
                    fprintf(stderr,"Invalid sequence in readInt() function.\n");
                    memfreeall();
                    exit(7);
                }
            }else { 		//there was no 0 or 1 so unget it and unget b
                fprintf(stderr,"Invalid sequence in readInt() function.\n");
                memfreeall();
                exit(7);
            }
        }else if(*temp == 'x') {	//it seem;s like hexadecimal number
            //check for allowed characters
            temp++;
            if((*temp >= '0' && *temp <= '9') || *temp == '_' || (*temp >= 'A' && *temp <= 'F')) {
                hexadecInt = 1;
                buff[position] = *temp;
                position++;
                if(position+2 == buffSize) {
                    buffSize += BUFFER_SIZE;
                    buff = memrealloc(buff, buffSize);
                }
                temp++;
                while((*temp >= '0' && *temp <= '9') || *temp == '_' || (*temp >= 'A' && *temp <= 'F')) {

                    if(*temp == '_') {
                        temp++;
                        if(*temp == '_') {
                            fprintf(stderr,"Invalid sequence in readInt() function.\n");
                            memfreeall();
                            exit(7);
                        }
                        temp--;
                    }else {
                        buff[position] = *temp;
                        position++;
                        if(position+2 == buffSize) {
                            buffSize += BUFFER_SIZE;
                            buff = memrealloc(buff, buffSize);
                        }
                    }
                    temp++;
                }
                if(*temp != '\0') {
                    fprintf(stderr,"Invalid sequence in readInt() function.\n");
                    memfreeall();
                    exit(7);
                }
            }else {
                fprintf(stderr,"Invalid sequence in readInt() function.\n");
                memfreeall();
                exit(7);
            }
        }else if(*temp >= '0' && *temp <= '7') {	//if after 0 comes a number in valid range it is octal
            buff[position] = *temp;
            position++;
            if(position+2 == buffSize) {
                buffSize += BUFFER_SIZE;
                buff = memrealloc(buff, buffSize);
            }
            temp++;
            //read all of the numbers
            while((*temp >= '0' && *temp <= '7') || *temp == '_') {
                octInt = 1;				//for parsing purposes
                if(*temp == '_') {
                    temp++;
                    if(*temp == '_') {
                        fprintf(stderr,"Invalid sequence in readInt() function.\n");
                        memfreeall();
                        exit(7);
                    }
                    temp--;
                }else {
                    buff[position] = *temp;
                    position++;
                    if(position+2 == buffSize) {
                        buffSize += BUFFER_SIZE;
                        buff = memrealloc(buff, buffSize);
                    }
                }
                temp++;
            }
            if(*temp != '\0') {
                fprintf(stderr,"Invalid sequence in readInt() function.\n");
                memfreeall();
                exit(7);
            }
        }
        buff[position] = '\0';
        printf("{%s}",buff);
        if(binInt == 1) {
            return binToDec(atoi(buff));
        }else if(octInt == 1) {
            return octToDec(atoi(buff));
        }else if(hexadecInt == 1) {
            return hexadecToDecI(buff);
        }
        return 1;
    }else {
        do{
            if(isdigit(*stringNum)) {                                         //if it's digit
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
    }
    return num*sign;
}

double readDouble() {                                                            //Will convert string into double
    int buffSize = LOCAL_BUFF_SIZE, position = 0;
    char *buff = (char*)memalloc(buffSize * sizeof(char));      //allocate memory for string
    int isDouble = 0;
    double mantissa = 0;
    char makeUseOf;
    long long longNum = 0;
    int mantCount = 10;                                                            //aka get rid of warnings var
    int exponent = 0,sign = 1, eSign = 1, hasDot = 0, hasE = 0, hasHaxE = 0;      //few bool-ints to handle . e +
    char* stringNum = readString();
    char* temp = stringNum;                                          //make use of readString();
    if(*temp == '0') {
        temp++;
        if(*temp == 'x') {	//it seem;s like hexadecimal number
            //check for allowed characters
            temp++;
            if((*temp >= '0' && *temp <= '9') || *temp == '_' || (*temp >= 'A' && *temp <= 'F')) {
                buff[position] = *temp;
                position++;
                if(position+2 == buffSize) {
                    buffSize += BUFFER_SIZE;
                    buff = memrealloc(buff, buffSize);
                }
                temp++;
                while((*temp >= '0' && *temp <= '9') || *temp == '_' || (*temp >= 'A' && *temp <= 'F')) {

                    if(*temp == '_') {
                        temp++;
                        if(*temp == '_') {
                            fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                            memfreeall();
                            exit(7);
                        }
                        temp--;
                    }else {
                        buff[position] = *temp;
                        position++;
                        if(position+2 == buffSize) {
                            buffSize += BUFFER_SIZE;
                            buff = memrealloc(buff, buffSize);
                        }
                    }
                    temp++;
                }

                //if there is a decimal part of number
                if(*temp == '.') {
                    hasDot = 1;				//for parsing purposes
                    temp++;
                    if((*temp >= '0' && *temp <= '9') || (*temp >= 'A' && *temp <= 'F') || *temp == '_'){
                        buff[position] = '.';
                        position++;
                        if(position+2 == buffSize) {
                            buffSize += BUFFER_SIZE;
                            buff = memrealloc(buff, buffSize);
                        }
                        //temp++;
                        if(*temp == '_') {
                            temp++;
                            if(*temp == '_') {
                                fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                memfreeall();
                                exit(7);
                            }
                            temp--;
                        }else {
                            buff[position] = *temp;
                            position++;
                            if(position+2 == buffSize) {
                                buffSize += BUFFER_SIZE;
                                buff = memrealloc(buff, buffSize);
                            }
                        }
                        isDouble = 1;

                        //rest
                        temp++;
                        while((*temp >= '0' && *temp <= '9') || *temp == '_' || (*temp >= 'A' && *temp <= 'F')) {
                            if(*temp == '_') {
                                temp++;
                                if(*temp == '_') {
                                    fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                    memfreeall();
                                    exit(7);
                                }
                                temp--;
                            }else {
                                buff[position] = *temp;
                                position++;
                                if(position+2 == buffSize) {
                                    buffSize += BUFFER_SIZE;
                                    buff = memrealloc(buff, buffSize);
                                }
                            }
                            temp++;
                        }

                    //    ******************************************************************************************
                        //there has to be exponent after . part
                        if(*temp == 'p' || *temp == 'P') {
                            //*temp = c; //p
                            temp++;
                            if(*temp == '+' || *temp == '-' || (*temp >= '0' && *temp <= '9')) { //0xFF.FFp-
                                buff[position] = 'p';
                                position++;
                                if(position+2 == buffSize) {
                                    buffSize += BUFFER_SIZE;
                                    buff = memrealloc(buff, buffSize);
                                }
                                buff[position] = *temp;
                                position++;
                                if(position+2 == buffSize) {
                                    buffSize += BUFFER_SIZE;
                                    buff = memrealloc(buff, buffSize);
                                }
                                hasHaxE = 1;
                                isDouble = 1;
                                temp++;
                                while((*temp >= '0' && *temp <= '9') || *temp == '_') {
                                    if(*temp == '_' && isdigit(buff[position-1])) {
                                        temp++;
                                        if(*temp == '_') {
                                            fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                            memfreeall();
                                            exit(7);
                                        }
                                        temp--;
                                    }else if(*temp == '_' && !isdigit(buff[position-1])){
                                        fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                        memfreeall();
                                        exit(7);
                                    }else {
                                        buff[position] = *temp;
                                        position++;
                                        if(position+2 == buffSize) {
                                            buffSize += BUFFER_SIZE;
                                            buff = memrealloc(buff, buffSize);
                                        }
                                    }
                                    temp++;
                                }
        //all theese ungets are here to get back into state before reading
                            }else {
                                fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                memfreeall();
                                exit(7);
                            }
                        }else {
                            fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                            memfreeall();
                            exit(7);
                        }
                    }else {
                        fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                        memfreeall();
                        exit(7);
                    }
                //if there is no . but only exponent part
            }else if(*temp == 'p' || *temp == 'P'){
                    if(*temp == 'p' || *temp == 'P') {		//casual double check
                        //tempestc = *temp; //P
                        printf(")XFFAD\n");
                        temp++;
                        if(*temp == '+' || *temp == '-' || (*temp >= '0' && *temp <= '9')) { //0xFF.FFp-
                            buff[position] = 'p';
                            position++;
                            if(position+2 == buffSize) {
                                buffSize += BUFFER_SIZE;
                                buff = memrealloc(buff, buffSize);
                            }
                            buff[position] = *temp;
                            position++;
                            if(position+2 == buffSize) {
                                buffSize += BUFFER_SIZE;
                                buff = memrealloc(buff, buffSize);
                            }
                            hasHaxE = 1;
                            isDouble = 1;
                            temp++;
                            while((*temp >= '0' && *temp <= '9') || *temp == '_') {
                                if(*temp == '_' && isdigit(buff[position-1])) {
                                    temp++;
                                    if(*temp == '_') {
                                        fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                        memfreeall();
                                        exit(7);
                                    }
                                    temp--;
                                }else if(*temp == '_' && !isdigit(buff[position-1])){
                                    fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                                    memfreeall();
                                    exit(7);
                                }else {
                                    isDouble = 1;
                                    buff[position] = *temp;
                                    position++;
                                    if(position+2 == buffSize) {
                                        buffSize += BUFFER_SIZE;
                                        buff = memrealloc(buff, buffSize);
                                    }
                                }
                                temp++;
                            }
                        }else {
                            fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                            memfreeall();
                            exit(7);
                        }
                    }else {
                        fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                        memfreeall();
                        exit(7);
                    }
                }else {		//next char is not p or P
    //                fprintf(stderr,"Invalid sequence in hreadDouble() function.\n");
    //                memfreeall();
    //                exit(7);
                }
            }else {
                fprintf(stderr,"Invalid sequence in readDouble() function.\n");
                memfreeall();
                exit(7);
            }
            buff[position] = '\0';
            printf("{%s}",buff);
            return hexadecToDecD(buff, isDouble, hasDot, hasHaxE);
        }else {
            temp--;
        }
    }

        do{
            if(((*stringNum == '-') || (*stringNum == '+')) && hasE == 1 ) {         //read sign of E, it is + by default
                if(*stringNum == '-') eSign *= -1;
            }/*else if(((*stringNum == '-') || (*stringNum == '+')) && num == 0 ) {    //read sign of number
                if(*stringNum == '-') sign *= -1;
            }*/else if(isdigit(*stringNum) && hasE == 0 && hasDot == 0) {              //read the integer part of number
            longNum *= 10;
            longNum += *stringNum - '0';
        }else if(isdigit(*stringNum) && hasE == 0 && hasDot == 1) {              //read part of number past .
            mantissa += *stringNum - '0';
            mantissa *= 10;
            mantCount *= 10;
            //if(*stringNum == '0' &&  mantissa ==  0.0) mantCount *= 10;
        }else if(isdigit(*stringNum) && hasE == 1) {                             //read part of number past e
            exponent *= 10;
            exponent += *stringNum - '0';
        }else if(*stringNum == 'e' || *stringNum == 'E') {                       //check if number has exponent
            hasE = 1;
        }else if(*stringNum == '.'){                                             //check if number has mantissa
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

    return ((longNum + (mantissa / mantCount))*fullExp)*sign;      //return number with mantissa but multiply it with calculated exponent (if any) and don't forget the sign
}

//value is the actual string or var name, option may be 0-> it is actual string || 1-> it is just name of variable
void print(char* value, int opt, thTable *htab, char* class) {

    thtabItem* tempItem;
    if(opt == 1){                                                           //value is variable
        tempItem = htabSearch(htab, value);                                 //find variable in local table
        if(tempItem == NULL) {
            if(!strstr(value,".")){		//if called function is short identifier
                strcpy(value,concat(value,class));	//the concat it with class name
            }
            if((tempItem=(htabSearch(resources->globalVarTable,value))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        if(tempItem != NULL){
            if(tempItem->varType == 28 ){                                   //choose the correct way to print variable
                printf("%d",tempItem->intValue);                            //int
                fflush(stdout);
            }else if(tempItem->varType == 23){
                printf("%g",tempItem->doubleValue);                         //double
                fflush(stdout);
            }else if(tempItem->varType == 30 ) {                            //string
                char preC = '~';                                            //previous char
                char makeUseOf;
                char* backup = (tempItem->stringValue);
                while(*(tempItem->stringValue)) {
                    if(preC == '\\') {                                      //If there is a chance of escape sequence
                        if(isdigit(*(tempItem->stringValue)) && *(tempItem->stringValue) <= '3'){              //first check for octal numbers
                            char num[] = "000";
                            num[0] = *(tempItem->stringValue);
                            makeUseOf = *(tempItem->stringValue)++;
                            makeUseOf = makeUseOf;
                            if(isdigit(*(tempItem->stringValue)) && *(tempItem->stringValue) <= '7'){          //still finding if octal number is correct
                                num[1] = *(tempItem->stringValue);
                                makeUseOf = *(tempItem->stringValue)++;
                                makeUseOf = makeUseOf;
                                if(isdigit(*(tempItem->stringValue)) && *(tempItem->stringValue) <= '7'){
                                    num[2] = *(tempItem->stringValue);
                                    char oct = octToDec(atoi(num));         //conver octal to dec so we cen print it as char
                                    printf("%c",oct);
                                    fflush(stdout);
                                }
                            }
                        }else if(*(tempItem->stringValue) == 'n') {         //there is \n , print new line
                            printf("\n");
                            fflush(stdout);
                        }else if(*(tempItem->stringValue) == 't'){          //there is \t , print tab space
                            printf("\t");
                            fflush(stdout);
                        }else if(*(tempItem->stringValue) == '\"'){         //there is \" , print ""
                            printf("\"");
                            fflush(stdout);
                        }else if(*(tempItem->stringValue) == '\\'){         //there is \\ , print "\"
                            printf("\\");
                            fflush(stdout);
                        }else {
                            memfreeall();
        					fprintf(stderr, "String is incorrect.\n");
        					exit(1);
                        }
                    }else if(*tempItem->stringValue == '\\'){
                        ;
                    }else {
                        printf("%c",*(tempItem->stringValue));
                        fflush(stdout);
                    }

                    if(preC == '\\'){ preC = '~';}                          //this will handle errors with strings like \\t\\ etc
                    else {preC = *(tempItem->stringValue);}

                    makeUseOf = *(tempItem->stringValue)++;
                    makeUseOf = makeUseOf;
                }
                (tempItem->stringValue) = backup;                           //I've been moving pointer so here is time to set it back
            }
        }
    }else { //value is just string, check commentary above
        char preC = '!';
        char makeUseOf;
        char* backup = value;
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
                            fflush(stdout);
                        }
                    }
                }else if((*value) == 'n') {
                    printf("\n");
                    fflush(stdout);
                }else if((*value) == 't'){
                    printf("\t");
                    fflush(stdout);
                }else if((*value) == '\"'){
                    printf("\"");
                    fflush(stdout);
                }else if((*value) == '\\'){
                    printf("\\");
                    fflush(stdout);
                }else {
                    memfreeall();
                    fprintf(stderr, "String is incorrect.\n");
                    exit(1);
                }
            }else if((*value) == '\\'){
                ;
            }else {
                printf("%c",*value);
                fflush(stdout);
            }

            if(preC == '\\'){ preC = '~';}
            else {preC = *value;}
            makeUseOf = *(value)++;
            makeUseOf = makeUseOf;
        }
        value = backup;
    }
}

//String s is key, int stringOpt works as in print function (0/1)
int length(String s, int stringOpt, thTable *htab, char* class){
    if(stringOpt) {                         //please look at print() commentary
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);     //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		        //if called function is short identifier
                strcpy(s,concat(s,class));	//the concat it with class name
            }
            if((tempItem=(htabSearch(resources->globalVarTable,s))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        s = tempItem->stringValue;
    }
    s = replaceOctals(s);
    char *old = s;
    int offset = 0;
    char preC = '!';
    char makeUseOf;
    while(*s) {
        if(preC == '\\') {
            if(isdigit((*s)) && *s <= '3'){
                makeUseOf = *(s)++;
                makeUseOf = makeUseOf;
                if(isdigit(*s) && *s <= '7'){
                    makeUseOf = *s++;
                    makeUseOf = makeUseOf;
                    if(isdigit(*s) && *s <= '7'){
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
    s = old;
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
                strcpy(s,concat(s,class));	//the concat it with class name
            }
            if((tempItem=(htabSearch(resources->globalVarTable,s))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        s = tempItem->stringValue;
    }
    if(iOpt) {
        thtabItem* tempItem2;
        tempItem2 = htabSearch(htab, iNum);         //find local var
        if(tempItem2 == NULL) {
            if(!strstr(iNum,".")){		            //if called function is short identifier
                strcpy(iNum,concat(iNum,class));	//the concat it with class name
            }
            if((tempItem2=(htabSearch(resources->globalVarTable,iNum))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem2->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        i = tempItem2->intValue;
    }
    if(nOpt) {
        thtabItem* tempItem3;
        tempItem3 = htabSearch(htab, nNum);         //find local var
        if(tempItem3 == NULL) {
            if(!strstr(nNum,".")){		//if called function is short identifier
                strcpy(nNum,concat(nNum,class));	//the concat it with class name
            }
            if((tempItem3=(htabSearch(resources->globalVarTable,nNum))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem3->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        n = tempItem3->intValue;
    }

    s = replaceOctals(s);

    if(lengthOld(s) < i+n || i < 0 || n < 0) {
        fprintf(stderr, "Substring error, invalid values\n");
        memfreeall();
        exit(10);
    }
    char *buff = (char*)memalloc((n+1) * sizeof(char));
    for(int x = 0; x < n; x++) {
        buff[x] = s[i+x];
    }
    buff[n] = '\0';
    return buff;
}

//please for details check functions above
int compare(String s1, int s1Opt, String s2, int s2Opt, thTable *htab, char* class) {
    if(s1Opt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s1);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s1,".")){		//if called function is short identifier
                strcpy(s1,concat(s1,class));	//the concat it with class name
            }
            if((tempItem=(htabSearch(resources->globalVarTable,s1))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        s1 = tempItem->stringValue;
    }
    if(s2Opt) {         //it is variable
        thtabItem* tempItem2;
        tempItem2 = htabSearch(htab, s2);         //find local var
        if(tempItem2 == NULL) {
            if(!strstr(s2,".")){		//if called function is short identifier
                strcpy(s2,concat(s2,class));	//the concat it with class name
            }
            if((tempItem2=(htabSearch(resources->globalVarTable,s2))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem2->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        s2 = tempItem2->stringValue;         //set s2 as value of variable
    }

    s1 = replaceOctals(s1);
    s2 = replaceOctals(s2);
    char* olds1 = s1;
    char* olds2 = s2;
    while(*olds1 && (*olds1 == *olds2))              //while first string has chars, and char of 1st and 2nd string match, pass it
    {
        olds1++;
        olds2++;
    }
    if((*olds1 - *olds2) < 0) {                   //if ord. value of s2 char is bigger than ord. value of s1 char, return -1

        return(-1);
    }
    else if((*olds1 - *olds2) > 0) {              //else if ord. value of s2 char is lesser than ord. value of s1 char, return 1

        return(1);
    }
    else {                                  //else strings do match so return 0 if they are equal/same

        return(0);
    }
}

//please for details check functions above
int find(String s, int stringOpt, String search, int searchOpt, thTable *htab, char* class) {           //Function finds substring in string, returns position of substring in string if found, or -1 if not found
    if(stringOpt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		//if called function is short identifier
                strcpy(s,concat(s,class));	//the concat it with class name
            }
            if((tempItem=(htabSearch(resources->globalVarTable,s))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        s = tempItem->stringValue;
    }
    if(searchOpt) {
        thtabItem* tempItem2;
        tempItem2 = htabSearch(htab, search);         //find local var
        if(tempItem2 == NULL) {
            if(!strstr(search,".")){		//if called function is short identifier
                strcpy(search,concat(search,class));	//the concat it with class name
            }
            if((tempItem2=(htabSearch(resources->globalVarTable,search))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem2->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        search = tempItem2->stringValue;
    }

    s = replaceOctals(s);
    search = replaceOctals(search);
	int P_len = strlen(search);            //Length of substring
    int T_len = strlen(s);          //Length of string

    int CharJump[256];          //Array of integers of size "MAX CHAR" in which the highest possible jump values of first BMA heuristics will be stored
    int MatchJump[P_len];           //Array of integers of size "length of substring given to be found" in which the highest possible jump values of second BMA heuristics will be stored

    int res = bma(search, s, CharJump, MatchJump);          //Finds position of substring in string using BMA function

    if(res < T_len) {           //If substring is found, return its position, else return -1
        return(res);
    }
    else {
        return(-1);
    }
}

//please for details check functions above
String sort(String s, int stringOpt, thTable *htab, char* class) {          //Function sorts string given
    if(stringOpt) {
        thtabItem* tempItem;
        tempItem = htabSearch(htab, s);         //find local var
        if(tempItem == NULL) {
            if(!strstr(s,".")){		//if called function is short identifier
                strcpy(s,concat(s,class));	//the concat it with class name
            }
            if((tempItem=(htabSearch(resources->globalVarTable,s))) == NULL){	//if not in local, search global
                fprintf(stderr,"Sem_Error. Expression based on nonexistant variable\n");
                memfreeall();
                exit(3);
            }
        }if (tempItem->isInit == 0) {
            fprintf(stderr, "Error in print function, variable uninitialized.\n");
            memfreeall();
            exit(8);
        }
        s = tempItem->stringValue;
    }
    s = replaceOctals(s);
    int s_len = strlen(s);          //Length of string to sort
    char *str = (char*)memalloc((s_len+1) * sizeof(char));          //Allocating help string, which will be sorted
    for(int x = 0; x < s_len; x++) {            //Initializing help string
        str[x] = s[x];
    }
    str[s_len] = '\0';

    int left = 0;           //Index of beginning of string
    int right = s_len - 1;            //Index of end of string
    quick_sort(str, left, right);           //Sorting string using quick_sort function

    return(str);
}
