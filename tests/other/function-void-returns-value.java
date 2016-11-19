class Main
{
	static void run()
	{
		int a = 5;
		int b = 6;
      int c = min(a, b);
	}

   static void min(int a, int b)
   {
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
