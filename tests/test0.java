class Main{
	static void run(){
		int a =2;
		//f(1,1,1);
		//a=f(2,a,a);
		while(a<100){
			a=f(2,a,a);
		}
	
		
	}
	static int f(int a,int b, int c){
		int d=a+b+c;
		return d;	
	}
}