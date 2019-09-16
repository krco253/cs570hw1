all: load change print query

load: load.c header.h
	gcc -g -o load load.c header.c
change: change.c header.h
	gcc -g -o change change.c header.c
query: query.c header.h
	gcc -g -o query query.c header.c
print:  print.c header.h
	gcc -g -o print print.c header.c
clean:
	rm -f create  change  *~ core
