class Main {
	static void run() {
		ifj16.print("Builtin 4.java\n");
		ifj16.print("Zadej string:\n");
		String a;
		a = ifj16.readString();
        String b;
		b = "Vstup string je ";
        ifj16.print(a+"\n");
        ifj16.print("Vstup string je " + a+"\n");
        ifj16.print(b + a+"\n");
	}
}
