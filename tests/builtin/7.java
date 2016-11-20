class Main {
	static void run() {
		ifj16.print("Builtin 7.java\n");

		String a;
		a = "strednicekaa";
		String substr1;// = "";
		String substr2;// = "";
		String substr3;// = " ";

		substr1 = ifj16.substr(a,0,4);	//should be "stře"
		ifj16.print("substr(a,0,4) a = \"strednicekaa\"; " + substr1 + ".\n");
		substr2 = ifj16.substr(a,4,4);	//should be "dníč"
		ifj16.print("substr(a,4,4) a = \"strednicekaa\"; " + substr2 + ".\n");
		substr3 = ifj16.substr(a,8,2);	//should be "ek"
		ifj16.print("substr(a,8,2) a = \"strednicekaa\"; " + substr3 + ".\n");
		//ifj16.print("[" + substr1+ "]"+ "\n");

	}
}
