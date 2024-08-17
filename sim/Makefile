all: sim

m68kmake: m68kmake.c

m68kops.c: m68kmake
	./m68kmake

objs=sim.o m68kops.o m68kcpu.o softfloat/softfloat.o
sim: $(objs)
	$(CC) $(objs) -o sim -lm

clean:
	rm -f *.o m68kmake m68kops.c m68kops.h sim

