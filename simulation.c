#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
//-lm pour compiler
//#define Lambda 9
#define Mu 1

#define EPSILON 1e-5
#define MAXEVENT 100000	//taille max de l'echeancier
#define MAXTEMPS 10000	//cond d'arret
#define N 10//nombre de serveurs
//Maxtemps devrait correspondre a 1h d'apres l'énoncé

double temps = 0;
long int n = 0;		//nb de clients dans la file a l'instant temps
int compteur = 0;	//cond d'arret 2
double cumule = 0;
int Nentree = 0;
double Moy = 0;
double Per = 0;
double Tmoyen = 0;

void init_global(){
	temps = 0;
	n = 0;
	compteur = 0;
	cumule = 0;
	Nentree = 0;
	Moy = 0;
	Tmoyen = 0;
}
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
// if(n>N)Tcumule += (e.date-temps)*(n-N); Tmoyen = Tcumule/Nentree
double Moyenne(int serveur, event e){
	if(n > serveur){
		cumule += (e.date - temps) * (n - serveur);
		Tmoyen =(double) cumule/Nentree;
	}
	//printf("serveur = %d, n = %ld, Nentree = %d, cumule = %f, temps = %f, Tmoyen = %f\n",serveur, n, Nentree, cumule, temps, Tmoyen);
	return Tmoyen;
}

double* Percentile (double Tmoyen, double* Tabtemps){
	if(Tmoyen > Tabtemps[8]){
		//printf("supp\n");
		if(Tmoyen > Tabtemps[9]){
			Tabtemps[8] = Tabtemps[9];
			Tabtemps[9] = Tmoyen;
		}
		else {
			Tabtemps[8] = Tmoyen;
			for(int i = 1; i < 9; i++){
				Tabtemps[i-1] = Tabtemps[i];
			}
		}
	}
	else if (Tmoyen < Tabtemps[8]){
		if (Tmoyen > Tabtemps[7]){
			Tabtemps[8] = Tmoyen;
			for(int i = 1; i < 9; i++){
				Tabtemps[i-1] = Tabtemps[i];
			}
		}
		else {
			Tabtemps[8] = Tabtemps[7];
			for(int i = 1; i < 8; i++){
				Tabtemps[i-1] = Tabtemps[i];
			}
		}
	}
	return Tabtemps;
}

void Arrivee_Client(event e, int Lambda, int mod){
	event e1;
	e1.type  = 0; //arrivée client
	e1.date = e.date + Exponnentielle(Lambda);
	e1.etat = 0; //non traité 

	Ajouter_Ech(e1);

	if(mod == 2){
		double alea = (double)random()/RAND_MAX;	//entre 0 et 1
		if(alea < 0.1){
			if(n==1){
				event e2;
				e2.type = 1; //service
				e2.date = e.date + Exponnentielle(Mu);
				e2.etat = 0; //non traité
				Ajouter_Ech(e2);
			}
			n++;
			Nentree ++;
		}
		temps = e.date;
	}
	else if(mod == 1){
		n++; //+1 client dans la file
		Nentree ++;
		if(n<=N){
			event e2;
			e2.type = 1; //service
			e2.date = e.date + Exponnentielle(Mu);
			e2.etat = 0; //non traité
			Ajouter_Ech(e2);
		}
		temps = e.date;
	}
}
void service_event(event e, int mod){
	if (n > 0){
		n--;
		if(mod == 2){
			if(n > 0){
				event e2;
				e2.type = 1; //service
				e2.date = e.date + Exponnentielle(Mu);
				e2.etat = 0;
				Ajouter_Ech(e2);
			}
		}
		if(mod == 1){
			if(n >= N){
				event e2;
				e2.type = 1; //service
				e2.date = e.date + Exponnentielle(Mu);
				e2.etat = 0;
				Ajouter_Ech(e2);
			}
		}
	}
	temps = e.date;
}
/***************************************************************************************************************************************************************************************************/
/**
 * Modele 1
 */
void Modele_1(FILE* f1, int Lambda){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	double* Tabpercentile = (double*)malloc(10*sizeof(double*));
	for (int i = 0; i < 10; i++){
		Tabpercentile[i] = 0;
	}

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		cumule += (e.date-temps)*n;

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;

		if(temps == 0){
			fprintf(f1,"0 \t 0 \n");
		}
		else{
			//printf("temps = %f et N = %ld et Nmoyen = %Lf \n",temps,n,Nmoyen);
			fprintf(f1,"%f \t %Lf \n",temps,Nmoyen);
		}

		if(e.type == 0){
			Arrivee_Client(e,Lambda,1);
		}
		if (e.type == 1) {
			service_event(e,1);
		}
		Moy= Moyenne(10,e);
		Tabpercentile = Percentile(Moy,Tabpercentile);
		//printf("tabperc : %f\n",Tabpercentile[8]);
	}	
	printf("Lambda : %d N moyen : %Lf  T moyen  : %f 90 percentile : %f\n",Lambda, Nmoyen, Moy,Tabpercentile[8]);
	FILE *fresult1 = fopen("Result_modele1.txt","a");
	fprintf(fresult1,"%d \t %f \t %f \n",Lambda,Moy,Tabpercentile[8]);
	fclose(fresult1);
	free(Tabpercentile);
}
/***************************************************************************************************************************************************************************************************/
/**
 * Modele 2
 */
void Modele_2(FILE* f1, int Lambda){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	double* Tabpercentile2 = (double*)malloc(10*sizeof(double*));
	for (int i = 0; i < 10; i++){
		Tabpercentile2[i] = 0;
	}

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		cumule += (e.date-temps)*n;

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;
		
		
		if(temps == 0){
			fprintf(f1,"0 \t 0 \n");
		}
		else{
			//printf("temps = %f et n = %ld et Nmoyen = %Lf \n",temps,n,Nmoyen);
			fprintf(f1,"%f \t %Lf \n",temps,Nmoyen);
		}

		if(e.type == 0){
			Arrivee_Client(e,Lambda,2);
		}
		if (e.type == 1) {
			service_event(e,2);
		}
		Moy= Moyenne(1,e);
		Tabpercentile2 = Percentile(Moy,Tabpercentile2);
	}	
	printf("Lambda : %d N moyen : %Lf  T moyen  : %f 90 percentile : %f\n",Lambda, Nmoyen, Moy,Tabpercentile2[8]);
	FILE *fresult2 = fopen("Result_modele2.txt","a");
	fprintf(fresult2,"%d \t %f \t %f \n",Lambda,Moy,Tabpercentile2[8]);
	fclose(fresult2);
	free(Tabpercentile2);
}
/***************************************************************************************************************************************************************************************************/

int main(int argc, char **argv){

	srandom(getpid() + time(NULL));
	FILE *f = fopen(argv[1],"r");
	int Lambda;
	
	init_global();
	for(int i = 0; i < 9; i++)
	{
		fscanf(f,"%d",&Lambda);
		FILE *f1 = fopen("MODELE1.data","w");
		FILE *f2 = fopen("MODELE2.data","w");
		//printf("Modèle 1 : \n");
		Modele_1(f1, Lambda);
		init_global();
		//printf("Modèle 2 : \n");
		Modele_2(f2, Lambda);
		init_global();
		fclose(f1);
		fclose(f2);	
		sleep(1);
	}
	fclose(f);

	return 0;
}

