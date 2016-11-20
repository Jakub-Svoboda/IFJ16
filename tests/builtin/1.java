class Main {
	static void run() {
		ifj16.print("Builtin 1.java\n");
		ifj16.print("Zadej integer:\n");
		int a = ifj16.readInt();
		ifj16.print("Zadej double:\n");
		double b = ifj16.readDouble();
		ifj16.print("Zadej string:\n");
		String c = ifj16.readString();

        ifj16.print("Vstup byl " + a + " " + b + " " + c + "\n");
		ifj16.print("Znovu byl " + a + " " + b + " " + c + "\n");
        //ifj16.print("Vstup int je: " + a);
        //ifj16.print(b + a);
	}
}
