#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

//strukturom podataka

int t; //simulacija vremena (trenutno vrijeme)

struct dretva{

	int id; //1,2,3...
	int p; //preostalo vrijeme rada
	int prio; //prioritet
	int rasp; //nacin raspoređivanja

};

#define MAX_DRETVI 7

struct dretva *P[MAX_DRETVI]; //red pripravnih dretvi, P[0] = aktivna dretva;
/*podaci o događajima pojave novih poslova -dretvi*/
#define DRETVI 6

int nove[DRETVI][5]=
{
	/* trenutak dolaska, id, p, prio,rasp (prio i rasp se koriste ovisno o raspoređivaču) */

	{1,3,5,3,1},
	{3,5,6,5,1},
	{7,2,3,5,1},
	{12,1,5,3,0},
	{20,6,3,6,1},
	{20,7,4,7,1}
};

void ispis_stanja(int ispisi_zaglavlje)
{
	int i;

	if(ispisi_zaglavlje){
		printf("  t  AKT");
		for(i = 1; i < MAX_DRETVI; i++)
			printf("     PR%d", i);
		printf("\n");
	}

	printf("%3d  ",t);

	for(i = 0; i < MAX_DRETVI; i++ )
	{	
		
		if(P[i] != NULL ){
				  
					printf(" %d/%d/%d/-%d ", P[i]->id, P[i]->prio, P[i]->p, P[i]->rasp); 	

			}
		else
			printf(" -/-/-  "); 
		}
	
	
	printf("\n");
}


void ubaci_nulti(int nove[][5], int indexDretve){

	P[0] = malloc(sizeof(struct dretva)); 
	P[0]->id = nove[indexDretve][1];
	P[0]->p = nove[indexDretve][2];
	P[0]->prio = nove[indexDretve][3];
	P[0]->rasp = nove[indexDretve][4]; 

	printf("%3d -- nova dretva id=%d,  p=%d, prio=%d, rasp=%d\n", t, nove[indexDretve][1], nove[indexDretve][2], nove[indexDretve][3], nove[indexDretve][4]);
}

void ubaci(int nove[][5], int indexDretve){

	int k,m,n;
	
	printf("%3d -- nova dretva id=%d,  p=%d, prio=%d\n", t, nove[indexDretve][1], nove[indexDretve][2], nove[indexDretve][3] );

	for(k = 0; k < MAX_DRETVI; k++)
	{	
	    if(P[k] == NULL){ 

			for(m = 0; m < k; m++)
			{		   				
				if( (nove[indexDretve][3] > P[m]->prio ))
				{	
					P[k] = malloc(sizeof(struct dretva));  	

					for( n = m+1; n > 0; n--)
					{     
							P[n]->id = P[n-1]->id;
							P[n]->p = P[n-1]->p;
							P[n]->prio = P[n-1]->prio; 
							P[n]->rasp = P[n-1]->rasp;		
					} 
		
			 		P[m]->id = nove[indexDretve][1];
					P[m]->p = nove[indexDretve][2];
					P[m]->prio = nove[indexDretve][3];
					P[m]->rasp = nove[indexDretve][4];
					break;

				}

				if(nove[indexDretve][3] <= P[m]->prio){ 	
					if(P[m+1] == NULL) 
					{
							P[k] = malloc(sizeof(struct dretva)); 
							P[m+1]->id = nove[indexDretve][1];
							P[m+1]->p = nove[indexDretve][2];
							P[m+1]->prio = nove[indexDretve][3];
							P[m+1]->rasp = nove[indexDretve][4];
							
							break;
					}			
				}			
			}

		break;
			
		}
	}
	
	
}

int izbaci()
{

	int k,m;
	printf(" Dretva %d je zavrsila\n", P[0]->id);
	
	for(k = 0; k < MAX_DRETVI; k++)
	{	
		    if(P[k] == NULL){ 
		    	
			   for( m = 0; m < k-1; m++ )
			   {
		   			P[m]->id = P[m+1]->id;
					P[m]->p = P[m+1]->p;
					P[m]->prio = P[m+1]->prio; 
					P[m]->rasp = P[m+1]->rasp;
			   }
			
			P[k-1]=NULL; 
			break;
			}
	} 	
}

