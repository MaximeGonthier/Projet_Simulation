# pour executer le code : R CMD BATCH "Draw90.R"


modelname = "result90.txt"
data = read.table(modelname)
attach(data);

Lambda = V1
t1 = V2
t2 = V3
t3 = V4

plot(Lambda,t1,xlim=c(0,9), ylim=c(0,29), type="l",xlab="Lambda",ylab="90ème percentile du temps d'attente",col="red")
lines(Lambda,t2,type="l",col="blue")
lines(Lambda,t3,type="l",col="green")
legend("topleft",legend=c("T90_modele1", "T90_modele2", "T90_modele3"), col=c("red","blue", "green"),lty=1.1,cex=0.8)

