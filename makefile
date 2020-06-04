all: hdispersa pdispersa

hdispersa: hdispersa.c hilos.c Util.c
	gcc -o hdispersa hdispersa.c hilos.c Util.c -I. -lpthread

pdispersa: pdispersa.c Procesos.c Util.c
	gcc -o pdispersa pdispersa.c Procesos.c Util.c -I.
