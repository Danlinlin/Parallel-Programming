#include <xmmintrin.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>

#define SSE_WIDTH		  4

#ifndef NUMTRIES
 #define NUMTRIES	    100
#endif

#ifndef SIZE
 #define SIZE	1000000
#endif

float A[SIZE];
float B[SIZE];
float SumSSE;
float SumnSSE;

float
SimdMulSum(float* a, float* b, int len)
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = (len / SSE_WIDTH) * SSE_WIDTH;
	register float* pa = a;
	register float* pb = b;
	__m128 ss = _mm_loadu_ps(&sum[0]);
	for (int i = 0; i < limit; i += SSE_WIDTH)
	{
		ss = _mm_add_ps(ss, _mm_mul_ps(_mm_loadu_ps(pa), _mm_loadu_ps(pb)));
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps(&sum[0], ss);
	for (int i = limit; i < len; i++)
	{
		sum[0] += a[i] * b[i];
	}
	return sum[0] + sum[1] + sum[2] + sum[3];
}

int
main(int argc, char* argv[])
{
	SumSSE = 0.;
	SumnSSE = 0.;
	double maxMega1 = 0.;
	double maxMega2 = 0.;
	for (int t = 0; t < NUMTRIES; t++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			A[i] = 1.;
			B[i] = 2.;
		}
        
        
		double time0 = omp_get_wtime();
        
		SumSSE= SimdMulSum(A, B, SIZE);
        
		double time1 = omp_get_wtime();
		double mega1 = (double)SIZE / (time1 - time0) / 1000000.;
		if (mega1 > maxMega1)
			maxMega1 = mega1;

		double Ntime0 = omp_get_wtime();
        
		for (int i = 0; i < SIZE; i++)
		{
			SumnSSE += A[i] * B[i];
		}
        
		double Ntime1 = omp_get_wtime();
		double mega2 = (double)SIZE / (Ntime1 - Ntime0) / 1000000.;
		if (mega2 > maxMega2)
			maxMega2 = mega2;
	}

	printf("SIZE\t\tPsse\t\tPnon-sse\t\tSpeedup\n");
	fprintf(stdout, "%d\t\t%5.2lf\t\t%5.2lf\t\t%5.2lf\n", SIZE, maxMega1, maxMega2, maxMega1 / maxMega2);
}
