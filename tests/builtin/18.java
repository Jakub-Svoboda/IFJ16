//Guess the number in 10 tries

class Main {
	static void run() {
		int guess; int number; int counter; int counter2;
		counter = 0; counter2 = counter;
		//ifj16.print("Enter a number to guess first but don't show it to anyone. I know it might be hard in this case.\n")
		number = 7;

		guess = readInt();
		while(counter < 10) {
			if(guess == number) {
				ifj16.print("You found out that the number is lucky 7.\n")
			}else {
				print("Try again!\n");
				guess = readInt();
			}
			counter = counter2 + 1; //not sure if x = x+1 is implemented
			counter2 = counter;
		}
	}
}
