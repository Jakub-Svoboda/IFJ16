class Main
{
    static void run()
    {
        badFunc2(2.2, 3.2, 4, 5); //to many parameters
        badFunc3(); //undefined function
        ahoj.badFunc2;//undefined class
        badFunc2(2.3, 3, 4.6); //bad type
        FloatTest();
        NotInic1();
        NotInic2();
        badAss1();
        badInic1();
        undef();
        badAss2();
        badArray1();
        badCond1();
        badFunc1();
        badIf();
        sameProm();

    }
    public static void FloatTest()
    {
        int a,b,c;
        a = 2;
        b = 3;
        c = b/a;
        ifj16.print("C is "+c);
    }

    public void NotInic1()
    {
        int j;
        ifj16.print(j); //semantic error
    }
    public void NotInic2()
    {
        int i;
        i++;
        ifj16.print(i);
    }

    public void badAss1()
    {
        int a = "hello";
        ifj16.print(a);
    }
    public void badInic1()
    {
        int = 1;
    }
    public void undef()
    {
        int a = 5;
        int b = 4;
        c = a * b; //undefined prom
        ifj16.print(c);
    }
    public void badAss2()
    {
        String s = "ahoj";
        int a = 6 - s;
        ifj16.print(a)
    }
    public void badArray1()
    {

        int[] v = new int [10];
        v[10] = 100;
        ifj16.print(v[10]);
    }

    public void badCond1()
    {
        int a = 1;
        int b = 2;

        if (a = b)
        {
            ifj16.print("Rovna se a a b?\n");
        }

    }
    public void badFunc1(double a)
    {
        return a;
    }

    public static double badFunc2(double a, double b, int c)
    {
        double d;

        d = c + b;
        b = d * a;
        ifj16.print(b);
    }
    public void badIf()
    {
        int a=5;
        String b = "c";
        if (a < b)
        {
            ifj16.print("good");
        }
        else
        {
            ifj16.print("bad");
        }
    }
    public void sameProm()
    {
        double a = 5;
        String a = "ahoj";
        ifj16.print(a);

    }
    public void sameProm()
    {
        ifj16.print("same function");
    }
    public void badInic2()
    {
        double e = "ahoj";
        ifj16.print(e);
    }
}