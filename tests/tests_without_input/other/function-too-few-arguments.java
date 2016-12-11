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
      int ahoj;
      if (a > b) {
         ahoj = b;
      }
      else {
         ahoj = a;
      }

      return(ahoj); 
   }
}
