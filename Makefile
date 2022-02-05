all: compile link run

compile:
	g++ -std=c++17 -c *.cpp -I "E:\Programy\C++\Fireworks\include" -DSFML_STATIC 

link:
	g++ *.o -o main -L "E:\Programy\C++\Fireworks\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopenaL32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lfreetype -lgdi32 -lopengl32 -lwinmm

clean:
	del *.o

run:
	.\main.exe