class pre{
	static int a = (((4+8)/3)*4)*(1*2);
	static void basnicka(){
		ifj16.print(Main.shakesbro+"\n");
		return;
	}
}



class Main{
	static void run(){
		pre.basnicka();
		ifj16.print(pre.a+"\n");		//12
		double mujDouble = 4;
		mujDouble=mulpi(mujDouble);
		ifj16.print(mujDouble+"\n");		//12
		ifj16.print(post.counter+"\n");		//12
	}
	static String shakesbro = "\tNot from the stars do I my judgement pluck,\n\tAnd yet methinks I have astronomy,\n\tBut not to tell of good, or evil luck,\n\tOf plagues, of dearths, or seasons' quality.";
	static double mulpi(double d){
		d=(d)*(3.1415926);
		ifj16.print(d+"\n");		//12
		return d;
	}

}




class post{
	static int counter = 5*5*5/5;

}
