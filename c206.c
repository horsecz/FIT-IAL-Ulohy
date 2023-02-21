
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    tDLList *new = (tDLList*) calloc(sizeof(tDLList), 1);
    //if (L == NULL)
    //    fprintf(stderr, "Chyba v inicializaci");
    //    return;

    new->First = NULL;
    new->Act = NULL;
    new->Last = NULL;

    *L = *new;
    free(new);
    return;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    if (L->First == NULL)
        return;

    // 1 prvek - lptr je vzdy NULL u first
    if (L->First->rptr == NULL) { // && L->First == L->Last && L->Last->lptr == NULL
        free(L->First);
    } else {
        while (L->Act != L->First) {
            L->Act = L->Last->lptr;
            free(L->Act->rptr);
            L->Last = L->Act;
        }

        if (L->First == L->Last) // && L->Act == L->First
            free(L->First);
    }

    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
    return;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLList *temp = (tDLList*) calloc(sizeof(tDLList), 1);
    if (temp == NULL) { DLError(); return; }

    tDLElemPtr new = (tDLElemPtr) calloc(sizeof(tDLElemPtr) + sizeof(int) + (2* sizeof(struct tDLElem*)), 1);
    temp->Act = new;
    if (temp->Act == NULL) { DLError(); free(temp); return; }
    temp->Act->data = val;

	if (L->First == NULL) { //empty list
	    L->First = temp->Act;
	    L->First->lptr = NULL;
	    L->First->rptr = NULL;
	    L->Last = L->First;
	} else { //nonempty list
	    temp->First = L->First;
	    L->First = temp->Act;
	    L->First->rptr = temp->First;
	    L->First->lptr = NULL;
	    L->First->rptr->lptr = L->First; // temp->First->lptr
	}

	free (temp);
	return;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLList *temp = (tDLList*) calloc(sizeof(tDLList), 1);
    if (temp == NULL) { DLError(); return; }

    tDLElemPtr new = (tDLElemPtr) calloc(sizeof(tDLElemPtr) + sizeof(int) + (2* sizeof(struct tDLElem*)), 1);
    temp->Act = new;
    if (temp->Act == NULL) { DLError(); free(temp); return; }
    temp->Act->data = val;

    if (L->First == NULL) { //empty list, L->First == L->Last
        L->First = temp->Act;
        L->First->lptr = NULL;
        L->First->rptr = NULL;
        L->Last = L->First;
    } else {
        L->Last->rptr = temp->Act;
        temp->Act->rptr = NULL;
        temp->Act->lptr = L->Last;
        L->Last = L->Last->rptr;
    }

    free(temp);
    return;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
    return;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
	return;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL) { DLError(); return; }
    *val = L->First->data;
    return;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL) { DLError(); return; }
    *val = L->Last->data;
    return;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	if (L->First == NULL) return;
	if (L->Act == L->First) L->Act = NULL;
    tDLList *temp = calloc(sizeof(tDLList), 1);

	if (L->First == L->Last) {
	    free(L->First);
	    L->First = NULL;
	    L->Last = NULL;
	} else if (L->First->rptr == L->Last) {
        free(L->First);
	    L->First = L->Last;
	    L->Last->lptr = NULL;
	} else {
	    temp->First = L->First->rptr;
        free(L->First);
        L->First = temp->First;
        L->First->lptr = NULL;
	}

	free(temp);
	return;
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L->First == NULL) return;
    if (L->Act == L->Last) L->Act = NULL;
    tDLList *temp = calloc(sizeof(tDLList), 1);

    if (L->First == L->Last) {
        free(L->First);
        L->First = NULL;
        L->Last = NULL;
    } else if (L->First->rptr == L->Last) {
        free(L->Last);
        L->Last = NULL;
        L->First->rptr = NULL;
    } else {
        temp->Last = L->Last->lptr;
        free(L->Last);
        L->Last = temp->Last;
        L->Last->rptr = NULL;
    }

    free(temp);
    return;
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L->Act == NULL || L->Act == L->Last) return;
    tDLList *temp = calloc(sizeof(tDLList), 1);

    if (L->First == L->Last) {
        free(L->First);
        L->First = NULL;
        L->Last = NULL;
        L->Act = NULL;
    } else if (L->First->rptr == L->Last) { // 2 prvky v seznamu, L->Act nemuze byt L->Last, tedy L->Act == L->First
        free(L->Last);
        L->First->rptr = NULL;
        L->Last = L->First;
    } else if (L->Act->rptr == L->Last) { // && L->Act != L->First
        free(L->Last);
        L->Last = L->Act;
        L->Last->rptr = NULL;
    } else { // && L->Act != L->First && L->Act->rptr != L->Last
        L->Act = L->Act->rptr;
        L->Act->lptr->rptr = L->Act->rptr;
        L->Act->rptr->lptr = L->Act->lptr;
        temp->Act = L->Act->lptr;
        free(L->Act);
        L->Act = temp->Act;
    }

    free(temp);
    return;
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if (L->Act == NULL || L->Act == L->First) return;
    tDLList *temp = calloc(sizeof(tDLList), 1);

    if (L->First == L->Last) {
        free(L->First);
        L->First = NULL;
        L->Last = NULL;
        L->Act = NULL;
    } else if (L->First->rptr == L->Last) { // 2 prvky v seznamu, L->Act nemuze byt L->First, tedy L->Act == L->Last
        free(L->First);
        L->First = L->Last;
        L->Last->lptr = NULL;
    } else if (L->Act->lptr == L->First) { // && L->Act != L->Last
        free(L->First);
        L->First = L->Act;
        L->First->lptr = NULL;
    } else { // && L->Act != L->Last && L->Act->lptr != L->First
        L->Act = L->Act->lptr;
        L->Act->rptr->lptr = L->Act->lptr;
        L->Act->lptr->rptr = L->Act->rptr;
        temp->Act = L->Act->rptr;
        free(L->Act);
        L->Act = temp->Act;
    }

    free(temp);
    return;
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act == NULL) return;
    tDLList *temp = (tDLList*) calloc(sizeof(tDLList), 1);

    tDLElemPtr new = (tDLElemPtr) calloc(sizeof(tDLElemPtr) + sizeof(int) + (2* sizeof(struct tDLElem*)), 1);
    temp->Act = new;
	if (temp->Act == NULL) { DLError(); return; }
	temp->Act->data = val;

	if (L->First == L->Last) { // pouze 1 prvek
        L->Last = temp->Act;
        L->First->rptr = temp->Act;
        temp->Act->lptr = L->First;
        temp->Act->rptr = NULL;
    } else if (L->Act == L->Last) { // && L->Act != L->First
        L->Last->rptr = temp->Act;
        temp->Act->lptr = L->Last;
        temp->Act->rptr = NULL;
        L->Last = temp->Act;
	} else { // && L->Act != L->Last
        temp->Act->lptr = L->Act;
        temp->Act->rptr = L->Act->rptr;
        L->Act->rptr = temp->Act;
        temp->Act->rptr->lptr = temp->Act;
	}

    free(temp);
    return;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act == NULL) return;
    tDLList *temp = (tDLList*) calloc(sizeof(tDLList), 1);

    temp->Act = (tDLElemPtr) calloc(sizeof(tDLElemPtr) + sizeof(int), 1);
    if (temp->Act == NULL) { DLError(); return; }
    temp->Act->data = val;

    if (L->First == L->Last) { // 1 prvek
        temp->Act->rptr = L->Last;
        L->First = temp->Act;
        temp->Act->lptr = NULL;
        L->Last->rptr = NULL;
        L->Last->lptr = temp->Act;
    } else if (L->Act == L->First) { // && L->Act != L->Last
        L->First->lptr = temp->Act;
        temp->Act->rptr = L->First;
        temp->Act->lptr = NULL;
        L->First = temp->Act;
    } else { // && L->Act != L->First
        temp->Act->rptr = L->Act;
        temp->Act->lptr = L->Act->lptr;
        L->Act->lptr = temp->Act;
        temp->Act->lptr->rptr = temp->Act;
    }

    free(temp);
    return;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if (L->Act == NULL) { DLError(); return; }
    *val = L->Act->data;
    return;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if (L->Act == NULL) return;
	L->Act->data = val;
	return;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act == NULL) return;
	L->Act = L->Act->rptr;
	return;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    if (L->Act == NULL) return;
    L->Act = L->Act->lptr;
    return;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
