# pour executer le code : R CMD BATCH "DrawE.R"

model1 = "result90.txt"
data = read.table(model1)
attach(data)

T1 = V1
T2 = V2

model2 = "resultE.txt"
data = read.table(model2)
attach(data)

E1 = V1
E2 = V2

plot(Lambda,E1,type="l",xlab="Lambda",ylab="E[A]",col="red")
lines(Lambda,E2,type="l",col="blue")
legend("topleft",legend=c("E[A]1", "E[A]2"), col=c("red","blue"),lty=1.1,cex=0.8)
  

plot(Lambda,T1,type="l",xlab="Lambda",ylab="t_90[A]",col="red")
lines(Lambda,T2,type="l",col="blue")
legend("topleft",legend=c("T_90[A]1", "T_90[A]2"), col=c("red","blue"),lty=1.1,cex=0.8)