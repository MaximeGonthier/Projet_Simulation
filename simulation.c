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

///////////////////////////////VARIBALES GLOBALES UTILISEES POUR LES SIMULATION///////////////////////////
double temps = 0;	//temps de simulation
long int n = 0;		//nb de clients dans la file a l'instant temps
int compteur = 0;	//cond d'arret 2
double cumule = 0;	//cumule des temps passées par les clients dans la simulation
int Nentree = 0;	//Nombre total d'entrée par simulation
double Moy = 0;		//Temps d'attente moyen par simulation
double Per = 0;		//90 percentile du temps d'attente par simulation
double Tmoyen = 0;	//Temps d'attente moyen par simulation
double Tabmodele3[10]; //Tableau représentant le nombre de client pour chaque serveur du modèle 3

/**
 * Initialisation des différentes variables globales pour les simulations
 */
void init_global(){
	temps = 0;
	n = 0;
	compteur = 0;
	cumule = 0;
	Nentree = 0;
	Moy = 0;
	Tmoyen = 0;
	//Tabmodele3 = (double*)malloc(10*sizeof(double*));
	for (int i = 0; i < 10; i++){
		Tabmodele3[i] = 0;
	}
}

/**
 * Structure représentant un événement de la simulation
 */
typedef struct Event {
	int type; //0 pour arrive 1 pour sortie
	double date; 
	int etat; //0 pour non traité 1 pour non traité
	int file; //exclusif au modèle 3,1 à 10 pour le modèle 3, -1 pour les autres modèles
}event;

/**
 * Structure contenant les évènement
 */
typedef struct Echeancier {
	event tab[MAXEVENT];
	int taille;
}echeancier;
echeancier Ech;

/**
 * Loi exponnentielle permettant de calculer le temps interarrivé et le temps de service
 */
double Exponnentielle(int lbda) {
	double r = (double)random()/RAND_MAX;	//entre 0 et 1
	
	while(r==0 || r==1) {	//tant que x vaut 0 ou 1 on refait un random
		r = (double)random()/RAND_MAX;
	}
	return -log(r)/(lbda*1.0); // - log(u)/lamda, avec U = unif(0,1)
}

/**
 * Fonction permettant d'ajouter un évènement à l'echeancier
 */
void Ajouter_Ech(event e) {
	if(Ech.taille < MAXEVENT) {
		Ech.tab[Ech.taille] = e;
		Ech.taille++;
	}
	else (printf("echeancier PLEIN"));
}

/**
 * Initialisation de l'échéancier pour la simulation
 */
void Init_Ech(){
	event e;
	e.type = 0;
	e.date = 0;
	e.etat = 0;
	e.file = -1;
	Ech.taille = 0;
	Ajouter_Ech(e);
}

/**
 * extrait l'event le plus ancien non traité
 */
