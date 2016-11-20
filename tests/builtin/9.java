class Main {
	static void run() {
		ifj16.print("Builtin 9.java\n");
		String a; String b;
		a = "středníček";
		b = "klopetek";

		int num0; int num1; int num2; int num3; int num4;

		num0 = ifj16.compare(b, a);
		num1 = ifj16.compare(a, b);
		num2 = ifj16.compare(a, a);
		num3 = ifj16.compare("klopetek", b);
		num4 = ifj16.compare("klopetek", "klopetek");

		ifj16.print(" a je " + a + " b je " + b + "\n");
		ifj16.print("compare(b,a) " + num0 + "| (a,b) " + num1 + "| (a,a) " + num2 + "| (\"klopetek\",b) " + num3 +"| (\"klopetek\",\"klopetek\") " + num4 + "\n");

	}
}
