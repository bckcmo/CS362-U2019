CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme.o: testme.c
	gcc -c testme.c -g  $(CFLAGS)

test: testme.o
	echo "Running Tests:" > results.out
	echo "randomtestcard1.c:" >> results.out
	gcc -o testme testme.o $(CFLAGS)
	./testme >> results.out
	gcov testme.c -b >> results.out

clean:
	rm -f *.o testme  *.gcov *.gcda *.gcno *.so *.out
