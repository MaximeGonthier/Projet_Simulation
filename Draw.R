# pour executer le code : R CMD BATCH "DrawE.R"

model1 = "Result_modele1.txt"
data = read.table(model1)
attach(data)

Lambda = V1
E = V2
T = V3

model2 = "Result_modele2.txt"
data = read.table(model2)
attach(data)

Lambda2 = V1
E2 = V2
T2 = V3

plot(Lambda,E,type="l",xlab="Lambda",ylab="E[A]",col="red")
lines(Lambda,E2,type="l",col="blue")
legend("topleft",legend=c("E[A]1", "E[A]2"), col=c("red","blue"),lty=1.1,cex=0.8)
  

plot(Lambda,T,type="l",xlab="Lambda",ylab="t_90[A]",col="red")
lines(Lambda,T2,type="l",col="blue")
legend("topleft",legend=c("T_90[A]1", "T_90[A]2"), col=c("red","blue"),lty=1.1,cex=0.8)