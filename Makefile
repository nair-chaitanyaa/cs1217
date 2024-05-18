build:
	gcc -o assignment4 main.c semaphore.h semaphore.c -pthread
	./assignment4 > output.txt

clean:
	rm assignment4