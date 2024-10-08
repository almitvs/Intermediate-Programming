CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra

# Links together files needed to create a project executable
project: project.o ppm_io.o image_manip.o
	$(CC) -o project project.o ppm_io.o image_manip.o -lm

# Compiles project.c to create project.o
project.o: project.c ppm_io.h image_manip.h
	$(CC) $(CFLAGS) -c project.c

# Compiles ppm_io.c to create ppm_io.o                                                                                                                                                                       
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

# Compiles image_manip.c to create image_manip.o                                                                                              
image_manip.o: image_manip.c image_manip.h ppm_io.h
	$(CC) $(CFLAGS) -c image_manip.c -lm

# Removes all objects and executables
clean:
	rm -f *.o project
