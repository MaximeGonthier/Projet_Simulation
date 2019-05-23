# pour executer le code : R CMD BATCH "Draw90.R"

modelname = "Result_modele1.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E = V3

plot(Lambda,E,type="l",xlab="Lambda",ylab="90 percentile",col="red")

modelname = "Result_modele2.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E = V3

plot(Lambda,E,type="l",xlab="Lambda",ylab="90 percentile",col="red")
