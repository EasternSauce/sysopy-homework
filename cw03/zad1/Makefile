all: proc procplot real_parent.txt wyniki.pdf

proc:
	gcc -std=gnu99 proc.c -o proc

procplot:
	gcc -std=gnu99 procplot.c -o procplot

real_parent.txt:
	./procplot

wyniki.pdf:
	Rscript plot.r




.PHONY: clean

clean:
	rm *.txt proc procplot plots.pdf
