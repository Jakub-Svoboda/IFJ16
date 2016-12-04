class Main
{
	static void run()
	{
		int a = 10;
		int vysl;

		vysl = 1;
		while (a > 0) {
			vysl = vysl * a;
			a = a - 1;
		}
		ifj16.print(vysl+"\n");
	}
}