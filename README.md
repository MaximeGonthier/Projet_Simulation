Projet_Simulation
=================

_Par : Maxime GONTHIER et Benjamin GUILLOT_

*********
Commandes
---------
*********
Depuis le dossier Projet_Simulation
- Lancement de l'application : ./script
Cela compile et lance l'application. En revanche un seul graphique sera généré en sortie.
- Pour obtenir le graphique des temps moyen d'attentes : R CMD BATCH "DrawE.R"
- Pour obtenir le graphique du 90ème percentile : R CMD BATCH "Draw90.R"

********
Fichiers
--------
********
simulation.c contient le code.
Dans le dossier rapport : rapport.pdf est le rapport, Theorie.pdf est l'explication
des modèles théoriques.

******************
Fichiers de sortie
------------------
******************
Les fichiers Result_modeleX.txt contiennent dans la première colonne lambda,
dans la deuxième le temps moyen d'attente E[A]et dans la dernière le 90ème
percentile du temps moyen d'attente.
resultE.txt et result90.txt contiennent respectiveemnt les temps d'attente moyen et les 90ème percentiles
de chaque modèle. Nous les utilisons pour tracer les graphes.
