#include <stdlib.h>

extern void dokoncitTah(int poz[], int b[][8]);
extern void nastavPassant(int column, int hrac);
extern int zkontrolujPassant(int row, int column, int hrac);

/*
Tato tøída zajišuje pohyb jednotlivých figurek, kde každá metoda zajišuje danou figurku.
Každá metoda nastavuje, kam se mùže která figurka pohnout, 
napø. vìž se mùže pohybovat pouze vodorovnì a svisle. 
Napø. u pìšáka metoda umožòuje skákat na sever, pøípadnì severozápad, nebo severovýchod. Pokud je na tahu druhý hráè, tak je možný pouze pohyb na opaènou stranu.
Navíc každá metoda kontroluje, zda mezi výchozí a cílovou pozicí není jiná figurka neumožòující provést tah. 
*/

int zkontrolujPozici(int row, int col, int b[][8]) {
	return b[row][col];
}
 
int pesak(int poz[], int hrac, int plocha[][8]) {
	if (hrac) {
		if (poz[3] == poz[1]+1 && poz[2] == poz[0]+1) {
			if (zkontrolujPozici(poz[3], poz[2], plocha)) {
				return 1;
			} else if(zkontrolujPassant(poz[1],poz[2],hrac)) {
				plocha[poz[3]-1][poz[2]] = 0;
				return 1;
			}
		}
		else if (poz[3] == poz[1]+1 && poz[2] == poz[0]-1) {
			if (zkontrolujPozici(poz[3], poz[2], plocha)) {
				return 1;
			} else if(zkontrolujPassant(poz[1],poz[2],hrac)) {
				plocha[poz[3]-1][poz[2]] = 0;
				return 1;
			}
			else {
				return 0;
			}
		}
		else if (poz[3] == poz[1]+1 && poz[2] == poz[0]) {
			if (!zkontrolujPozici(poz[3], poz[2], plocha)) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else if (poz[3] == poz[1]+2 && poz[2] == poz[0]) {
			if (!zkontrolujPozici(poz[3], poz[2], plocha) && !zkontrolujPozici(poz[3]-1, poz[2], plocha) && (poz[1] == 1 || poz[1] == 6)) {
				dokoncitTah(poz, plocha);
				nastavPassant(poz[0],hrac);
				return 2;
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}

	else {
		if (poz[3] == poz[1]-1 && poz[2] == poz[0]+1) {
			if (zkontrolujPozici(poz[3], poz[2], plocha)) {
				return 1;
			} else if(zkontrolujPassant(poz[1],poz[2],hrac)) {
				plocha[poz[3]+1][poz[2]] = 0;
				return 1;
			}
		}
		else if (poz[3] == poz[1]-1 && poz[2] == poz[0]-1) {
			if (zkontrolujPozici(poz[3], poz[2], plocha)) {
				return 1;
			} else if(zkontrolujPassant(poz[1],poz[2],hrac)) {
				plocha[poz[3]+1][poz[2]] = 0;
				return 1;
			}
			else {
				return 0;
			}
		}
		else if (poz[3] == poz[1]-1 && poz[2] == poz[0]) {
			if (!zkontrolujPozici(poz[3], poz[2], plocha)) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else if (poz[3] == poz[1]-2 && poz[2] == poz[0]) {

			if (!zkontrolujPozici(poz[3], poz[2], plocha) && !zkontrolujPozici(poz[3]+1, poz[2], plocha) && (poz[1] == 1 || poz[1] == 6)) {
				dokoncitTah(poz, plocha);
				nastavPassant(poz[0],hrac);
				return 2;
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}
    return 0;
}

int vez(int poz[], int plocha[][8]) {
	int i;
	if (poz[3] > poz[1] && poz[2] == poz[0]) {
		for(i = poz[1]+1; i < poz[3]; ++i) {
			if (zkontrolujPozici(i,poz[0],plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] == poz[1] && poz[2] < poz[0]) {
		for(i = poz[0]-1; i > poz[2]; --i) {
			if (zkontrolujPozici(poz[1],i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] < poz[1] && poz[2] == poz[0]) {
		for(i = poz[1]-1; i > poz[3]; --i) {
			if (zkontrolujPozici(i,poz[0],plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] == poz[1] && poz[2] > poz[0]) {
		for(i = poz[0]+1; i < poz[2]; ++i) {
			if (zkontrolujPozici(poz[1],i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}

int kralovna(int poz[], int plocha[][8]) {
	int i, j;
	if (poz[3] > poz[1] && poz[2] == poz[0]) {
		for(i = poz[1]+1; i < poz[3]; ++i) {
			if (zkontrolujPozici(i,poz[0],plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] == poz[1] && poz[2] < poz[0]) {
		for(i = poz[0]-1; i > poz[2]; --i) {
			if (zkontrolujPozici(poz[1],i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] < poz[1] && poz[2] == poz[0]) {
		for(i = poz[1]-1; i > poz[3]; --i) {
			if (zkontrolujPozici(i,poz[0],plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] == poz[1] && poz[2] > poz[0]) {
		for(i = poz[0]+1; i < poz[2]; ++i) {
			if (zkontrolujPozici(poz[1],i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] > poz[1] && poz[2] > poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]+1; i < poz[2]; ++i) {
			j++;
			if (zkontrolujPozici(j,i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] > poz[1] && poz[2] < poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]-1; i > poz[2]; --i) {
			++j;
			if (zkontrolujPozici(j,i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] < poz[1] && poz[2] < poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]-1; i > poz[2]; --i) {
			--j;
			if (zkontrolujPozici(j,i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else if (poz[3] < poz[1] && poz[2] > poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]+1; i < poz[2]; ++i) {
			--j;
			if (zkontrolujPozici(j,i,plocha)) {
				return 0;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}
int jezdec(int poz[], int plocha[][8]) {
	if (poz[3] == poz[1]+1 && poz[2] == poz[0]+2) {
		return 1;
	}
	else if (poz[3] == poz[1]+2 && poz[2] == poz[0]+1) {
		return 1;
	}
	else if (poz[3] == poz[1]+2 && poz[2] == poz[0]-1) {
		return 1;
	}
	else if (poz[3] == poz[1]+1 && poz[2] == poz[0]-2) {
		return 1;
	}
	else if (poz[3] == poz[1]-1 && poz[2] == poz[0]-2) {
		return 1;
	}
	else if (poz[3] == poz[1]-2 && poz[2] == poz[0]-1) {
		return 1;
	}
	else if (poz[3] == poz[1]-2 && poz[2] == poz[0]+1) {
		return 1;
	}
	else if (poz[3] == poz[1]-1 && poz[2] == poz[0]+2) {
		return 1;
	}
	else {
		return 0;
	}
}


int strelec(int poz[], int plocha[][8]) {
	int i,j;
	if (poz[3] > poz[1] && poz[2] > poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]+1; i < poz[2]; i++) {
				j++;
				if (zkontrolujPozici(i,j,plocha)) {
					return 0;
				}
			}
		return 1;
	}
	else if (poz[3] > poz[1] && poz[2] < poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]-1; i > poz[2]; i--) {
				j++;
				if (zkontrolujPozici(i,j,plocha)) {
					return 0;
				}
			}
		return 1;
	}
	else if (poz[3] < poz[1] && poz[2] < poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]-1; i > poz[2]; i--) {
				--j;
				if (zkontrolujPozici(i,j,plocha)) {
					return 0;
				}
			}
		return 1;
	}
	else if (poz[3] < poz[1] && poz[2] > poz[0]) {
		if (!(abs(poz[0]-poz[2])==abs(poz[1]-poz[3]))){
			return 0;
		}
		j = poz[1];
		for(i = poz[0]+1; i < poz[2]; i++) {
				--j;
				if (zkontrolujPozici(i,j,plocha)) {
					return 0;
				}
			}
		return 1;
	}
	else {
		return 0;
	}
}


int kral(int poz[], int plocha[][8]) {
	if (poz[3] == poz[1]+1 && poz[2] == poz[0]) {
		return 1;
	}
	else if (poz[3] == poz[1] && poz[2] == poz[0]-1) {
		return 1;
	}
	else if (poz[3] == poz[1]-1 && poz[2] == poz[0]) {
		return 1;
	}
	else if (poz[3] == poz[1] && poz[2] == poz[0]+1) {
		return 1;
	}
	else if (poz[3] == poz[1]+1 && poz[2] == poz[0]+1) {
		return 1;
	}
	else if (poz[3] == poz[1]+1 && poz[2] == poz[0]-1) {
		return 1;
	}
	else if (poz[3] == poz[1]-1 && poz[2] == poz[0]-1) {
		return 1;
	}
	else if (poz[3] == poz[1]-1 && poz[2] == poz[0]+1) {
		return 1;
	}
	else {
		return 0;
	}
}
