class Main {
	static void run() {
		String a;
		a = "strednicek";
		String substr1;
		String substr2;
		String substr3;

		int num1; int num2; int num3;
		num1 = 0;
		num2 = 4;
		num3 = 2;
		substr1 = ifj16.substr(a, num1, num2);	//should be "stře"
		ifj16.print("substr: " + substr1 + ".\n");
		substr2 = ifj16.substr(a, num2, num2);	//should be "dníč"
		ifj16.print("substr: " + substr2 + ".\n");
		substr3 = ifj16.substr(a, 8, num3);	//should be "ek"
		ifj16.print("substr: " + substr3 + ".\n");
	}
}
