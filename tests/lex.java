class Main {
	static void run() {
		String oddelovac = "\n-------------------\n\n";
        ifj16.print("6 - Compare vstup \"pivo\" se stringem:\n");							//
        String pivoStr = "pivo";
        String pivo = ifj16.readString();														//
        int cmpPivo1 = ifj16.compare(pivoStr, pivo);										//
        int cmpPivo2 = ifj16.compare("pivo", pivo);											//
        int cmpPivo3;																		//
        cmpPivo3 = 2 - 3;// ifj16.compare("Pivo", "pivo");									//
        ifj16.print("ifj16.compare(\"pivo\", pivo) " + cmpPivo1+ " <- ma byt 0.\n");		//
        ifj16.print("ifj16.compare(pivoStr, pivo) " + cmpPivo2 + " <- ma byt 0.\n");		//
        ifj16.print("ifj16.compare(\"Pivo\", pivo) " +cmpPivo3 + " <- nema byt 0.\n");		//
                                                                                            //
        ifj16.print(oddelovac);																//
                                                                                            //
        ifj16.print("7 - Zadej topivojemoje:\n");											//
        String toPivoJeMoje = "kek";							//TODO: This causes SEGFAULT. but why?
        toPivoJeMoje =  ifj16.readString();												//
        ifj16.print("Tisk " + toPivoJeMoje+ "\n");										//
    }
}
