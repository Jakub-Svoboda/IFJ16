class Main
{
	static void run()
	{
		String str1;
		str1 = "Toto je nejaky text";
		String str2;
		str2 = str1 + ", ktery jeste trochu obohatime";
		String str3 = Smth.func(str2);
		ifj16.print(str3+"\n");
	}
}

class Smth
{
	static String func(String str)
	{
		String str1 = "String given :";
		str = str1 + str;
		return(str);
	}
}