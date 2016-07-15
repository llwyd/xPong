xPong: xPong.o
	gcc -std=c11 xPong.o -o xPong -lX11
xPong.o: xPong.c
	gcc -std=c11 -c xPong.c -o xPong.o -lX11
rm:
	rm xPong.o xPong