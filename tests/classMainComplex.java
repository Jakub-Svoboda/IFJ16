class Main {
	static void run() {
		String oddelovac = "\n-------------------\n\n";
		int i5;
		ifj16.print("Builtin classMainComplex.java\n");
		ifj16.print("1 - Zadej 5:\n");
		i5 = ifj16.readInt();
        ifj16.print("Prvni tisk " + i5+ "\n");
		ifj16.print("Druhy tisk " +i5 + "\n");

		ifj16.print(oddelovac);

		ifj16.print("2 - Zadej 10:\n");
		int i10= ifj16.readInt();
        ifj16.print("Prvni tisk " + i10+ "\n");
		ifj16.print("Druhy tisk " +i10 + "\n");

		ifj16.print(oddelovac);
		ifj16.print("4 - Pocitani 5 + 10:\n");
		int i5i10 = i5+i10;
        ifj16.print("Prvni tisk " + i5i10+ "\n");
		ifj16.print("Druhy tisk " +i5i10 + "\n");

		ifj16.print(oddelovac);

		ifj16.print("5 - Zadej \"pivo\":\n");
		String pivo = ifj16.readString();
		ifj16.print("Prvni tisk " + pivo+ "\n");
		ifj16.print("Druhy tisk " +pivo + "\n");

		ifj16.print(oddelovac);

		ifj16.print("6 - Compare vstup \"pivo\" se stringem:\n");							//
		String pivoStr = "pivo";															//
		int cmpPivo1 = ifj16.compare(pivoStr, pivo);										//
		int cmpPivo2 = ifj16.compare("pivo", pivo);											//
		int cmpPivo3 = 2 - 3;// ifj16.compare("Pivo", "pivo");									//
		ifj16.print("ifj16.compare(\"pivo\", pivo) " + cmpPivo1 + " <- ma byt 0.\n");		//
		ifj16.print("ifj16.compare(pivoStr, pivo) " + cmpPivo2 + " <- ma byt 0.\n");		//
		ifj16.print("ifj16.compare(\"Pivo\", pivo) " + cmpPivo3 + " <- nema byt 0.\n");		//
																							//
		ifj16.print(oddelovac);																//
																							//
		ifj16.print("7 - Zadej \"to pivo je moje\":\n");											//
		String toPivoJeMoje =  ifj16.readString();												//
		ifj16.print("Tisk " + toPivoJeMoje+ "\n");										//
		ifj16.print(oddelovac);

 		ifj16.print("8 - Compare vstup \"pivo\" s \"topivojemoje\":\n");
		/*
		int cmpToPivoJeMoje1 = ifj16.compare(toPivoJeMoje, pivo);
		int cmpToPivoJeMoje2 = ifj16.compare("topivojemoje", pivo);
		ifj16.print("ifj16.compare(\"topivojemoje\", pivo) " + cmpToPivoJeMoje2+ " <- nema byt 0.\n");
		ifj16.print("ifj16.compare(toPivoJeMoje, pivo) " +cmpToPivoJeMoje1 + " <- nema byt 0.\n");
		*/
	}
}