//aktivnu stavi u P[i] ispred onog koji ima manji prioritet,a iza onog koji ima isti
void stavi_u_red()
{
	int k,m,n; 

	for(k = 0; k < MAX_DRETVI; k++)
	{	
	    if(P[k] == NULL){ 

		    //P[k] = malloc(sizeof(struct dretva)); 

			for(m = 1; m < k; m++)
			{		   			
				if(P[0]->prio == P[m]->prio){ 	

					if(P[m+1] == NULL)
					{
						    struct dretva Temp;

						    Temp.id = P[m]->id;
						    Temp.p = P[m]->p;
						    Temp.prio = P[m]->prio;
						    Temp.rasp = P[m]->rasp;

							P[m]->id = P[0]->id;
							P[m]->p = P[0]->p;
							P[m]->prio = P[0]->prio;	
							P[m]->rasp = P[0]->rasp;

							P[0]->id = Temp.id;
							P[0]->p = Temp.p;
							P[0]->prio = Temp.prio;	
							P[0]->rasp = Temp.rasp;

							break;
					}	

					if(P[0]->prio > P[m+1]->prio)
					{
						
					    struct dretva Temp;
						    
						    Temp.id = P[m]->id;
						    Temp.p = P[m]->p;
						    Temp.prio = P[m]->prio;
						    Temp.rasp = P[m]->rasp;

							P[m]->id = P[0]->id;
							P[m]->p = P[0]->p;
							P[m]->prio = P[0]->prio;	
							P[m]->rasp = P[0]->rasp;

							P[0]->id = Temp.id;
							P[0]->p = Temp.p;
							P[0]->prio = Temp.prio;
							P[0]->rasp = Temp.rasp;
					}			
				}
			}

		break;	

		}
	}
}

void otkucaj()
{	
	(P[0]->p)--; 
	t++;
	if(P[0]->rasp == 1)
		stavi_u_red(); 
		//ako je PRIO+RR

	sleep(1);	
}

int obradi(int nove[][5], int retci ,int stupci, int trenutniRedak )
{		
		int j;
		ispis_stanja(0); 
		ubaci_nulti(nove,trenutniRedak);				
		ispis_stanja(0);
	
				while(1)
				{
					if( (P[0] != NULL)&&(P[0]->p > 1)) { otkucaj(); }
					
					ispis_stanja(0); 
					for( j = trenutniRedak+1; j < DRETVI; j++ ){
						if(P[0]==NULL && nove[j][0]!=t && t<21) {  continue; }
					
						if(P[0] == NULL && nove[j][0] == t)
						{	ubaci_nulti(nove,j);
							ispis_stanja(0);
							j++;
							while(nove[j][0]==t){ 
									 
							          ubaci(nove,j);
							          ispis_stanja(0);
							          j++;
							          if(nove[j][0]!= t) break;
							        }
							   
							//otkucaj();
							ispis_stanja(0);
							//j++;

						}
					 	if(nove[j][0] == t) 
					 	{	
							ubaci(nove,j); 
							ispis_stanja(0);
							if(nove[j+1][0]!= t)
								otkucaj();
							ispis_stanja(0);			 		
					 	}							
					 	if(P[0]->p == 1) { 
					 		printf("if: ako je kraj dretve\n");
					 		ispis_stanja(0);
						    izbaci(); 
							ispis_stanja(0);
							//otkucaj();
							t++; 
							sleep(1);
							if(t==27) {printf("\n1111 \n");return t;}
							ispis_stanja(0); 
						}			
					}		
					if(t==27) { printf("\n2222\n ");return t;}
				}
return t;
}

int main(int argc, char** argv){
	
	int i;
	//postavljanje svih elemenata polja P na NULL
	for(i=0; i< MAX_DRETVI; i++)
		P[i] = NULL;
	//ispis pocetnog postavljenog stanja u trenutcima t
	ispis_stanja(1);
	
	while(1)
	{	//prolaz po svim dretvama matrice nove (retcima)
		for( i = 0; i < DRETVI; i++ )
		{ 	
			
			//ako u danom trenutku nova dretva iz matrice nova postoji
			if(nove[i][0] == t) 
				{t = obradi(nove,DRETVI,5,i); break;}
		}
		t++; 
		if(t > 20) { printf("\nGotovo\n"); break;};
	}

	//oslobodi memoriju polja P
	for(i=0; i< MAX_DRETVI; i++)
		free(P[i]);
}


