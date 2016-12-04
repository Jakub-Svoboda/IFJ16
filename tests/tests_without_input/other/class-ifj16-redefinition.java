class Main
{
	static void run()
	{
		int a = 5;
		int b = 6;
        int c = ifj16.min(a, b);
	}
}

class ifj16
{
	static int min(int a, int b) {
    	int min;
    	if (a > b) {
    		min = b;
        }
    	else {
    		min = a;
        }

      return(min); 
    }
}