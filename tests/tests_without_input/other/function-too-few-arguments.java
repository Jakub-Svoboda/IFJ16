class Main
{
	static void run()
	{
		int a = 5;
      int b = 6;
      a = min(a);
	}

   static int min(int a, int b)
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
