#include "trellisCoder.h"

// Таблица для решетчатого кодирования 1/2 (Trellis Encoder State Transition Tables)
const int8_t table1_2[SIZE_OF_TRELLIS_STATE][SIZE_OF_TRELLIS_STATE] =
						{  0,15,12, 3,
						   4,11, 8, 7,
						  13, 2, 1,14,
						   9, 6, 5,10};

// Таблица для перемежения дибитов (Interleave Table)
const int8_t interleaveTable[SIZE_OF_CODED_FRAME/2]=
								 {0,1,8,9,16,17,24,25,
							   32,33,40,41,48,49,56,57,
							   64,65,72,73,80,81,88,89,
							   96,97,2,3,10,11,18,19,
							   26,27,34,35,42,43,50,51,
							   58,59,66,67,74,75,82,83,
							   90,91,4,5,12,13,20,21,
							   28,29,36,37,44,45,52,53,
							   60,61,68,69,76,77,84,85,
							   92,93,6,7,14,15,22,23,
							   30,31,38,39,46,47,54,55,
							   62,63,70,71,78,79,86,87,
							   94,95};						   
						   
//Constellation to Dibit Pair Mapping
//Кодер сопоставляет числам 0-15 (можно их назвать номерами точек созвездия) пары дибитов в соответствии со следующей таблицей
const int8_t ConstelToDibitPair[SIZE_OF_TRELLIS_OUTPUT_CONSTELLATION][2]=
						{	{0,2}, {2,2}, {1,3}, {3,3}, {3,2}, {1,2}, {2,3}, {0,3},
							{3,1}, {1,1},	{2,0}, {0,0},	{0,1}, {2,1},	{1,0} ,{3,0} };
								 
//Функция кодирования данных кодом треллиса с эффективностью 1/2
//Параметры:
//	pDataIn -- указатель на входной массив данных для кодирования, значение каждого элемента =0 или 1;
//	pDataEncoded -- указатель на выходной массив данных после кодирования, значение каждого элемента =0 или 1;
void trellisEnc1_2(const int8_t * const pDataIn, int8_t * const pDataOut)
{
	uint16_t cntDataInBits, cntDataOutBits;
	int8_t curCoderInput;			//текущее значение входного дибита кодера (Final State Machine Input)
	int8_t curCoderState;			//текущее состояние конечного автомата (Current State Storage)
	int8_t nConstelPointOut;	//число 0-15, указывающее на выходную пару дибитов (Constellation Point Output)
	int8_t nDibitOut;					//значение выходного дибита
	
	int8_t pCoderOut[SIZE_OF_CODED_FRAME];	//результат кодирования (без перемежения)
	memset(pCoderOut,0,SIZE_OF_CODED_FRAME*sizeof(int8_t));
	
	curCoderState = 0;	//начальное состояние автомата - нулевое
	for(cntDataInBits=0,cntDataOutBits=0; cntDataInBits<SIZE_OF_UNCODED_FRAME; cntDataOutBits+=SIZE_OF_TRELLIS_STATE,cntDataInBits+=2)
	{			
		curCoderInput = (pDataIn[cntDataInBits]<<1) + (pDataIn[cntDataInBits+1]);	//берем дибит из входных данных
		
		nConstelPointOut = table1_2[curCoderState][curCoderInput];	//Входному значению автомата кодирования и его текущему состоянию
																																//ставится в соответствие число 0-15
		
		curCoderState = curCoderInput;											//Изменяем текущее состояние кодера на значение задержанного входного дибита

		//Число 0-15 преобразуем в пару дибитов		
		nDibitOut = ConstelToDibitPair[nConstelPointOut][0];			//первый дибит
		pCoderOut[cntDataOutBits] = (nDibitOut) & 1;
		pCoderOut[cntDataOutBits+1] = (nDibitOut>>1) & 1;
		
		nDibitOut = ConstelToDibitPair[nConstelPointOut][1];			//второй дибит
		pCoderOut[cntDataOutBits+2] = (nDibitOut) & 1;
		pCoderOut[cntDataOutBits+3] = (nDibitOut>>1) & 1;
	}
	
	//В конце к данным добавляем 2 нулевых flush-бита
	nConstelPointOut = table1_2[curCoderState][0];	//Последнему (нулевому) значению автомата кодирования и его текущему состоянию
																									//ставим в соответствие число 0-15
	
	//Число 0-15 преобразуем в пару дибитов
	nDibitOut = ConstelToDibitPair[nConstelPointOut][0];
	pCoderOut[cntDataOutBits] = (nDibitOut) & 1;
	pCoderOut[cntDataOutBits+1] = (nDibitOut>>1) & 1;
	
	nDibitOut = ConstelToDibitPair[nConstelPointOut][1];
	pCoderOut[cntDataOutBits+2] = (nDibitOut) & 1;
	pCoderOut[cntDataOutBits+3] = (nDibitOut>>1) & 1;

	#ifndef TRELLIS_WITHOUT_INTERLEAVER
	//Перемежение
	uint16_t cntDataOutDibits;
	for(cntDataOutDibits=0,cntDataOutBits=0; cntDataOutDibits<SIZE_OF_CODED_FRAME/2; cntDataOutDibits++,cntDataOutBits+=2)
	{
		pDataOut[cntDataOutBits] = pCoderOut[(interleaveTable[cntDataOutDibits]<<1)+1];	
		pDataOut[cntDataOutBits+1] = pCoderOut[(interleaveTable[cntDataOutDibits]<<1)];
	}
	#else
	memcpy(pDataOut,pCoderOut,SIZE_OF_CODED_FRAME*sizeof(int8_t));
	#endif
}


