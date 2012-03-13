#  Executables
EX=ex01 ex03 ex04 ex05 ex09 ex10 ex11 ex12 ex13 ex14 ex15 ex16 ex17 ex18 ex19 ex20 ex22 ex23 ex24 ex25 ex26

#  Libraries - Linux
LIBS=-lglut -lGLU
#  Libraries - OSX
#LIBS=-framework GLUT -framework OpenGL
#  Libraries - MinGW
#LIBS=-lglut32cu -lglu32 -lopengl32

#  Main target
all: $(EX)

#  Generic compile rules
.c.o:
	gcc -c -O3 -Wall $<
.cpp.o:
	g++ -c -O3 -Wall $<

#  Generic compile and link
%: %.c CSCIx239.a
	gcc -Wall -O3 -o $@ $^ $(LIBS)

#  Delete
clean:
	rm -f $(EX) *.o *.a

#  Create archive (include glWindowPos if you need it)
CSCIx239.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o fps.o elapsed.o shader.o noise.o
	ar -rcs CSCIx239.a $^

#  C++ Example
ex01:ex01.o Object.o Sphere.o Cube.o WaveOBJ.o CSCIx239.a
	g++ -Wall -O3 -o $@ $^ $(LIBS)
ex01.o:    ex01.cpp    Object.h  Cube.h Sphere.h WaveOBJ.h
Object.o:  Object.cpp  Object.h
Cube.o:    Cube.cpp    Cube.h    Object.h
Sphere.o:  Sphere.cpp  Sphere.h  Object.h
WaveOBJ.o: WaveOBJ.cpp WaveOBJ.h Object.h Vec.h

#  OpenCV
ex13: ex13.o CSCIx239.a; gcc -Wall -O3 -o $@ $^ -lhighgui -lcv $(LIBS)
#  OpenMP
ex20:ex20.c CSCIx239.a;  gcc -fopenmp -Wall -o $@ $^ $(LIBS)
#  CUDA
ex22:ex22.cu;            nvcc -o $@ $^ -L/usr/lib/nvidia-current -lcuda
#  OpenCL
ex23:ex23.cpp;           g++ -Wall -o $@ -I/usr/local/cuda/include $^ -lOpenCL

#  Ray traced spheres
ex24: ex24.cpp RaySphere.cpp;g++ -fopenmp -Wall -O3 -o $@ $^ $(LIBS)
#  Mandelbulb OpenMP
ex25:ex25.cpp;               g++ -fopenmp -Wall -O3 -o $@ $^ $(LIBS)
#  Mandelbulb CUDA
ex26:ex26.cu; nvcc -o $@ $< -L/usr/lib/nvidia-current -lcuda $(LIBS)

#  ZIP files
zip:
	zipc ex01.zip ex01.cpp Object.cpp Sphere.cpp Cube.cpp WaveOBJ.cpp
	zipc ex03
	zipc ex04
	zipc ex05
	zipc ex09
	zipc ex10
	zipc ex11
	zipc ex12
	zipc ex13
	zipc ex14
	zipc ex15
	zipc ex16
	zipc ex17
	zipc ex18
	zipc ex19
	zipc ex20
	zipc ex22.zip ex22.cu
	zipc ex23.zip ex23.cpp
	zipc ex24.zip ex24.cpp RaySphere.cpp
	zipc ex25.zip ex25.cpp
	zipc ex26.zip ex26.cu
	zipc CSCIx239.zip CSCIx239.h fatal.c loadtexbmp.c print.c project.c errcheck.c object.c fps.c elapsed.c shader.c noise.c Makefile

#  Obligatory UNIX inside joke
love:
	@echo "not war?"
