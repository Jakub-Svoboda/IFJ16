class Main {
	static void run() {
		String a;
		int i;
		double d;
		int l;
		ifj16.print("Type string: ");
		a = ifj16.readString();
		ifj16.print("Type int: ");
		i = ifj16.readInt();
		ifj16.print("Type double: ");
		d = ifj16.readDouble();

		//l = ifj16.length(a);
		//ifj16.print("\n"+l+"\n");
		ifj16.print(a + i + d + "\n");

		//l = ifj16.length(a);
		//ifj16.print("\n"+l+"\n");
		ifj16.print(i + d + a + "\n");

		//l = ifj16.length(a);
		//ifj16.print("\n"+l+"\n");
		ifj16.print(d + a + i + "\n");

		//l = ifj16.length(a);
		//ifj16.print("\n"+l+"\n");
	}
}
