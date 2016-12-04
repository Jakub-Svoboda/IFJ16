class Main
{
	static int a = 5;
	static void run()
	{
		int b = Main.a + Smth.a;
	}
}

class Smth
{
	static int a = 10;
	static void neco()
	{
		int b = Main.a + Smth.a;
	}
}