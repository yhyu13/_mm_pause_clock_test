// https://community.intel.com/t5/Software-Archive/rep-Nop-asm-pause/m-p/995897

#include <stdio.h>
#include <thread>
#include <intrin.h>

#define ReadTSC( x ) __asm cpuid \
__asm rdtsc \
__asm mov dword ptr x, eax \
__asm mov dword ptr x + 4, edx 
#define LOOP_COUNT 160000.
#define REPEAT_25( x ) x x x x x x x x x x x x x x x x x x x x x x x x x
#define REPEAT_100(x) REPEAT_25(x) REPEAT_25(x) REPEAT_25(x) REPEAT_25(x)
#define REPEAT_1000(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x) REPEAT_100(x)
#define FACTOR ((double)LOOP_COUNT*1000.0)
#define CLOCKSPERINSTRUCTION(start,end) ((double)end-(double)start)/(FACTOR)

void main(int argc, char** argv)
{
	__int64 start, end, total;
	total = 0;;
	ReadTSC(start);
	for (int i = 0; i < LOOP_COUNT; ++i)
		{
		REPEAT_1000(__asm { nop};)
		}
	ReadTSC(end);
		total = end - start;
		printf("nop: clocks per instruction %4.2f ", (double)total / (double)FACTOR);

		ReadTSC(start);
		total = 0;;
		for (int i = 0; i < LOOP_COUNT; ++i)
			{
			REPEAT_1000(__asm { pause};)
			}
	ReadTSC(end);
		total = end - start;
		printf("pause: clocks per instruction %4.2f ", (double)total / (double)FACTOR);

		ReadTSC(start);
		total = 0;;
		for (int i = 0; i < LOOP_COUNT; ++i)
		{
			REPEAT_1000(_mm_pause();)
		}
		ReadTSC(end);
		total = end - start;
		printf("_mm_pause: clocks per instruction %4.2f ", (double)total / (double)FACTOR);

		ReadTSC(start);
		total = 0;;
		for (int i = 0; i < LOOP_COUNT; ++i)
		{
			REPEAT_1000(std::this_thread::yield();)
		}
		ReadTSC(end);
		total = end - start;
		printf("yield: clocks per instruction %4.2f ", (double)total / (double)FACTOR);
}