//Решетка переходов для кода треллиса 1/2
const int8_t TrelStruct[SIZE_OF_TRELLIS_STATE][SIZE_OF_TRELLIS_STATE]=
{
    {  0x02, 0x0C, 0x01, 0x0F },
    {  0x0E, 0x00, 0x0D, 0x03 },
    {  0x09, 0x07, 0x0A, 0x04 },
    {  0x05, 0x0B, 0x06, 0x08 }
};

// Расчет метрики для ребра (числа 1 в битовом представлении числа Data)
uint8_t countMetrics(uint8_t Data)
{
	int8_t cntBits;
	uint8_t numOnes = 0;

	for(cntBits=0; cntBits<8; cntBits++)
		numOnes += (Data>>cntBits)&1;

/*	//подсчёт единиц за один такт процессора (работает хорошо при -o3 и в отсутствие оптимизации)
    asm("        MOV *(#_TempCntMet), AC0"); //гружу в AC0 байт, число единиц которого надо посчитать
    asm("        MOV #255, AC1"); //маска на байт
    asm("        BCNT AC0, AC1, TC1, T0"); //подсчёт единиц в регистре AC0; число единиц теперь помещается в регистр T0
    asm("        MOV T0, *(#_TempCntMet)"); //результат записываю в TempCntMet
*/
    return numOnes;
}

typedef struct SoManyWays
{
	//массив путей с лучшими метриками
	int8_t arr[(SIZE_OF_CODED_FRAME+SIZE_OF_TRELLIS_STATE)/SIZE_OF_TRELLIS_STATE][SIZE_OF_TRELLIS_STATE];
} arSoManyWays;


