class Main {
	static void run() {
        String a = "ahoj";
        String b = "Karel ahroj vola zdola aheoj.";
        int findino = find.find(b, a);
        if(findino == 0) {
            ifj16.print("Retezec \""+ a +"\" byl v \""+ b + "\" nalezen.\n");
        }else {
            ifj16.print("Retezec \""+ a +"\" nebyl v \""+ b + "\" nalezen. :(\n");
        }
    }
}
class find {
    static int find(String source, String toFind) {
        int strLen = ifj16.length(toFind);
        int srcLen = ifj16.length(source);
        int srcCount = 0;
        int found = 99;

        String srcSubstr;
        int err = 0;
        while(found != 0 ) {
            if(srcCount + strLen > srcLen) {
                err = 1;
                found = 0;
            }else {
                srcSubstr = ifj16.substr(source, srcCount, strLen);
                found = ifj16.compare(toFind, srcSubstr);
                srcCount = 1 + srcCount;
            }
        }

        if(err) {
            return 666;
        }else {
            return found;
        }

    }
}
