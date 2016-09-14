file="plots.pdf"

results = read.csv("real_parent.txt")

options(scipen=999)

pdf(file, height=4, width=4, bg="white")

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("Real parent")

results = read.csv("real_child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("Real child")


results = read.csv("real_parent&child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("Real parent+child")

results = read.csv("user_child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes (fork)", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("User child")

results = read.csv("user_parent.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("User parent")

results = read.csv("user_parent&child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("User parent+child")

results = read.csv("sys_parent.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("Sys parent")

results = read.csv("sys_child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("Sys child")

results = read.csv("sys_parent&child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("Sys parent+child")

results = read.csv("user&sys_parent.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("User+sys parent")

results = read.csv("user&sys_child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("User+sys child")

results = read.csv("user&sys_parent&child.txt")

options(scipen=999)

plot(results$n, results$fork, type="o", xlab="Number of processes", ylab="Time in seconds", ylim=range(0,max(results$fork,results$vfork,results$clonefork,results$clonevfork)), col="red")
lines(results$n, results$vfork, type="o", col="blue") 
lines(results$n, results$clonefork, type="o", col="green")
lines(results$n, results$clonevfork, type="o", col="yellow")
legend("topleft", c("fork","vfork","clonefork","clonevfork"),lty=c(1,1,1,1),lwd=c(2.5,2.5,2.5,2.5),col=c("red","blue","green","yellow"), cex=0.5)
title("User+sys parent+child")

dev.off()
