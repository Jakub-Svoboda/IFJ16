class Main
{
	static void run()
	{
		int a = 10;
		int vysl;
		int neg;
		vysl = factorial(a);
		neg = 0 - vysl;
	}

	static int factorial(int n)
	{
		int temp_result;
		int decremented_n = n - 1;
		if (n < 2) {
			return 1;
		}
		else {
			temp_result = factorial(decremented_n);
			temp_result = n * temp_result;
			return temp_result;
		}
	}
}