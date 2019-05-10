#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
//-lm pour compiler
#define Lambda 9
#define Mu 1

#define EPSILON 1e-5
#define MAXEVENT 1000000	//taille max de l'echeancier
#define MAXTEMPS 10000	//cond d'arret

double temps = 0;
long int n = 0;		//nb de clients dans la file a l'instant temps
int compteur = 0;	//cond d'arret 2
double cumule = 0;

typedef struct Event {
	int type; //0 pour arrive 1 pour sortie
	double date;
	int etat; //0 pour non traité 1 pour non traité
}event;

typedef struct Echeancier {
	event tab[MAXEVENT];
	int taille;
}echeancier;

echeancier Ech;

double Exponnentielle(int lbda) {
	double r = (double)random()/RAND_MAX;	//entre 0 et 1
	
	while(r==0 || r==1) {	//tant que x vaut 0 ou 1 on refait un random
		r = (double)random()/RAND_MAX;
	}
	return -log(r)/(lbda*1.0); // - log(u)/lamda, avec U = unif(0,1)
}


void Ajouter_Ech(event e) {
	if(Ech.taille < MAXEVENT) {
		Ech.tab[Ech.taille] = e;
		Ech.taille++;
		//printf("Taille = %d\n", Ech.taille);
	}
	//else (printf("echeancier PLEIN"));
}

void Init_Ech(){
	event e;
	e.type = 0;
	e.date = 0;
	e.etat = 0;
	Ech.taille = 0;
	Ajouter_Ech(e);
}

event extraire(){//extrait l'event le plus ancien non traité
	int i, imin;
	event min;

	for (int i = 0; i < Ech.taille; ++i)
	{
		if (Ech.tab[i].etat == 0)
		{
			min = Ech.tab[i];
			imin = i;
			break;
		}
	}

	for (int i = 0; i < Ech.taille; ++i)
	{
		if (min.date > Ech.tab[i].date && Ech.tab[i].etat == 0)
		{
			min = Ech.tab[i];
			imin = i;
		}
	}

	Ech.tab[imin].etat = 1;
	return min;
}

int condition_arret(long double Old, long double New){
	if(fabs(Old-New) < EPSILON && temps > 1000){
		compteur ++;
		if (compteur > 1e3)
			return 1;
	}
	return 0;
}

/***************************************************************************************************************************************************************************************************/
/**
 * Modele 1
 *

#define N 10

void Arrivee_Client(event e){
	n++; //+1 client dans la file

	event e1;
	e1.type  = 0; //arrivée client
	e1.date = e.date + Exponnentielle(Lambda);
	e1.etat = 0; //non traité 

	Ajouter_Ech(e1);

	if(n<=N){
		event e2;
		e2.type = 1; //service
		e2.date = e.date + Exponnentielle(Mu);
		e2.etat = 0; //non traité
		Ajouter_Ech(e2);
			
	
	}
	temps = e.date;
}

void service_event(event e){
	if (n > 0){
		n--;
		if(n >= N){
			event e2;
			e2.type = 1; //service
			e2.date = e.date + Exponnentielle(Mu);
			e2.etat = 0;
			Ajouter_Ech(e2);
		}
	}
	temps = e.date;
	
}
	
void Modele_MMM(FILE* f1){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		cumule += (e.date-temps)*n;

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;

		if(temps == 0){
			fprintf(f1,"0 \t 0 \n");
		}
		else{
			printf("temps = %f et N = %ld et Nmoyen = %Lf \n",temps,n,Nmoyen);
			fprintf(f1,"%f \t %Lf \n",temps,Nmoyen);
		}

		if(e.type == 0){
			Arrivee_Client(e);
		}
		if (e.type == 1) {
			service_event(e);
		}
	}	
	printf("N moyen : %Lf\n", Nmoyen);
}
*/
/***************************************************************************************************************************************************************************************************/
/**
 * Modele 2
 */
void Arrivee_Client(event e){
	//n++; //+1 client dans la file
	double alea = (double)random()/RAND_MAX;	//entre 0 et 1
	event e1;
	e1.type  = 0; //arrivée client
	e1.date = e.date + Exponnentielle(Lambda);
	e1.etat = 0; //non traité 

	Ajouter_Ech(e1);

	if(alea < 0.1){
		if(n==1){
			event e2;
			e2.type = 1; //service
			e2.date = e.date + Exponnentielle(Mu);
			e2.etat = 0; //non traité
			Ajouter_Ech(e2);
		}
		n++;
	}
	temps = e.date;
}

void service_event(event e){
	if (n > 0){
		n--;
		if(n > 0){
			event e2;
			e2.type = 1; //service
			e2.date = e.date + Exponnentielle(Mu);
			e2.etat = 0;
			Ajouter_Ech(e2);
		}
	}
	temps = e.date;
	
}
	
void Modele_MM1_1(FILE* f1){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		cumule += (e.date-temps)*n;

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;
	
		
		if(temps == 0){
			fprintf(f1,"0 \t 0 \n");
		}
		else{
			printf("temps = %f et N = %ld et Nmoyen = %Lf \n",temps,n,Nmoyen);
			fprintf(f1,"%f \t %Lf \n",temps,Nmoyen);
		}

		if(e.type == 0){
			Arrivee_Client(e);
		}
		if (e.type == 1) {
			service_event(e);
		}
	}	
	printf("N moyen : %Lf\n", Nmoyen);
}

/***************************************************************************************************************************************************************************************************/

int main(){
	srandom(getpid() + time(NULL));

	/*FILE *f1 = fopen("MODELE1.data", "w");
	Modele_MMM(f1);
	fclose(f1);*/

	FILE *f2 = fopen("MODELE2.data","w");
	Modele_MM1_1(f2);
	fclose(f2);
	

	return 0;
}
