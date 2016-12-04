class Main {
	static void run() {
		ifj16.print("Builtin 10.java\n");
		String a; String b; String klop;
		a = "středníček";
		b = "klopetek";
		klop = "klop";

		int num0; int num1; int num2; int num3; int num4;

		num0 = ifj16.find(b, a);
		num1 = ifj16.find(b, klop);
		num2 = ifj16.find(a, a);
		num3 = ifj16.find("klopetek", b);
		num4 = ifj16.find("klopetek", klop);

		ifj16.print(" a je " + a + " b je " + b + " klop je " + klop + "\n");
		ifj16.print("find(b,a) " + num0 + "| (b, klop) " + num1 + "| (a,a) " + num2 + "| (\"klopetek\",b) " + num3 +"| (\"klopetek\",klop) " + num4 + "\n");

	}
}
