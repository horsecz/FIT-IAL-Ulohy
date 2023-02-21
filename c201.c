
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    tList *new = (tList*) calloc(sizeof(tList), 1);
    //if (L == NULL)
    //    fprintf(stderr, "Chyba v inicializaci");
    //    return;

	new->First = NULL;
	new->Act = NULL;

	*L = *new;
	free(new);
	return;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    if (L->First == NULL)
        return;

    tList *temp = calloc(sizeof(tList), 1);
    L->Act = L->First;

    while (L->Act != NULL) {
        temp->Act = L->Act;
        L->Act = L->Act->ptr;
        free(temp->Act);
    }

    L->First = NULL;
    L->Act = NULL;

    free(temp);
    return;
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    tList *temp = calloc(sizeof(tList), 1);
    temp->First = NULL;
    temp->Act = NULL;

    if (L->First == NULL) { // seznam prazdny
        L->First = (tElemPtr) calloc(sizeof(tElemPtr) + sizeof(int), 1);
        L->First->ptr = NULL;
        L->First->data = val;
    } else { // neprazdny
        temp->First = (tElemPtr) calloc(sizeof(tElemPtr) + sizeof(int), 1);
        temp->First->data = val;
        temp->First->ptr = L->First;
        L->First = temp->First;
    }

    free(temp);
    return;
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L->Act = L->First;
	return;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if (L->First == NULL)
        Error();
	else
	    *val = L->First->data;

    return;
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->First == NULL)
	    return;

	tList *temp = (tList*) calloc(sizeof(tList), 1);
	temp->Act = NULL;
	temp->First = NULL;

	temp->Act = L->First;
	L->First = L->First->ptr;

	if (L->Act == temp->Act)
	    L->Act = NULL;

	free(temp->Act);

	free(temp);
    return;
}

void PostDelete (tList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/

    if (L->Act == NULL || L->Act->ptr == NULL)
        return;

    tList *temp = (tList*) calloc(sizeof(tList), 1);
    temp->Act = L->Act->ptr;
    temp->First = NULL;

	if (L->Act->ptr->ptr != NULL) { // prvek za mazanym neni posledni
	    L->Act->ptr = L->Act->ptr->ptr;
        free(temp->Act);
	} else { // mazany prvek je posledni
	    free(L->Act->ptr);
	    L->Act->ptr = NULL;
	}

	free(temp);
    return;
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
    if (L->Act == NULL)
        return;

    tList *temp = (tList*) calloc(sizeof(tList), 1);
    temp->Act = NULL;
    temp->First = NULL;

    if (L->Act->ptr == NULL) { // prvek je posledni
        L->Act->ptr = (tElemPtr) calloc(sizeof(tElemPtr)+sizeof(int), 1);
        L->Act->ptr->ptr = NULL;
        L->Act->ptr->data = val;
    } else {
        temp->Act = (tElemPtr) calloc(sizeof(tElemPtr)+sizeof(int), 1);
        temp->Act->ptr = L->Act->ptr;
        temp->Act->data = val;

        L->Act->ptr = temp->Act;
    }

    free(temp);
    return;
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    if (L->Act == NULL) {
        Error();
        return;
    }

    *val = L->Act->data;
    return;
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	if (L->Act == NULL)
	    return;

	L->Act->data = val;
	return;
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	if (L->Act == NULL)
	    return;

	L->Act = L->Act->ptr;
	return;
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
    return ((L->Act != NULL) ? 1 : 0);
}

/* Konec c201.c */
