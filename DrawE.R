# pour executer le code : R CMD BATCH "DrawE.R"

modelname = "resultE.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
E1 = V2
E2 = V3
E3 = V3

plot(Lambda,E1,xlim=c(0,9), ylim=c(0,16), type="l",xlab="Lambda",ylab="Temps moyen d'attente",col="red")
lines(Lambda,E2,type="l",col="blue")
lines(Lambda,E3,type="l",col="green")
legend("topleft",legend=c("E_modele1[A]", "E_modele2[A]", "E_modele3[A]"), col=c("red","blue","green"),lty=1.1,cex=0.8)

