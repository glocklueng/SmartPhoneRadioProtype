#include "mathfuncs.h"

#define SIZE_OF_SIN_TABLE	(256)
int16_t SinTab[SIZE_OF_SIN_TABLE] = {0, 804, 1608, 2410, 3212, 4011, 4808, 5602, 6393, 
												7179,	7962, 8739, 9512, 10278, 11039, 11793, 12539, 13279, 14010, 
												14732, 15446, 16151, 16846, 17530, 18204, 18868, 19519, 20159, 
												20787, 21403, 22005, 22594, 23170, 23731, 24279, 24811, 25329, 
												25832, 26319, 26790, 27245, 27683, 28105, 28510, 28898, 29268, 
												29621, 29956, 30273, 30571, 30852, 31113, 31356, 31580, 31785, 
												31971, 32137, 32285, 32412, 32521, 32609, 32678, 32728, 32757, 
												32767, 32757, 32728, 32678, 32609, 32521, 32412, 32285, 32137, 
												31971, 31785, 31580, 31356, 31113, 30852, 30571, 30273, 29956, 
												29621, 29268, 28898, 28510, 28105, 27683, 27245, 26790, 26319, 
												25832, 25329, 24811, 24279, 23731, 23170, 22594, 22005, 21403, 
												20787, 20159, 19519, 18868, 18204, 17530, 16846, 16151, 15446, 
												14732, 14010, 13279, 12539, 11793, 11039, 10278, 9512, 8739, 7962, 
												7179, 6393, 5602, 4808, 4011, 3212, 2410, 1608, 804, 0, -804, 
												-1608, -2410, -3212, -4011, -4808, -5602, -6393, -7179, -7962, 
												-8739, -9512, -10278, -11039, -11793, -12539, -13279, -14010, 
												-14732, -15446, -16151, -16846, -17530, -18204, -18868, -19519, 
												-20159, -20787, -21403, -22005, -22594, -23170, -23731, -24279, 
												-24811, -25329, -25832, -26319, -26790, -27245, -27683, -28105, 
												-28510, -28898, -29268, -29621, -29956, -30273, -30571, -30852, 
												-31113, -31356, -31580, -31785, -31971, -32137, -32285, -32412, 
												-32521, -32609, -32678, -32728, -32757, -32767, -32757, -32728, 
												-32678, -32609, -32521, -32412, -32285, -32137, -31971, -31785, 
												-31580, -31356, -31113, -30852, -30571, -30273, -29956, -29621, 
												-29268, -28898, -28510, -28105, -27683, -27245, -26790, -26319, 
												-25832, -25329, -24811, -24279, -23731, -23170, -22594, -22005, 
												-21403, -20787, -20159, -19519, -18868, -18204, -17530, -16846, 
												-16151, -15446, -14732, -14010, -13279, -12539, -11793, -11039, 
												-10278, -9512, -8739, -7962, -7179, -6393, -5602, -4808, -4011, 
												-3212, -2410, -1608, -804};

												
void FillBufByToneSignal(int16_t* pBuf, uint16_t nSizeBuf, uint16_t nFreqSampl, uint16_t nFreqTone)
{
	static uint8_t phase = 0;
	uint16_t cntSamples;
	
	for(cntSamples=0; cntSamples<nSizeBuf; cntSamples++)
	{
		pBuf[cntSamples] = SinTab[phase];
		phase += (SIZE_OF_SIN_TABLE * nFreqTone)/nFreqSampl;
	}
}
