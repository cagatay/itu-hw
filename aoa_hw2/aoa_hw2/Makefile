#
#	Makefile for AoA HW2
#	Ali Cagatay Yuksel (040050220)
#

# Builds the encoder program, removes object files and runs the executable with sample input
# then decodes the encoded text with the given python program

test:	all clean
	./encoder input.txt
	python decode.py encoded.txt hcodes.txt decoded.txt

# Does the building job,

all:	main.o encoder.o
	g++ -o encoder main.o encoder.o

main.o:	
	g++ -c -o main.o main.cpp

encoder.o:
	g++ -c -o encoder.o encoder.cpp

# Removes object files

clean:
	rm -rf encoder.o main.o

# Removes object files and all outputs
cleanall:
	rm -rf encoded.txt hcodes.txt decoded.txt stats.txt freq.txt encoder main.o encoder.o
