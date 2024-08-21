// cc -O0 -g -Wall drums_compress.c -o drums_compress -lm && ./drums_compress
// sox -r 20800 -e signed -b 8 -c 1 drums_000.raw -r 48000 -b 16 x.wav
// sox -r 20800 -e signed -b 8 -c 1 drums_000.raw -r 48000 -b 16 x.wav
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define CHUNK_SIZE_LOG2 (4)

static inline int8_t s8(int v)
{
	return v < -128 ? -128 : v > 127 ? 127 : v;
}


static void rawrite(const char* filename, int n_samples, int8_t* samples)
{
	FILE* f = fopen(filename, "wb");
	assert(f);
	assert(fwrite(samples, 1, n_samples, f) == n_samples);
	fclose(f);
}

static inline void minmax(int val, int is_first, int* pmin, int* pmax)
{
	if (is_first) {
		*pmin = *pmax = val;
	} else {
		if (val < *pmin) *pmin = val;
		if (val > *pmax) *pmax = val;
	}
}

static int repminmap(const char* what, int vmin, int vmax)
{
	const int n = vmax-vmin+1;
	const double nlog2 = log2(n);
	printf(" %s=[%d,%d] n=%d nlog2=%f ", what, vmin, vmax, n, nlog2);
	return (int)ceil(nlog2);
}

int main(int argc, char** argv)
{
	FILE* f = fopen("drums.raw", "rb");
	assert(f && "could not open sample file");
	assert(fseek(f, 0, SEEK_END) == 0);
	const int n_samples = (int)ftell(f);
	assert(fseek(f, 0, SEEK_SET) == 0);
	int8_t* samples = malloc(n_samples);
	assert(fread(samples, 1, n_samples, f) == n_samples);
	fclose(f);

	int8_t* o0 = malloc(n_samples);
	int8_t* o1 = malloc(n_samples);

	printf("n_samples = %d\n", n_samples);
	int prev = 0, dprev = 0;
	int prev_chunk = -1;
	int smin,smax,dmin,dmax,ddmin,ddmax;
	int n_total_bits = 0;
	int bc[256] = {0};
	for (int i = 0; i < n_samples; i++) {
		int chunk_begin=0;
		const int chunk = i >> CHUNK_SIZE_LOG2;
		if (chunk > prev_chunk) {
			if (prev_chunk >= 0) {
				printf("chunk[%d] ", prev_chunk);
				const int ns = repminmap("s", smin, smax);
				const int nd = repminmap("d", dmin, dmax);
				const int ndd = repminmap("dd", ddmin, ddmax);
				int n =
					(ns<=nd && ns<=ndd) ? ns :
					(nd<=ns && nd<=ndd) ? nd :
					(ndd<=ns && ndd<=nd) ? ndd :
					999999;
				n_total_bits += (n << CHUNK_SIZE_LOG2);
				printf("\n");
			}
			prev_chunk = chunk;
			chunk_begin = 1;
		}
		const int cur = samples[i];
		bc[(uint8_t)samples[i]]++;
		const int d = cur - prev;
		const int dd = d - dprev;
		minmax(cur, chunk_begin, &smin, &smax);
		minmax(d, chunk_begin, &dmin, &dmax);
		minmax(dd, chunk_begin, &ddmin, &ddmax);
		o0[i] = s8(d);
		o1[i] = s8(dd);
		//printf("[%d]\ts=%d\td=%d\tdd=%d\n", i, cur, d, dd);
		prev = cur;
		dprev = d;

	}

	const int bdc = ((n_total_bits+7)>>3) + ((n_samples >> CHUNK_SIZE_LOG2) * 1);
	printf("bit/delta compressed size: %d bytes\n", bdc);

	#if 0
	for (int i = -128; i <= 127; i++) {
		const uint8_t ii = i;
		printf("bc[%d]=%d\n", i, bc[ii]);
	}
	#endif

	rawrite("o0.raw", n_samples, o0);
	rawrite("o1.raw", n_samples, o1);

	return 0;
}
