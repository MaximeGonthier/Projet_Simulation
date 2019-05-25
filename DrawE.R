# pour executer le code : R CMD BATCH "DrawE.R"

modelname = "Result_modele1.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E = V2
T = V3

plot(Lambda,E,type="l",xlab="Lambda",ylab="Temps moyen d'attente",col="red")
lines(Lambda,T,type="l",col="blue")
legend("topleft",legend=c("E[A]", "T_90[A]"), col=c("red","blue"),lty=1.1,cex=0.8)

modelname = "Result_modele2.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E = V2
T = V3

plot(Lambda,E,type="l",xlab="Lambda",ylab="E",col="red")
lines(Lambda,T,type="l",col="blue")
legend("topleft",legend=c("E[A]", "T_90[A]"), col=c("red","blue"),lty=1.1,cex=0.8)