//Guess the number

class Main {
	static void run() {
		int guess; int number; int counter; int counter2;
		counter = 1; counter2 = 1;
		ifj16.print("Enter a number to guess first but don't show it to anyone. I know it might be hard in this case.\n");
		number = ifj16.readInt();

		guess = ifj16.readInt();
		while(guess != number) {
			counter = counter2 + 1; //not sure if x = x+1 is implemented
			counter2 = counter;
			ifj16.print("Try again!\n");
			guess = ifj16.readInt();
		}
		ifj16.print("Wow, you are such a clever boy. The number was "+number+ " and you guessed it on " + counter + " try.\n");
	}
}
