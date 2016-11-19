class Main {
	static void run() {
		String a;
		a = "strednicek";
		String substr1 = "a";
		String substr2 = " a";
		String substr3 = "";

		substr1 = ifj16.substr(a,0,4);	//should be "stře"
		ifj16.print("substr: " + substr1 + ".\n");
		substr2 = ifj16.substr(a,4,4);	//should be "dníč"
		ifj16.print("substr: " + substr2 + ".\n");
		substr3 = ifj16.substr(a,8,2);	//should be "ek"
		ifj16.print("substr: " + substr3 + ".\n");

	}
}
