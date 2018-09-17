#include <stdio.h>
#include <string.h>
extern int vez(int poz[], int plocha[][8]);
extern int jezdec(int poz[], int plocha[][8]);
extern int strelec(int poz[], int plocha[][8]);
extern int kralovna(int poz[], int plocha[][8]);
extern int kral(int poz[], int plocha[][8]);
extern int pesak(int poz[], int hrac, int plocha[][8]);
extern int jeKonec(int vyherce);

/* Nastavení int hodnoty jednotlivých figurek*/
static int prazdnePolicko = 0;
static int bPesak=1, bJezdec=2, bStrelec=3, bVez=4, bKralovna=5, bKral=6;
static int cPesak=7, cJezdec=8, cStrelec=9, cVez=10, cKralovna=11, cKral=12;
int enPassantB[8], enPassantC[8];

/* Nastaví figurky pro start hry*/
void nastavPlochu(int docasnaPlocha[][8]){
	docasnaPlocha[7][7] = bVez;
	docasnaPlocha[7][6] = bJezdec;
	docasnaPlocha[7][5] = bStrelec;
	docasnaPlocha[7][3] = bKralovna;
	docasnaPlocha[7][4] = bKral;
	docasnaPlocha[7][2] = bStrelec;
	docasnaPlocha[7][1] = bJezdec;
	docasnaPlocha[7][0] = bVez;
	docasnaPlocha[0][7] = cVez;
	docasnaPlocha[0][6] = cJezdec;
	docasnaPlocha[0][5] = cStrelec;
	docasnaPlocha[0][3] = cKralovna;
	docasnaPlocha[0][4] = cKral;
	docasnaPlocha[0][2] = cStrelec;
	docasnaPlocha[0][1] = cJezdec;
	docasnaPlocha[0][0] = cVez;
	
	int i, j;
	for (j = 0; j < 8; ++j) {
		/*černá řada pěšáků*/
		docasnaPlocha[1][j] = cPesak;
	}

	for (j = 0; j < 8; ++j) {
	/*bílá řada pěšáků*/
		docasnaPlocha[6][j] = bPesak;
	}

	for (i = 2; i < 6; ++i) {
		for (j = 0; j < 8; ++j) {
			/*prázdná políčka*/
			docasnaPlocha[i][j] = prazdnePolicko;
		}
	}
}

/* resetuje hodnoty v Passant poli na 0, aby nebylo možné provést podruhé tento nah na stejném poli*/
void resetujPassantPole(void) {
	int i;
	for (i = 0; i < 8; ++i) {
		enPassantB[i] = 0;
		enPassantC[i] = 0;
	}
}

/* Funkce která se zavolá, v případě, že se pěšák pohne o 2 pole*/
void nastavPassant(int column, int hrac) {
	if (hrac) {
		enPassantC[column] = 1;
	} else {
		enPassantB[column] = 1;
	}
}

/* Zkontroluje, zda je možné provést tzv. Braní mimochodem, často též en passant (soupeřův pěšák se pohnul o 2 pole dopředu a je možné ho skočit) */
int zkontrolujPassant(int row,int column, int hrac) {
	if (!hrac) {
		if (row == 3) {
			return enPassantC[column];
		}else return 0; 
	} else {
		if (row == 4) {
			return enPassantB[column];
		}else return 0; 
	}
}

/* Funkce dokončující tah. Zároveň kontroluje, zda nebyl na cílové pozici smazán král, v tom případě se hra ukončí, pokud se pěšák dostane na poslední pole, změní se na královnu*/
void dokoncitTah(int poz[], int b[][8]) {
	if(b[poz[3]][poz[2]] == 6){
		jeKonec(2);
	}else if(b[poz[3]][poz[2]] == 12){
		jeKonec(1);
	}
	if(b[poz[1]][poz[0]] == 1 && poz[3] ==0){
		b[poz[1]][poz[0]] = 5;
	}

	if(b[poz[1]][poz[0]] == 7 && poz[3] ==7){
		b[poz[1]][poz[0]] = 11;
	}
	
	int docasnaFigurka = b[poz[1]][poz[0]];
	b[poz[1]][poz[0]] = 0;
	b[poz[3]][poz[2]] = docasnaFigurka;
	resetujPassantPole();
}

/*Funkce kontrolující, zda je na cílové pozici nepřátelská, nebo žádná figurka*/
int zkontrolujBarvu(int tah[], int hrac, int b[][8]) {
	int barvaA = tah[0];
	int barvaB = tah[2];
	int FigurkaA = b[tah[1]][barvaA];
	int FigurkaB = b[tah[3]][barvaB];
	if (((hrac == 0) && ((FigurkaA > 0 && FigurkaA < 7) && ((FigurkaB > 6 && FigurkaB < 13) || (FigurkaB == 0)))) || ((hrac == 1) && ((FigurkaA > 6 && FigurkaA < 13) && ((FigurkaB > 0 && FigurkaB < 7) || (FigurkaB == 0))))) {
		return 1;
	}
	else {
		return 0;
	}
}

/*Metoda kontrolující validitu tahu, metoda se převážně odkazuje na metody jednotlivých figurek*/
int zkontrolujPohyb(int vstup[], int hrac, int plocha[][8]) {
	int piece = plocha[vstup[1]][vstup[0]];
    int pomocna;
	if (piece == 1 || piece == 7) {
		/*pěšák*/
        pomocna = pesak(vstup, hrac, plocha);
		if (pomocna == 1) {
            dokoncitTah(vstup, plocha);
			return 1;
		}
        else if (pomocna == 2)
        {
            return 1;
        }
		else {
			return 0;
		}
	}
	else if (piece == 2 || piece == 8) {
		if (jezdec(vstup, plocha)) {
            dokoncitTah(vstup, plocha);
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (piece == 3 || piece == 9) {
		/*střelec*/
		if (strelec(vstup, plocha)) {
            dokoncitTah(vstup, plocha);
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (piece == 4 || piece == 10) {
		/*věž*/
		if (vez(vstup, plocha)) {
            dokoncitTah(vstup, plocha);
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (piece == 5 || piece == 11) {
		/*královna*/
		if (kralovna(vstup, plocha)) {
            dokoncitTah(vstup, plocha);
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (piece == 6 || piece == 12) {
		/*králl*/
		if (kral(vstup, plocha)) {
            dokoncitTah(vstup, plocha);
			return 1;
		}
		else {
			return 0;
		}
	}
    return 0;
}

/* Zkontroluje zda výchozí a cílová pozice X a Y se nechází na hrací ploše */
int checkvstup(int vstup[]) {
	if ((vstup[0] >= 0 && vstup[0] < 8) ) {
		if (vstup[1] >= 0 && vstup[1] < 8) {
			if ((vstup[2] >= 0 && vstup[2] < 8) ) {
				if (vstup[3] >= 0 && vstup[3] < 8) {
					return 1;
				}
			}
		}
	}
	return 0;
}

/* Provedení tahu, nejprve zkontrolujeme, zda sedí vstupní hodnoty, následně zkontroluje, zda je na cílové pozici nepřátelská figurka, nebo žádná... */
int provedTah(int hrac, int *tah, int plocha[][8])
{
	if (checkvstup(tah)) {
		if (zkontrolujBarvu(tah, hrac, plocha)) {
			if (zkontrolujPohyb(tah, hrac, plocha)) {
				return 0;
			}
		}
	} 
	return 1;
}
