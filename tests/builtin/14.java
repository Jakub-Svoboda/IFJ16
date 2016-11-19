class Main {
	static void run() {
		String a;
		int i;
		double d;
		a = ifj16.readString();
		i = ifj16.readInt();
		d = ifj16.readDouble();
		ifj16.print(a + i + d + "\n");
		ifj16.print(i + d + a + "\n");
		ifj16.print(d + a + i + "\n");
	}
}
