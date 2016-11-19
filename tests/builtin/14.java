class Main {
	static void run() {
		String a;
		int i;
		double d;
		a = readString();
		i = readInt();
		d = readDouble();
		printf(a + i + d + "\n");
		printf(i + d + a + "\n");
		printf(d + a + i + "\n");
	}
}
