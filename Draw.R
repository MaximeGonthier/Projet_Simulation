# pour executer le code : R CMD BATCH "DrawE.R"

model1 = "result90.txt"
data = read.table(model1)
attach(data)

Lambda = V1
T1 = V2
T2 = V3

plot(Lambda,T2,type="l",xlab="Lambda",ylab="t_90[A]",col="red")
lines(Lambda,T1,type="l",col="blue")
legend("topleft",legend=c("T_90[A]2", "T_90[A]1"), col=c("red","blue"),lty=1.1,cex=0.8)

model2 = "resultE.txt"
data = read.table(model2)
attach(data)

Lambda = V1
E1 = V2
E2 = V3

plot(Lambda,E2,type="l",xlab="Lambda",ylab="E[A]",col="red")
lines(Lambda,E1,type="l",col="blue")
legend("topleft",legend=c("E[A]2", "E[A]1"), col=c("red","blue"),lty=1.1,cex=0.8)
  

