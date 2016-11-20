class Main {
	static void run() {
		String a; String b; String c;
		String aS; String bS; String cS;
		String aSort; String bSort; String cSort;
		int adiff; int bdiff; int cdiff;
		a = "hesoyam";	aS = "aehmosy";
		b = "aezakmi";	bS = "aaeikmz";
		c = "wanrltv";	cS = "alnrtvw";

		aSort = ifj16.sort(a);
		bSort = ifj16.sort(b);
		cSort = ifj16.sort(c);

		adiff = ifj16.compare(aSort, aS);
		bdiff = ifj16.compare(bSort, bS);
		cdiff = ifj16.compare(cSort, cS);

		ifj16.print("aSort: " + aSort + " bSort: " + bSort + " cSort: " + cSort + "\n");
		ifj16.print("adiff: " + adiff + " bdiff: " + bdiff + " cdiff: " + cdiff + "\n");
	}
}
