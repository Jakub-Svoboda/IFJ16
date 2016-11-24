class Main {
	static void run() {
		ifj16.print("Builtin 14.java\n");
		String a;
		int i;
		double d;
		ifj16.print("Zadej string:\n");
		a = ifj16.readString();
		ifj16.print("Zadej int:\n");
		i = ifj16.readInt();
		ifj16.print("Zadej double:\n");
		d = ifj16.readDouble();
		ifj16.print("StrIntDou\n");
		ifj16.print(a + i + d + "\n");
		ifj16.print("IntDouStr\n");
		ifj16.print(i + d + a + "\n");
		ifj16.print("DouStrInt\n");
		ifj16.print(d + a + i + "\n");
	}
}
