class Main
{
	static void run()
	{
		ifj16.print("Challenge: Think of a word and then code it.\n");
		ifj16.print("(Change its characters positions like: Hello -> oHlel)\n");
		ifj16.print("Original and coded word cannot match.\n");
		ifj16.print("Input original word: ");
		String original = ifj16.readString();
		ifj16.print("Input a coded word: ");
		String coded = ifj16.readString();
		int res = ifj16.compare(original, coded);
		if(res == 0) {
			ifj16.print("Original and coded word cannot match!\n");
			return;
		}
		else {}
		String a = ifj16.sort(original);
		String b = ifj16.sort(coded);
		res = ifj16.compare(a, b);
		if(res == 0) {
			ifj16.print("Congratulations! You managed to code the word correctly.\n");
		}
		else {
			ifj16.print("You failed to code the word correctly.\n");
		}
		ifj16.print("Now input a string, which contains your original word OR your coded word OR both: ");
		String text = ifj16.readString();
		int text_len = ifj16.length(text);
		int offset = 0;
//		int num = -2;			//This does not work
		int resoriginal = ifj16.find(text, original);
		int rescoded = ifj16.find(text, coded);
		if((resoriginal + rescoded + 1) < 0) {
			ifj16.print("You failed to input a string which contains your original word or your coded word.\n");
		}
		else {
			if(resoriginal < 0) {
				ifj16.print("Congratulations! You managed to input a string which contains your coded word \"" + coded + "\" on index: " + rescoded + "\n");
				ifj16.print("Text     :" + text + ":\nCoded    :");
				Other.printspace(rescoded);
				ifj16.print(coded);
				offset = ifj16.length(coded);
				offset = text_len - (offset + rescoded);
				Other.printspace(offset);
				ifj16.print(":\n");
				return;
			}
			else {}
			if(rescoded < 0) {
				ifj16.print("Congratulations! You managed to input a string which contains your original word \"" + original + "\" on index: " + resoriginal + "\n");
				ifj16.print("Text     :" + text + ":\nOriginal :");
				Other.printspace(resoriginal);
				ifj16.print(original);
				offset = ifj16.length(original);
				offset = text_len - (offset + resoriginal);
				Other.printspace(offset);
				ifj16.print(":\n");
				return;
			}
			else {}
			ifj16.print("Congratulations! You managed to input a string which contains your original word \"" + original + "\" on index: " + resoriginal + " and your coded word \"" + coded + "\" on index: " + rescoded + "\n");
			if(rescoded < resoriginal) {
				ifj16.print("Text     :" + text + ":\nCoded    :");
				Other.printspace(rescoded);
				ifj16.print(coded);
//				int someint;			//This declaration does not work here
				offset = ifj16.length(coded);
				offset = text_len - (offset + rescoded);
				Other.printspace(offset);
				ifj16.print(":\nOriginal :");
				Other.printspace(resoriginal);
				ifj16.print(original);
				offset = ifj16.length(original);
				offset = text_len - (offset + resoriginal);
				Other.printspace(offset);
				ifj16.print(":\n");
			}
			else {
				ifj16.print("Text     :" + text + ":\nOriginal :");
				Other.printspace(resoriginal);
				ifj16.print(original);
				offset = ifj16.length(original);
				offset = text_len - (offset + resoriginal);
				Other.printspace(offset);
				ifj16.print(":\nCoded    :");
				Other.printspace(rescoded);
				ifj16.print(coded);
				offset = ifj16.length(coded);
				offset = text_len - (offset + rescoded);
				Other.printspace(offset);
				ifj16.print(":\n");
			}
		}
	}
}

class Other
{
	static void printspace(int n) {
		/*
//		for(int i = 0; i < n; i++) {			//This also does not work, even if int i declared before for()
			ifj16.print(" ");
		}
		*/
		while((n) > (0)) {
			ifj16.print(" ");
			n = (n - 1);
		}
	}
}