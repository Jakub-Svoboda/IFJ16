class Main {
	static void run() {
		ifj16.print("Builtin 11.java\n");
		String a; String b; String c;
		String aS; String bS; String cS;
		String aSort; String bSort; String cSort;
		a = "hesoyam";	cS = "aehmosy";
		b = "aezakmi";	bS = "aaeikmz";
		c = "wanrltv";	cS = "alnrtvw";

		aSort = ifj16.sort(a);
		bSort = ifj16.sort(b);
		cSort = ifj16.sort(c);

		ifj16.print("a: " + a + " b: " + b + " c: " + c + "\n");
		ifj16.print("aSort: " + aSort + " bSort: " + bSort + " cSort: " + cSort + "\n");
	}
}
