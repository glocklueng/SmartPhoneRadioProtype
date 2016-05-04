#include <limits.h>
#include <string.h>

#ifdef STM32F071xB
#include "stm32f0xx_hal.h"
#endif
	 
#ifdef STM32F103xE	 
#include "stm32f1xx_hal.h"
#endif

#define SIZE_OF_CODED_FRAME		(196)
#define SIZE_OF_UNCODED_FRAME	(96)

//������ ��������� FSM (�������� ��������� ���������) ������, ���
#define SIZE_OF_TRELLIS_STATE	(4)
//������ ��������� ���������
#define SIZE_OF_TRELLIS_OUTPUT_CONSTELLATION 		(SIZE_OF_TRELLIS_STATE*SIZE_OF_TRELLIS_STATE)

void trellisEnc1_2(const int8_t * const pDataIn, int8_t * const pDataOut);
int16_t trellisDec1_2(const int8_t * const pDataIn, int8_t * const pDataOut);

void TestTrellisCoder();
