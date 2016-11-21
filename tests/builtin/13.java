class Main {
	static void run() {
		ifj16.print("Builtin 13.java\n");
		String a; String aSort;

		ifj16.print("Zadej string:\n");
		a = ifj16.readString();

		aSort = ifj16.sort(a);
		ifj16.print("Unsorted a: " + a + "\n");
		ifj16.print("Sorted a: " + aSort + "\n");
	}
}
