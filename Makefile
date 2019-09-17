all: load change print query Clean

load: load.c header.h
	gcc -g -o load load.c header.c
change: change.c header.h
	gcc -g -o change change.c header.c
query: query.c header.h
	gcc -g -o query query.c header.c
print:  print.c header.h
	gcc -g -o print print.c header.c
Clean: clean.c header.h
	gcc -g -o clean clean.c header.c
clean:
	rm -f load query print  change  *~ core
