#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "m68k.h"

#define RAM_SIZE (1<<21)

static struct {
	uint8_t ram[RAM_SIZE];
} g;

static inline void address_guard(unsigned address, int size)
{
	//assert((address & (size-1)) == 0);
	assert(0 <= address && address <= (RAM_SIZE-size));
}

unsigned m68k_read_memory_8(unsigned address)
{
	printf("read8 $%x\n", address);
	address_guard(address, 1);
	return g.ram[address];
}

unsigned m68k_read_memory_16(unsigned address)
{
	printf("read16 $%x\n", address);
	address_guard(address, 2);
	return (g.ram[address+0]<<8) | (g.ram[address+1]);
}

unsigned m68k_read_memory_32(unsigned address)
{
	printf("read32 $%x\n", address);
	address_guard(address, 4);
	return (g.ram[address]<<24) | (g.ram[address+1]<<16) | (g.ram[address+2]<<8) | (g.ram[address+3]);
}

#define HW0 0xddd000
#define HW1 (HW0+0xfff)

static inline int in_hw_range(unsigned address)
{
	return HW0 <= address && address <= HW1;
}

static inline void hw_write(unsigned address, int size, unsigned value)
{
	assert(in_hw_range(address));
	if (address == HW0) {
		// XXX this seems to be the only convenient way to "count
		// cycles", and it even works without stopping the program. I
		// tried using m68k_execute() and m68k_cycles_run(); they both
		// return the number of cycles executed, however, when using
		// the STOP instruction, execution stops, but the cycle budget
		// is exhausted anyway, so the value is close to the cycle
		// budget you passed to m68k_execute().
		printf("HW0 REQ AT CYCLE %d\n", m68k_cycles_run());
	}
}


void m68k_write_memory_8(unsigned address, unsigned value)
{
	printf("write8 $%x <= $%x\n", address, value);
	if (in_hw_range(address)) {
		hw_write(address, 1, value);
	} else {
		address_guard(address, 1);
		g.ram[address] = value;
	}
}

void m68k_write_memory_16(unsigned address, unsigned value)
{
	printf("write16 $%x <= $%x\n", address, value);
	address_guard(address, 2);
	if (in_hw_range(address)) {
		hw_write(address, 2, value);
	} else {
		g.ram[address] = value>>8;
		g.ram[address+1] = value;
	}
}

void m68k_write_memory_32(unsigned address, unsigned value)
{
	printf("write32 $%x <= $%x\n", address, value);
	address_guard(address, 4);
	if (in_hw_range(address)) {
		hw_write(address, 4, value);
	} else {
		g.ram[address] = value>>24;
		g.ram[address+1] = value>>16;
		g.ram[address+2] = value>>8;
		g.ram[address+3] = value;
	}
}

int main(int argc, char** argv)
{
	g.ram[1] = 2;    // SP = 0x20000
	g.ram[7] = 0x80; // PC = 0x80

	uint8_t* pc = &g.ram[0x80];

	*(pc++) = 0x24; // move.l  #$10000,a2
	*(pc++) = 0x7c;
	*(pc++) = 0x00;
	*(pc++) = 0x01;
	*(pc++) = 0x00;
	*(pc++) = 0x00;

	*(pc++) = 0x74; // moveq   #$42,d2
	*(pc++) = 0x42;

	*(pc++) = 0x72; // moveq   #3,d1
	*(pc++) = 0x03;

	// loop:
	*(pc++) = 0x14; // move.b  d2,(a2)+
	*(pc++) = 0xc2;

	*(pc++) = 0x51; // dbra d1, loop
	*(pc++) = 0xc9;
	*(pc++) = 0xff;
	*(pc++) = 0xfc;

	for (int i = 0; i < 3; i++) {
		*(pc++) = 0x26; // move.l  #$ddd000,a3
		*(pc++) = 0x7c;
		*(pc++) = 0x00;
		*(pc++) = 0xdd;
		*(pc++) = 0xd0;
		*(pc++) = 0x00;

		*(pc++) = 0x16; // move.b  #0,(a3)
		*(pc++) = 0xbc;
		*(pc++) = 0x00;
		*(pc++) = 0x00;
	}

	*(pc++) = 0x4e; // stop #0
	*(pc++) = 0x72;
	*(pc++) = 0x00;
	*(pc++) = 0x00;

	m68k_init();
	m68k_set_cpu_type(M68K_CPU_TYPE_68000);
	m68k_pulse_reset();
	m68k_execute(2000000000);

	assert(g.ram[0x0ffff] == 0);
	assert(g.ram[0x10000] == 0x42);
	assert(g.ram[0x10001] == 0x42);
	assert(g.ram[0x10002] == 0x42);
	assert(g.ram[0x10003] == 0x42);
	assert(g.ram[0x10004] == 0);

	return EXIT_SUCCESS;
}
