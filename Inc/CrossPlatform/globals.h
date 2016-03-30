/**
  ******************************************************************************
  * @file    globals.h
  * @brief   ����, ���������� ���������� ��������� ������� SmartPhonePrototype
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 NTLab
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBALS_H
#define __GLOBALS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <limits.h>	 
#include "cmx7262.h"
	 
#define TRUE 	(1)
#define FALSE (0)
	 
	 
	 
//������������ �������� ������ ������ ����������� ������, ��
#define CMX7262_BUFFER_DURATION_MS (60)

//����� ������� ������ ��������, ������������� ����������� ������ ��� ���������������� ��������
#define NUM_CMX7262_BUFFERS_INITACCUM_FOR_TX	(3)		//60 �� x 3 = 180 ��

//����� ������� ������ �������� � ����� �����������
#define NUM_CMX7262_BUFFERS_IN_RADIOPACK	(3)				//60 �� x 3 = 180 ��

//������ ����������� � ������ �������� ������, ������ ������� ������
#define RADIOPACK_VOICEMODE_SIZE 	(NUM_CMX7262_BUFFERS_IN_RADIOPACK * CMX7262_CODEC_BUFFER_SIZE)

//������ ����������� � ������ �������� ������
#define RADIOPACK_DATAMODE_SIZE 	(RADIOPACK_VOICEMODE_SIZE)

//������ ������ �� ��������, ������������� ����������� ������ ��� ���������������� ��������
#define SIZE_OF_DATA_FROM_CMX7262_INITACCUM_FOR_TX	(NUM_CMX7262_BUFFERS_INITACCUM_FOR_TX * CMX7262_CODEC_BUFFER_SIZE)

//������ ������������ ����������� � ������ �������� ������, ������� ������ + ���������
//��������� ������ - ������ 1-�������� �����, ������������ ������� ������
#define RADIOPACK_MODE4800_EXTSIZE	(RADIOPACK_VOICEMODE_SIZE+5)

#define RADIOPACK_MODE9600_EXTSIZE	(RADIOPACK_MODE4800_EXTSIZE)
#define RADIOPACK_MODE19200_EXTSIZE	(RADIOPACK_MODE4800_EXTSIZE)
#define RADIOPACK_MODE48000_EXTSIZE	(RADIOPACK_MODE4800_EXTSIZE)

//����� ������-������, ����������� ��1120 � �������� �������� ������ � ���������� ������� RSSI, LQI � ��������� �������� CRC
#define SIZE_OF_RADIO_STATUS	(2)

//������������ ������ �����������, ����
#define MAX_RADIOPACK_SIZE	(128)
	 
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GLOBALS_H */