event extraire(){
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

/**
 * condition d'arrêt de la simulation
 */
int condition_arret(long double Old, long double New){
	if(fabs(Old-New) < EPSILON && temps > 1000){
		compteur ++;
		if (compteur > 1e3)
			return 1;
	}
	return 0;
}

/**
 * Calcul du temps d'attente moyen par simulation
 * le paramètre serveur permet de calculer la moyenne pour une M/M/1 ou une M/M/N
 */
double Moyenne(int serveur, event e){
	if(n > serveur){
		cumule += (e.date - temps) * (n - serveur);
		Tmoyen =(double) cumule/Nentree;
	}
	return Tmoyen;
}

int factorielle(int n){
	int resultat;
    if(n<=0)
     return 1;
    else{
     for(resultat =1; n > 1; n--)
        resultat *= n;
    }
   return resultat;
}

/**
 * Calcul du temps moyen d'attente pour une M/M/N
 */
double Moyenne_theorique1(int lambda){
	double rho = (double)lambda/(10*Mu);
	double SumN, nomSumN, denumSumN;
	for(int i = 1; i < 10; i ++){
		nomSumN = (double)pow(10*rho,i);
		denumSumN = factorielle(i);
		SumN += (double)nomSumN/denumSumN;
	}
	double p0, nump0, denump0;
	nump0 = (double)pow(10*rho,10);
	denump0 = (double)factorielle(10) * (1 - rho);
	p0 = 1 + ((double)nump0/denump0) + SumN;
	p0 = (double)pow(p0,-1);

	double ProbaQ, nomProbaQ, denumProbaQ;
	nomProbaQ = nump0;
	denumProbaQ = denump0;
	ProbaQ = ((double)nomProbaQ/denumProbaQ) * p0;

	double moy = (double)ProbaQ/(10*Mu*(1 -rho));
	return moy;
}

/**
 * Calcul du temps moyen d'attente pour une M/M/1
 * On multiplie le resultat par 10 pour pouvoir comparer avec notre second modèle
 */
double Moyenne_theorique2(int lambda){
	double rho = (double)lambda/10;
	double nom = (double)1/10;
	double denom = (1-rho);
	double moy = (double)rho*(double)nom/denom;
	return moy*10;
}
/**
 * calcul du 90 percentile du temps d'attente par simulation
 */
double* Percentile (double Tmoyen, double* Tabtemps){
	if(Tmoyen > Tabtemps[8]){
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

/**
 * Fonction gérant l'arrivée des clients
 */
void Arrivee_Client(event e, int Lambda, int mod){
	event e1;
	e1.type  = 0; //arrivée client
	e1.date = e.date + Exponnentielle(Lambda);
	e1.etat = 0; //non traité 
	e1.file = -1; //Inutile si on est pas dans le modèle 3

	Ajouter_Ech(e1);

	/**
	 * Arrivée client pour le modèle 1
	 */
	if(mod == 1){
		n++; //+1 client dans la file
		Nentree ++; //+1 client dans le total des clients
		if(n<=N){
			event e2;
			e2.type = 1; //service
			e2.date = e.date + Exponnentielle(Mu);
			e2.etat = 0; //non traité
			Ajouter_Ech(e2);
		}
		temps = e.date;
	}
	/**
	 * Arrivée client pour le modèle 2
	 */
	else if(mod == 2){
		double alea = (double)random()/RAND_MAX;//entre 0 et 1
		if(alea < 0.1){ //représente la probabilité que le client entre bien dans la file
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
	/**
	 * Arrivée client pour le modèle 3
	 */
	else if(mod == 3){
		int min = 0;
		min = Tabmodele3[0];
		int indicemin = 0;
			//choix de la file avec le plus petit nombre de clients
			for (int i = 0; i < 10; i++)
			{
				if(min > Tabmodele3[i])
				{
					min = Tabmodele3[i];
					indicemin = i;
				}
			}
		Tabmodele3[indicemin]+=1;//ajout d'un client dans cette file
		if(Tabmodele3[indicemin]==1){
			event e2;
			e2.type = 1; //service
			e2.date = e.date + Exponnentielle(Mu);
			e2.etat = 0; //non traité
			e2.file = indicemin; //on indique dans quelle file on va devoir traiter le service
			Ajouter_Ech(e2);
		}
		n++;
		Nentree ++;
		temps = e.date;
	}
}

/**
 * Fonction gérant le service des clients
 */ 
void service_event(event e, int mod){
	if (n > 0){
		n--;
		/**
		 * service du modèle 1
		 */
		if(mod == 1){
			if(n >= N){//si le nombre de client est superieur au nombre de serveur, on lance un service
				event e2;
				e2.type = 1; //service
				e2.date = e.date + Exponnentielle(Mu);
				e2.etat = 0;
				e2.file = -1;
				Ajouter_Ech(e2);
			}
		}

		/**
		 * service du modèle 2
		 */
		if(mod == 2){
			if(n > 1){//si le nombre de client est positif, on lance un service
				event e2;
				e2.type = 1; //service
				e2.date = e.date + Exponnentielle(Mu);
				e2.etat = 0;
				e2.file = -1;
				Ajouter_Ech(e2);
			}
		}

		/**
		 * service du modèle 3
		 */
		if(mod == 3){
			Tabmodele3[e.file] --; //on enlève un client de la file concernée par le service
			if(Tabmodele3[e.file] > 0){ //si le nombre de client dans cette file est positif, on lance un service
				event e2;
				e2.type = 1; //service
				e2.date = e.date + Exponnentielle(Mu);
				e2.etat = 0;
				e2.file = e.file;
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
void Modele_1(int Lambda){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	/**
	 * Initialisation du tableau permettant d'obtenir le 90 percentile du temps d'attente
	 */
	double* Tabpercentile = (double*)malloc(10*sizeof(double*));
	for (int i = 0; i < 10; i++){
		Tabpercentile[i] = 0;
	}

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		cumule += (e.date-temps)*n;

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;

		if(e.type == 0){
			Arrivee_Client(e,Lambda,1);
		}
		if (e.type == 1) {
			service_event(e,1);
		}
		Moy= Moyenne(10,e);
		Tabpercentile = Percentile(Moy,Tabpercentile);
	}	

	//Ecriture des résultats dans un fichier
	//printf("Lambda : %d N moyen : %Lf  T moyen  : %f 90 percentile : %f\n",Lambda, Nmoyen, Moy,Tabpercentile[8]);
	FILE *fresult1 = fopen("Result_modele1.txt","a");
	fprintf(fresult1,"%d \t %f \t %f \n",Lambda,Moy,Tabpercentile[8]);

	//Ecriture de la moyenne et du 90percentile dans un fichier communs aux trois simulations pour le tracage des courbes
	FILE *resultE = fopen("resultE.txt","a");
	fprintf(resultE,"%d \t %f",Lambda, Moy);
	fclose(resultE);

	FILE *result90 = fopen("result90.txt","a");
	fprintf(result90,"%d \t %f",Lambda, Tabpercentile[8]);
	fclose(result90);

	fclose(fresult1);
	free(Tabpercentile);
}
/***************************************************************************************************************************************************************************************************/
/**
 * Modele 2
 */
void Modele_2(int Lambda){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	/**
	 * Initialisation du tableau permettant d'obtenir le 90 percentile du temps d'attente
	 */
	double* Tabpercentile2 = (double*)malloc(10*sizeof(double*));
	for (int i = 0; i < 10; i++){
		Tabpercentile2[i] = 0;
	}

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		//Ici si on a 1 client ou moins cela veut dire qu'un client est forcément en train d'être servi
		//donc on ne le compte pas dans les clients en attente dans la file d'attente, d'où le if(n>1)
		if (n>1) cumule += (n-1)*(e.date-temps);

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;
		

		if(e.type == 0){
			Arrivee_Client(e,Lambda,2);
		}
		if (e.type == 1) {
			service_event(e,2);
		}
		Moy= Moyenne(1,e);
		Tabpercentile2 = Percentile(Moy,Tabpercentile2);
	}	

	//Ecriture des résultats dans un fichier
	//printf("Lambda : %d N moyen : %Lf  T moyen  : %f 90 percentile : %f\n",Lambda, Nmoyen, Moy,Tabpercentile2[8]);
	FILE *fresult2 = fopen("Result_modele2.txt","a");
	fprintf(fresult2,"%d \t %f \t %f \n",Lambda,Moy,Tabpercentile2[8]);

	//Ecriture de la moyenne et du 90percentile dans un fichier communs aux trois simulations pour le tracage des courbes
	FILE *resultE = fopen("resultE.txt","a");
	fprintf(resultE," \t %f",Moy);
	fclose(resultE);

	FILE *result90 = fopen("result90.txt","a");
	fprintf(result90," \t %f",Tabpercentile2[8]);
	fclose(result90);

	fclose(fresult2);
	free(Tabpercentile2);
}
/***************************************************************************************************************************************************************************************************/

/***************************************************************************************************************************************************************************************************/
/**
 * Modele 3
 */
void Modele_3(int Lambda){
	long double OldNmoyen;
	long double Nmoyen;
	Init_Ech();
	event e;

	/**
	 * Initialisation du tableau permettant d'obtenir le 90 percentile du temps d'attente
	 */
	double* Tabpercentile3 = (double*)malloc(10*sizeof(double*));
	for (int i = 0; i < 10; i++){
		Tabpercentile3[i] = 0;
	}

	while(condition_arret(OldNmoyen, Nmoyen) == 0){
		e = extraire();
		cumule += (e.date-temps)*n;

		OldNmoyen = Nmoyen;
		Nmoyen = cumule/temps;
	

		if(e.type == 0){
			Arrivee_Client(e,Lambda,3);
		}
		if (e.type == 1) {
			service_event(e,3);
		}
		Moy= Moyenne(10,e);
		Tabpercentile3 = Percentile(Moy,Tabpercentile3);
	}	

	//Ecriture des résultats dans un fichier
	//printf("Lambda : %d N moyen : %Lf  T moyen  : %f 90 percentile : %f\n",Lambda, Nmoyen, Moy,Tabpercentile3[8]);
	FILE *fresult3 = fopen("Result_modele3.txt","a");
	fprintf(fresult3,"%d \t %f \t %f \n",Lambda,Moy,Tabpercentile3[8]);

	//Ecriture de la moyenne et du 90percentile dans un fichier communs aux trois simulations pour le tracage des courbes
	double m1 = Moyenne_theorique1(Lambda);
	double m2 = Moyenne_theorique2(Lambda);
	FILE *resultE = fopen("resultE.txt","a");
	fprintf(resultE," \t %f \t %f \t %f\n",Moy, m1, m2);
	fclose(resultE);

	FILE *result90 = fopen("result90.txt","a");
	fprintf(result90," \t %f\n",Tabpercentile3[8]);
	fclose(result90);

	fclose(fresult3);

	free(Tabpercentile3);

}
/***************************************************************************************************************************************************************************************************/

int main(int argc, char **argv){

	srandom(getpid() + time(NULL));
	/**
	 * ouverture du fichier contenant les différentes valeurs de lambda que l'on veut tester
	 */
	FILE *f = fopen(argv[1],"r");
	int Lambda;

	/**
	 * Ouverture des fichier de résultat afin de les effacer s'ils sont rempli
	 */
	FILE *fresult1 = fopen("Result_modele1.txt","w");
	fclose(fresult1);
	FILE *fresult2 = fopen("Result_modele2.txt","w");
	fclose(fresult2);
	FILE *fresult3 = fopen("Result_modele3.txt","w");
	fclose(fresult3);

	FILE *result90 = fopen("result90.txt","w");
	fclose(result90);
	FILE *resultE = fopen("resultE.txt","w");
	fclose(resultE);
	
	init_global();
	for(int i = 0; i < 9; i++)
	{
		fscanf(f,"%d",&Lambda);
		
		Modele_1(Lambda);
		init_global();
		
		Modele_2(Lambda);
		init_global();

		Modele_3(Lambda);
		init_global();
	}
	fclose(f);

	return 0;
}
