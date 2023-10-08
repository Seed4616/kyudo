all:	kyudo main doc

kyudo:	kyudo.c kyudo.h
	gcc -c -o kyudo.o kyudo.c -pthread

main:	kyudo.o main.c
	gcc -o main main.c kyudo.o -pthread

doc:
	doxygen kyudo.h kyudo.c main.c

clean:
	rm *.o main kyudo
