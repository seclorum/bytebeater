all:	supersaw thxsnd wavegen

supersaw:	supersaw.c
	gcc supersaw.c -o bin/supersaw

supersaw_chord:	supersaw_chord.c
	gcc supersaw_chord.c -o bin/supersaw_chord

supersaw_test:	supersaw
	./bin/supersaw | sox -t raw -r 8000 -e unsigned-integer -b 8 -c 1 - -d

supersaw_chord_test:	supersaw_chord
	./bin/supersaw_chord | sox -t raw -r 8000 -e unsigned-integer -b 8 -c 1 - -d

wavegen:	wavegen.c
	gcc wavegen.c -o bin/wavegen -lm

wavegen_test:	wavegen
	./bin/wavegen | sox -t raw -r 8000 -e unsigned-integer -b 8 -c 1 - -d

thxsnd:	thxsnd.c
	gcc thxsnd.c -o bin/thxsnd -Wno-unsequenced

thxsnd_test:	thxsnd
	./bin/thxsnd | sox -t raw -r 8000 -e unsigned-integer -b 8 -c 1 - -d

clean:
	rm -rf bin/* *.?~
