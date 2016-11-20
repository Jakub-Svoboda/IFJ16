class Main {
	static void run() {
		String a;
		a = "strednicekaa";
		String substr1;// = "";
		String substr2;// = "";
		String substr3;// = " ";

		substr1 = ifj16.substr(a,0,4);	//should be "stře"
		ifj16.print("substr: " + substr1 + ".\n");
		substr2 = ifj16.substr(a,4,4);	//should be "dníč"
		ifj16.print("substr: " + substr2 + ".\n");
		substr3 = ifj16.substr(a,8,2);	//should be "ek"
		ifj16.print("substr: " + substr3 + ".\n");
		//ifj16.print("[" + substr1+ "]"+ "\n");

	}
}
