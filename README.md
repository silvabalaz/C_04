# C_04
Simulacija raspoređivanja dretvi

Simulirati raspoređivanje dretvi prema prioritetu kao osnovnom kriteriju te FIFO ili RR kao dodatnom (kada prvi kriterij nije dostatan). Svaka dretva ima zadan i prioritet i način raspoređivanja (FIFO ili RR). Ako je aktivna dretva tipa RR, onda će nakon jednog koraka (kvanta/sekunde) biti vraćena u red pripravnih, iza dretvi istog prioriteta, ali ispred onih manjeg prioriteta. Ako je dretva tipa FIFO onda će ona raditi do završetka ili do pojave prioritetnije dretve.

Načini raspoređivanja SCHED_FIFO i SCHED_RR odgovaraju navedenom opisu (raspoređivač može raditi s dretvama oba tipa istovremeno u sustavu).



Prijedlog strukture podataka te primjer ispisa



int t; //simulacija vremena (trenutno vrijeme)


struct dretva {
	int id; //1, 2, 3, ...
	int p;  //preostalo vrijeme rada
	int prio; //prioritet
	int rasp; //način raspoređivanja (za opcionalni zadatak)
};

define MAX_DRETVI	7

struct dretva *P[MAX_DRETVI]; //red pripravnih dretvi, P[0] = aktivna dretva

define DRETVI	6

int nove[DRETVI][5] =
{
	/* trenutak dolaska, id, p, prio, rasp (prio i rasp se koriste ovisno o rasporedivacu) */
	{ 1,  3, 5, 3, 1 }, /* rasp = 0 => PRIO+FIFO; 1 => PRIO+RR */
	{ 3,  5, 6, 5, 1 },
	{ 7,  2, 3, 5, 0 },
	{ 12, 1, 5, 3, 0 },
	{ 20, 6, 3, 6, 1 },
	{ 20, 7, 4, 7, 1 },
};


void ispis_stanja ( int ispisi_zaglavlje )
{
	int i;

	if ( ispisi_zaglavlje ) {
		printf ( "  t    AKT" );
		for ( i = 1; i < MAX_DRETVI; i++ )
			printf ( "     PR%d", i );
		printf ( "\n" );
	}

	printf ( "%3d ", t );
	for ( i = 0; i < MAX_DRETVI; i++ )
		if ( P[i] != NULL )
			printf ( "  %d/%d/%d ",
				 P[i]->id, P[i]->prio, P[i]->p );
		else
			printf ( "  -/-/- " );
	printf ( "\n");
}

int main () {/* napraviti! */}