//Декодер кода треллиса 1/2
//Параметры:
//  pDataIn -- указатель на входной массив закодированной последовательности, значение каждого элемента =0 или 1;
//  pDataOut -- указатель на выходной массив декодированной последовательности, значение каждого элемента =0 или 1;
int16_t trellisDec1_2(const int8_t * const pDataIn, int8_t * const pDataOut)
{
	int16_t i, n, j;	
	int16_t arPathMetrics[SIZE_OF_TRELLIS_STATE];			//метрики лучших путей, входящих в каждую из вершин решетки треллиса
	int16_t arPrevPathMetrics[SIZE_OF_TRELLIS_STATE]; //метрики лучших путей на предыдущем шаге
	int16_t nMinPathMetric, nPathMetric;
	uint16_t step;
	arSoManyWays ways;
	int8_t pDecoderIn[SIZE_OF_CODED_FRAME];

	#ifndef TRELLIS_WITHOUT_INTERLEAVER
	//1. Деперемежение
	//NO: Кроме деперемежения 16-битных слов еще делается изменение порядка байтов в словах
	uint16_t cntDataInBits, cntDataInDibits;
	for(cntDataInDibits=0,cntDataInBits=0; cntDataInDibits<SIZE_OF_CODED_FRAME/2; cntDataInDibits++,cntDataInBits+=2)
	{
		pDecoderIn[(interleaveTable[cntDataInDibits]<<1)] = pDataIn[cntDataInBits];	
		pDecoderIn[(interleaveTable[cntDataInDibits]<<1)+1] = pDataIn[cntDataInBits+1];
	}
	#else
	memcpy(pDecoderIn,pDataIn,SIZE_OF_CODED_FRAME*sizeof(int8_t));
	#endif

	memset(&ways,0,sizeof(SoManyWays));
	
	//2. Декодирование. Прямое прохождение по решетке
	for (n=0,step=0; n<SIZE_OF_CODED_FRAME; n+=SIZE_OF_TRELLIS_STATE,step++)
	{		
		//Выделяем 2 дибита
		int8_t nDibitPairVal;
		#ifndef TRELLIS_WITHOUT_INTERLEAVER
		nDibitPairVal = (pDecoderIn[n]<<3) + (pDecoderIn[n+1]<<2) + (pDecoderIn[n+2]<<1) + (pDecoderIn[n+3]);
		#else
		nDibitPairVal = (pDecoderIn[n+1]<<3) + (pDecoderIn[n]<<2) + (pDecoderIn[n+3]<<1) + (pDecoderIn[n+2]);
		#endif

	  // ------ Строим решетку с каждым из четырех битов -----------
		memcpy(arPrevPathMetrics,arPathMetrics,SIZE_OF_TRELLIS_STATE*sizeof(int16_t));
		for(j=0; j<SIZE_OF_TRELLIS_STATE; j++) //j- номер обрабатываемой вершины решетки состояний
		{
			if(!step)	//На первом шаге все пути выходят из нулевого состояния
				nMinPathMetric = countMetrics(TrelStruct[0][j] ^ nDibitPairVal);	//метрику для каждого из 16-ти новых ребер
			else
			{// высчитываем минимальный вес из четырех входных путей
				nMinPathMetric = SHRT_MAX;
				for(i=0; i<SIZE_OF_TRELLIS_STATE; i++)
				{
					//Cчитаем суммарную метрику, которая входит в j-ую вершину, из i-того пути
					nPathMetric = arPrevPathMetrics[i] + countMetrics(TrelStruct[i][j] ^ nDibitPairVal);
					
					if(nPathMetric<=nMinPathMetric) //определение ребра (с минимальной сумм. метрикой), входящего в j-ое состояние решетки
					{
						nMinPathMetric = nPathMetric;
						ways.arr[step][j] = i;  //фиксация выжившего пути
					}
				}
			}
			arPathMetrics[j] = nMinPathMetric;	//сохраняем метрику в массив, в котором хранятся метрики для следующего шага
		}
		// ------------------------------------------------------------
	}

	//3. Декодирование. Обратное прохождение по решетке
	//Формируем результат по наименьшей метрике
	int8_t nDibitVal = 0; //возвращаемся из нулевой вершины последней вертикали решетки к нулевой вершине певой вертикали решетки
	for (i=(SIZE_OF_CODED_FRAME-SIZE_OF_TRELLIS_STATE)/SIZE_OF_TRELLIS_STATE; i>0; i--)
	{ //i меняется от 1 до 49, т.е. вертикаль ways.arr[0][0..3] не используется
		nDibitVal = ways.arr[i][nDibitVal];//номер вершины выжившего пути на предидущем шаге
    //записываем результат декодирования в выходной массив
 	  pDataOut[i*2-1] = nDibitVal&1;
	  pDataOut[i*2-2] = (nDibitVal>>1)&1;
	}
	
	return arPathMetrics[0]; 	//в arPathMetrics[0] хранится метрика нулевого (правильного) пути (т.к. выхожу из нулевого состояния 
														//и в него же должен вернуться). она хранит число исправленных ошибок
}



#ifdef DEBUG_TRELLIS_CODER
void TestTrellisCoder()
{
	//Исходное сообщение
	const int8_t MessageForTransfer[SIZE_OF_UNCODED_FRAME] = 		
	{0,1,0,0,1,0,0,0, 0,0,0,0,1,0,0,0, 0,1,0,0,0,0,0,1, 0,0,0,0,0,0,1,0,
	 1,0,1,0,0,1,0,1, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,
	 0,0,0,0,0,1,1,1, 1,0,0,0,0,0,0,0, 1,0,1,0,1,0,1,0, 1,0,0,0,1,0,0,1};
	
	//Сообщение после преобразований кодирования и декодирования
	int8_t MessageTransfered[SIZE_OF_UNCODED_FRAME];
	memset(MessageTransfered,0,SIZE_OF_UNCODED_FRAME);
	
	//Закодированное сообщение
	int8_t EncodedData[SIZE_OF_CODED_FRAME];
	
	//Кодируем
	trellisEnc1_2(MessageForTransfer, EncodedData);

	//Вносим канальные ошибки
	EncodedData[1] ^= 1;
	EncodedData[3] ^= 1; //EncodedData[4] ^= 1;
	EncodedData[10] ^= 1; EncodedData[18] ^= 1;
	EncodedData[48] ^= 1; EncodedData[52] ^= 1; EncodedData[56] ^= 1;
	EncodedData[94] ^= 1;

	//Декодируем
	int16_t numDetectedErrors = trellisDec1_2(EncodedData, MessageTransfered);
	
	//Сравниваем сообщение до и после кодирования
	int8_t CmpResult[SIZE_OF_UNCODED_FRAME];
	uint8_t numErrors = 0;
	for(int8_t i=0; i<SIZE_OF_UNCODED_FRAME; i++)
	{
		CmpResult[i] = (MessageForTransfer[i]==MessageTransfered[i] ? 0 : 1);
		if(CmpResult[i])
			numErrors++;
	}
}	
#endif