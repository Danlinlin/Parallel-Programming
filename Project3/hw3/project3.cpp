#include<iostream>
#include <omp.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>

int	NowYear;		// 2022 - 2027
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

int NowNumWolf;

unsigned int seed = 0;

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

float
SQR( float x )
{
        return x*x;
}

float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

void Deer(){
    int NextNumDeer = NowNumDeer;
    int carryingCapacity = (int)(NowHeight);
    if (NextNumDeer < carryingCapacity)
        NextNumDeer++;
    else
        if (NextNumDeer > carryingCapacity)
            NextNumDeer--;
    if (NowNumWolf >= 2) {
        NextNumDeer = NextNumDeer - NowNumWolf ;
    }
    else{
        NextNumDeer = NextNumDeer + 0;
    }
        
    if (NextNumDeer < 0)
        NextNumDeer = 0;
    
#pragma omp barrier
    NowNumDeer = NextNumDeer ;
#pragma omp barrier
}

void Grain(){
    float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
    float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));

    float NextHeight = NowHeight;
    NextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
    NextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
    if (NextHeight < 0.){
        NextHeight = 0.;
    }
    #pragma omp barrier
    NowHeight = NextHeight;
    #pragma omp barrier
}

void Wolf(){
    float NextWolf = 1;
    if (NowNumWolf < NowNumDeer/2) {
        NextWolf  = NowNumWolf  + 1;
    }
    else {
        NextWolf  = NowNumWolf  - 1;
    }
    if (NextWolf < 1) {
        NextWolf = 1;
    }
#pragma omp barrier
    NowNumWolf = NextWolf;
#pragma omp barrier
}

void Watcher(){
    #pragma omp barrier
    #pragma omp barrier

    float Temperature = (5. / 9.) * (NowTemp - 32);
    float Precipitation_Height = 2.54 * NowPrecip;
    float Grain_Height = 2.54 * NowHeight;

    printf("%d,%d,%0.3f,%0.3f,%0.3f,%d,%d \n", NowYear, NowMonth, Temperature, Precipitation_Height, Grain_Height, NowNumDeer, NowNumWolf);

    NowMonth++;
    if (NowMonth > 11){
        NowMonth = 0;
        NowYear++;
    }
}

int main(){
    // starting date and time:
    NowMonth =    0;
    NowYear  = 2022;
    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;
    seed = 0;
    while( NowYear < 2028 ){
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
	        NowPrecip = 0.;

    omp_set_num_threads( 4 );	

#pragma omp parallel sections
{
    #pragma omp section
    {
        Deer( );
    }

    #pragma omp section
    {
        Grain( );
    }

    #pragma omp section
    {
        Watcher( );
    }

    #pragma omp section
    {
        Wolf( );	// your own
    }
    }       // implied barrier -- all functions must return in order
	// to allow any of them to get past here
    }
    return 0;  
}
