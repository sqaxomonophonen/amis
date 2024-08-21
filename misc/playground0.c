#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

int8_t samples[1<<24];

int main(int argc, char** argv)
{
	#if 0
	const int N = 5000000;
	for (int t = 0; t < N; t++) {
		//samples[i] = (i&31) == 0 ? -128 : 127;
		//samples[i] = (i&127) < 1 ? -5 : 5;
		//10*(t>>6|t|t>>(t>>16))+(7&t>>11)
		// http://viznut.fi/demos/unix/bytebeat_formulas.txt
		 
		//const int x = t*((t>>5)|(t>>8))>>(t>>16);
		//const int x = 
		//printf("x[%d]=%d\n", i, x);
		//const int x = t*(42&t>>10);
		//const int x = 3*((t>>6)|t|(t>>(t>>16)))+(7&t>>11) - ((5*t) & (t>>9));
		const int t2 = t+200-15;
		const int t3 = t+11111;
		const int x = (((11111^t) | (t>>5)) | ((10034^t2)>>8) | (t>>1)) - 3*(t3 | (t3>>7) | (t2>>1));
		samples[t] = x;
	}
	#endif

	#if 0
	const int N = 5000000;
	int t1 = 112;
	int t2 = 0;
	for (int t0 = 0; t0 < N; t0++) {
		const int x = (((t2 ^ t1) & t2)) | ((t2+12000)>>8) | (t2>>4);
		const int y = (t2>>3);
		samples[t0] = x & y;
		t1 += (t0>>7);
		t2 += 5;
	}
	#endif

	#if 0
	const int N = 5000000;
	int t1 = 0;
	int t2 = 0;
	for (int t0 = 0; t0 < N; t0++) {
		const int t11 = (t1>>5) | (t1>>2) | (t2>>9);
		const int x = t11;
		samples[t0] = x;
		t1 += 6;
		t2 += 11;
	}
	#endif

	#if 0
	const int N = 5000000;
	int t1 = 0;
	int t2 = 0;
	for (int i = 0; i < N; i++) {
		const int x = ((t1^t2)>>4) | (t2>>11) | (t1>>11);
		samples[i] = x;
		t1 += 500;
		t2 += 1001;
	}
	#endif

	#if 0
	int t1=0,t2=0;
	const int N = 5000000;
	for (int i = 0; i < N; i++) {
		const int x = (t1>>9)|(t2>>9);
		samples[i] = x;
		//samples[i] = (x + (rand()&15)) >> 2;
		t1 += 120;
		t2 += 119;
	}
	#endif

	#if 0
	// cool one; also reimplemented in bytebeat.asm
	unsigned t1=0,t2=0;
	const int N = 5000000;
	for (int i = 0; i < N; i++) {
		const unsigned x = (t1&((t1<<5))) ? (t1>>13) : (t1>>2);
		samples[i] = x | ((((t1>>9)&7) & (t2>>7)) - (t1>>12));
		t1 += 77;
		t2 += 1064;
	}
	#endif


	FILE* f = fopen("playground0.raw", "wb");
	assert(fwrite(samples, 1, N, f) == N);
	fclose(f);

	return 0;
}
