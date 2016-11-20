class Main {
	static void run() {
		ifj16.print("Builtin 5.java\n");
		ifj16.print("Zadej string:\n");
		String a;
		a = ifj16.readString();
        String b;
		b = "Vstup string je";
		ifj16.print("Prepis String b na:\n");
		b = ifj16.readString();
        ifj16.print(b+"\n");
        ifj16.print(b + a+"\n");
	}
}
