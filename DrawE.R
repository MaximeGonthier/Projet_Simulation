# pour executer le code : R CMD BATCH "DrawE.R"

modelname = "Result_modele1.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E = V2

plot(Lambda,E,type="l",xlab="Lambda",ylab="Temps moyen d'attente",col="red")

modelname = "Result_modele2.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E = V2

plot(Lambda,E,type="l",xlab="Lambda",ylab="E",col="red")
