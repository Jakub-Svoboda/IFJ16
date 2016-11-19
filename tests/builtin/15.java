class Main {
	static void run() {
		String a;
		int i;
		double d;
		ifj16.print("Type string: ");
		a = ifj16.readString();
		ifj16.print("Type int: ");
		i = ifj16.readInt();
		ifj16.print("Type double: ");
		d = ifj16.readDouble();
		ifj16.print(a + i + d + "\n");
		ifj16.print(i + d + a + "\n");
		ifj16.print(d + a + i + "\n");
	}
}
