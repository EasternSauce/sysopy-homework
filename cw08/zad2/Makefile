all: main zerodiv


main: main.c
	gcc -O2 -Wall -DI -DSIG=SIGUSR1  main.c -o u1 -lpthread
	gcc -O2 -Wall -DI -DSIG=SIGTERM  main.c -o t1 -lpthread
	gcc -O2 -Wall -DI -DSIG=SIGKILL  main.c -o k1 -lpthread
	gcc -O2 -Wall -DI -DSIG=SIGSTOP  main.c -o s1 -lpthread
	gcc -O2 -Wall -DII -DSIG=SIGUSR1  main.c -o u2 -lpthread
	gcc -O2 -Wall -DII -DSIG=SIGTERM  main.c -o t2 -lpthread
	gcc -O2 -Wall -DII -DSIG=SIGKILL  main.c -o k2 -lpthread
	gcc -O2 -Wall -DII -DSIG=SIGSTOP  main.c -o s2 -lpthread
	gcc -O2 -Wall -DIII -DSIG=SIGUSR1  main.c -o u3 -lpthread
	gcc -O2 -Wall -DIII -DSIG=SIGTERM  main.c -o t3 -lpthread
	gcc -O2 -Wall -DIII -DSIG=SIGKILL  main.c -o k3 -lpthread
	gcc -O2 -Wall -DIII -DSIG=SIGSTOP  main.c -o s3 -lpthread
	gcc -O2 -Wall -DIV -DSIG=SIGUSR1  main.c -o u4 -lpthread
	gcc -O2 -Wall -DIV -DSIG=SIGTERM  main.c -o t4 -lpthread
	gcc -O2 -Wall -DIV -DSIG=SIGKILL  main.c -o k4 -lpthread
	gcc -O2 -Wall -DIV -DSIG=SIGSTOP  main.c -o s4 -lpthread
	gcc -O2 -Wall -DV -DSIG=SIGUSR1  main.c -o u5 -lpthread
	gcc -O2 -Wall -DV -DSIG=SIGTERM  main.c -o t5 -lpthread
	gcc -O2 -Wall -DV -DSIG=SIGKILL  main.c -o k5 -lpthread
	gcc -O2 -Wall -DV -DSIG=SIGSTOP  main.c -o s5 -lpthread

zerodiv: zerodiv.c
	gcc -O2 -Wall zerodiv.c -o zerodiv -lpthread

clean:
	rm u1 u2 u3 u4 u5 t1 t2 t3 t4 t5 k1 k2 k3 k4 k5 s1 s2 s3 s4 s5 zerodiv
