class Main {
	static int i;
	static int value;
	static void run(){
		i=2;
		value=0;
		int a=f();
		
		
	}
	
	static int f(){
		int a;
		if(i){
			i=i-1;
			value=value+10;
			a=f();
			value = value + 100;
			return value;
		}else{
			value = value + 1;
			return value;
		}
		
		
		
		return a;
	}
}