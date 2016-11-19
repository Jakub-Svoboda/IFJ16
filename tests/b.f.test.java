class pre{
	static int inc(int x){
		x=x+1;
		return x;
	}
	static int a = (((4+8)/3)*4)*(1*2);
	static void basnicka(){
		ifj16.print(Main.shakesbro+"\n");
		return;
	}	
	static int dec(int x){
		x=x-1;
		return x;
	}
}



class Main{
	static void run(){
		pre.basnicka();
		ifj16.print(pre.a+"\n");		//12
		double mujDouble = 4;
		mujDouble=mulpi(mujDouble);		
		ifj16.print(post.counter+"\n");		
		int a = 10;
		ifj16.print(a+"\n");	//10
		a=pre.inc(a);
		ifj16.print(a+"\n");	//11
		a=pre.dec(a);
		ifj16.print(a+"\n");	//10
		a=post.mulByTwo(a);
		ifj16.print(a+"\n");	//20
		
		post.rectangle(5,10);
		
	}
	static String shakesbro = "\n\tNot from the stars do I my judgement pluck,\n\tAnd yet methinks I have astronomy,\n\tBut not to tell of good, or evil luck,\n\tOf plagues, of dearths, or seasons' quality.\n";
	static double mulpi(double d){
		d=(d)*(3.1415926);
		ifj16.print(d+"\n");		
		return d;
	}

	
	

}

class post{

	static int mulByTwo(int x){
		x=x*2;
		return x;
	}
	static int counter = (5*5*5/5)/5;
	static int recurse(int x){
		
		return x;
	}
	static int rectangle(int width, int height){
		int i=0;
		int j=0;
		ifj16.print("\n");
		while(i<width){
			
			while(j<height){
				ifj16.print("M");
				j=j+1;
			}
			j=0;
			ifj16.print("\n");
			i=i+1;
		}
		ifj16.print("\n");
		if(counter > 5){
			return 0;
		}else{
			return 1;
		}
		
		
	}
	
}