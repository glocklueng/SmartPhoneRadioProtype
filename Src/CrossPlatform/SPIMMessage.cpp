#include "SPIMMessage.h"


SPIMMessage::SPIMMessage()
{
	SPIMHeaderData = SPIMmsgData;
	SPIMbodyData = SPIMmsgData + SIZE_OF_HEADER;

	//�������������� ������ ��� ������, ���������� ��� ���������, � �.�. ��������� � CRC
	memset(SPIMmsgData,0,MAX_SIZE_OF_MSG);

	//�� ��������� ������� ��������� � ����� ������� �����
	Size = MIN_SIZE_OF_MSG;
	SPIMbodySize = 0;

	//��������� �� ���� CRC ������������� ����� �� ����������, ��������� ���� ����� ������� �����
	SPIMCRC = SPIMmsgData + SIZE_OF_HEADER;

	Data = SPIMmsgData;
	Body = SPIMbodyData;
}


SPIMMessage::SPIMMessage(const uint8_t* pMsgData, uint16_t msgSize)
{
	if(!pMsgData)
		return;

	if((msgSize < MIN_SIZE_OF_MSG) || (msgSize > MAX_SIZE_OF_MSG))
		return;

	SPIMHeaderData = SPIMmsgData;
	SPIMbodyData = SPIMmsgData + SIZE_OF_HEADER;

	//�������������� ������ ��� ������, ���������� ��� ���������
	memset(SPIMmsgData,0,MAX_SIZE_OF_MSG);

	//�������� ������ � ���� ���������
	memcpy((void*)SPIMmsgData,(void*)pMsgData,msgSize);

	Size = msgSize;
	SPIMbodySize = Size - (SIZE_OF_HEADER + SIZE_OF_CRC);

	//������������� ��������� �� ���� CRC
	SPIMCRC = SPIMmsgData + SIZE_OF_HEADER + SPIMbodySize;

	Data = SPIMmsgData;
	Body = SPIMbodyData;

	return;
}


SPIMMessage::~SPIMMessage()
{

}


/**
	* @brief  ����������� ��������� ��������� � ����������� ���
	*           ��������������� � ���������
	*
	* @param  bodySize - ����� ����, ����
	* @param  address - ������� ���������
	* @param  noMsg - ���������� ����� ��������� (�� ������ 4)
	* @param  IDcmd - ������������� �������
	*
	* @note   ������� ���������� ��������� �� ��������� �����,
	*               ���������� ��� ������� ���������, � �������� ���
	*               � ���������
	*
	* @retval ��������� ���������� �������:
	*               0 - ��������� ������� ��������� � ���������� �
	*               ���������;
	*               �� 0 - ��������� �� ����� ���� ���������
	*               (������������ ������� ������)
	*/
uint8_t SPIMMessage::setHeader(uint8_t bodySize, uint8_t address, uint8_t noMsg, uint8_t IDcmd)
{
	structSPIMMsgHeader SPIMMsgHeader;

	if(bodySize <= MAX_SIZE_OF_BODY)
		SPIMMsgHeader.bodySize = bodySize;
	else
		return 1;

	SPIMMsgHeader.adress = address;

	SPIMMsgHeader.noMsg = noMsg;

	SPIMMsgHeader.IDCmd = IDcmd;

	memcpy((void*)SPIMHeaderData,(void*)&SPIMMsgHeader,SIZE_OF_HEADER);

	return 0;
}

/**
	* @brief  ����������� ���� ��������� ��������������� � ���������
	*
	* @param  pBodyData - ��������� �� ������ ����;
	* @param  bodySize - ����� ����, ����
	*
	* @retval ��������� ���������� �������:
	*               0 - ���� ������� ����������� � ���������;
	*               �� 0 - ���� �� ����� ���� ����������� � ���������:
	*               ������� ������� ��������� ��� ������ ������� ������
	*               ��������� ����������� ����������
  */
uint8_t SPIMMessage::setBody(uint8_t* pBodyData, uint8_t bodySize)
{
	if((!pBodyData) && bodySize)
		return(1);

	if(bodySize > MAX_SIZE_OF_BODY)
		return(1);

	memcpy((void*)SPIMbodyData,(void*)pBodyData,bodySize);

	SPIMbodySize = bodySize;
	Size = SPIMbodySize + SIZE_OF_HEADER + SIZE_OF_CRC;

	//������������� ��������� �� ���� CRC
	SPIMCRC = SPIMmsgData + SIZE_OF_HEADER + SPIMbodySize;

	return(0);
}


/**
	* @brief  ����������� ������ ��������� � SPIMMessage
	*
	* @param  pMsgData - ��������� �� ������ ���������;
	* @param  msgSize - ����� ���������, ����
	*
	* @retval ��������� ���������� �������:
	*               0 - ������ ������� �����������;
	*               �� 0 - ������ �� ����� ���� ����������� � ���������:
	*               ������� ������� ��������� ��� ������������ ������
	*               ������� ������
  */
uint8_t SPIMMessage::setMsg(uint8_t* pMsgData, uint8_t msgSize)
{
	if(!pMsgData)
		return 1;

	if((msgSize < MIN_SIZE_OF_MSG) || (msgSize > MAX_SIZE_OF_MSG))
		return 1;

	memcpy((void*)SPIMmsgData,(void*)pMsgData,msgSize);

	Size = msgSize;
	SPIMbodySize = Size - (SIZE_OF_HEADER + SIZE_OF_CRC);

	//������������� ��������� �� ���� CRC
	SPIMCRC = SPIMmsgData + SIZE_OF_HEADER + SPIMbodySize;

	return 0;
}

/**
	* @brief  �������� ����������� ����� � SPIMMessage, �����������
	*             �� ������ ������ ���������
	*
  */
uint8_t SPIMMessage::setCRC()
{
	uint8_t nCRC = CRC_Calc(SPIMmsgData, Size-1);

	*SPIMCRC = nCRC;

	return(nCRC);
}

/**
	* @brief  ������� ���������� ������ ��������� ���������
	*
	* @param pHeaderData - ���������, �� �������� ������
	*               ���� �������� ������ ���������
	*
	* @note ������� �� �������� ������ ��� ������������
	*           ������; ��� ������ ���� �������� ��������������
	*           � ����������� ������ ���������� ��������
	*
	* @retval ��������� ������������ ��������:
	*               �� 255 - ������ ������ ���������;
	*               255 - ������ ���������� ������� (�������
	*               ������� ���������)
  */
uint8_t SPIMMessage::getHeader(uint8_t* pHeaderData)
{
	if(!pHeaderData)
		return(0xFF);

	memcpy(pHeaderData,SPIMHeaderData,SIZE_OF_HEADER);

	return(SIZE_OF_HEADER);
}

/**
    * @brief  ������� ���������� ������ ���� ���������
    *
    * @param pHeaderData - ���������, �� �������� ������
    *               ���� �������� ������ ����
    *
    * @note ������� �� �������� ������ ��� ������������
    *           ������; ��� ������ ���� �������� ��������������
    *           � ����������� ������ ���������� ��������
    *
    * @retval ��������� ������������ ��������:
    *               0-128 - ������ ������ ����;
    *               255 - ������ ���������� ������� (�������
    *               ������� ���������)
  */
uint8_t SPIMMessage::getBody(uint8_t* pBodyData)
{
	if(!pBodyData)
		return(0xFF);

	if(!SPIMbodySize)
		return(0);

	memcpy(pBodyData,SPIMbodyData,SPIMbodySize);

	return(SPIMbodySize);
}

/**
    * @brief  ������� ���������� �������� ���� CRC
    *
    * @retval �������� ���� CRC
  */
uint8_t SPIMMessage::getCRC()
{
	return(*SPIMCRC);
}

/**
    * @brief  ������� �������� ������������ ����������� �����,
    *           ���������� � ��������������� ���� SPIMMessage
    *
    * @retval   0 - �������� ���� ����������� (�� ��������� �
    *               ����������� ����������� ������),
    *               1 - �������� ���� ���������
  */
uint8_t SPIMMessage::checkCRC()
{
	if(*SPIMCRC == CRC_Calc(SPIMmsgData, Size-1))
		return(1);
	else
		return(0);
}

/**
    * @brief  ������� ������ �������� ������ �� ����������������
    *           ���� ��������� ��������� SPIMMessage
    *
    * @retval �������� ���� ������
  */
uint8_t SPIMMessage::getAddress()
{
	structSPIMMsgHeader *SPIMMsgHeader;

	SPIMMsgHeader = (structSPIMMsgHeader*)SPIMHeaderData;

	return(SPIMMsgHeader->adress);
}

/**
	* @brief  ������� ������ �������� ������ ���������
	*           �� ���������������� ���� ��������� ���������
	*           SPIMMessage
	*
	* @retval �������� ���� ������ ���������
  */
uint8_t SPIMMessage::getNoMsg()
{
	structSPIMMsgHeader *SPIMMsgHeader;

	SPIMMsgHeader = (structSPIMMsgHeader*)SPIMHeaderData;

	return(SPIMMsgHeader->noMsg);
}

/**
	* @brief  ������� ������ �������� ����� ���� ��
	*           ���������������� ���� ��������� ���������
	*           SPIMMessage
	*
	* @retval �������� ���� ����� ����
  */
uint8_t SPIMMessage::getSizeBody()
{
	structSPIMMsgHeader *SPIMMsgHeader;

	SPIMMsgHeader = (structSPIMMsgHeader*)SPIMHeaderData;

	return(SPIMMsgHeader->bodySize);
}

/**
	* @brief  ������� ������ �������� �������������� �������
	*           �� ���������������� ���� ��������� ���������
	*           SPIMMessage
	*
	* @retval �������� ���� �������������� �������
  */
uint8_t SPIMMessage::getIDCmd()
{
	structSPIMMsgHeader *SPIMMsgHeader;

	SPIMMsgHeader = (structSPIMMsgHeader*)SPIMHeaderData;

	return(SPIMMsgHeader->IDCmd);
}

/**
	* @brief  ������� ���������� 8-������� CRC �� ������ XOR
	*
	* @retval �������� CRC
  */
uint8_t SPIMMessage::CRC_Calc(uint8_t* pData, uint8_t sizeData)
{
	uint8_t nCRC = 0;

	while(sizeData--)
		nCRC ^= *pData++;

	return(nCRC);
}

/**
	* @brief  ����������� ������������� ������� ������ ��� 
	*					�������� �������
	*
	* @retval ������������� ������� ������
  */
uint8_t SPIMMessage::IDBackCmd(uint8_t IDCmd)
{
	//������������ ������ ��� ���� ������ ����� �������� �� 1 ������ �������������� �������
	return(IDCmd+1);
}


void SPIMMessage::ParseOpModeCode(uint8_t opModeCode, uint8_t& RadioChanType, uint8_t& IsTestMode, 
																	uint8_t& SignalPower, uint8_t& ARMPowerMode, uint8_t& BaudRate)
{
	RadioChanType	= (opModeCode>>SHIFT_RADIOCHANTYPE_IN_OPMODECODE)&MASK_RADIOCHANTYPE_IN_OPMODECODE;
	
	IsTestMode = (opModeCode>>SHIFT_TESTMODE_IN_OPMODECODE)&MASK_TESTMODE_IN_OPMODECODE;
	
	SignalPower = (opModeCode>>SHIFT_SIGNALPOWER_IN_OPMODECODE)&MASK_SIGNALPOWER_IN_OPMODECODE;
	
	ARMPowerMode = (opModeCode>>SHIFT_ARMPOWERMODE_IN_OPMODECODE)&MASK_ARMPOWERMODE_IN_OPMODECODE;

	BaudRate = (opModeCode>>SHIFT_RADIOBAUDRATE_IN_OPMODECODE)&MASK_RADIOBAUDRATE_IN_OPMODECODE;
}


void SPIMMessage::ParseAudioCode(uint8_t audioCode, uint8_t& AudioOutLevel, uint8_t& AudioInLevel)
{
	AudioOutLevel = (audioCode>>SHIFT_OUTLEVEL_IN_AUDIOCODE)&MASK_OUTLEVEL_IN_AUDIOCODE;
	
	AudioInLevel = (audioCode>>SHIFT_INLEVEL_IN_AUDIOCODE)&MASK_INLEVEL_IN_AUDIOCODE;
}


void SPIMMessage::CmdReqParam::SetPointerToMessage(SPIMMessage* mes)
{
	objSPIMMessage = mes;
}

uint8_t SPIMMessage::CmdReqParam::OpModeCode(uint8_t RadioChanType, uint8_t IsTestMode, 
																						 uint8_t SignalPower, uint8_t ARMPowerMode, uint8_t BaudRate)
{
 	return( ((RadioChanType&MASK_RADIOCHANTYPE_IN_OPMODECODE)<<SHIFT_RADIOCHANTYPE_IN_OPMODECODE) |
					((IsTestMode&MASK_TESTMODE_IN_OPMODECODE)<<SHIFT_TESTMODE_IN_OPMODECODE) |
					((SignalPower&MASK_SIGNALPOWER_IN_OPMODECODE)<<SHIFT_SIGNALPOWER_IN_OPMODECODE)	|
					((ARMPowerMode&MASK_ARMPOWERMODE_IN_OPMODECODE)<<SHIFT_ARMPOWERMODE_IN_OPMODECODE) |
					((BaudRate&MASK_RADIOBAUDRATE_IN_OPMODECODE)<<SHIFT_RADIOBAUDRATE_IN_OPMODECODE)	);
}


uint8_t SPIMMessage::CmdReqParam::AudioCode(uint8_t AudioOutLevel, uint8_t AudioInLevel)
{
 	return( ((AudioOutLevel&MASK_OUTLEVEL_IN_AUDIOCODE)<<SHIFT_OUTLEVEL_IN_AUDIOCODE) |
					((AudioInLevel&MASK_INLEVEL_IN_AUDIOCODE)<<SHIFT_INLEVEL_IN_AUDIOCODE)	);
}


uint8_t SPIMMessage::CmdReqParam::MaskReqParam()
{
	#ifdef DEBUG_SPIMMESSAGE_DEFINE_INNER_CLASS_WO_POINTER_TO_OUTTER
	//��� ���� ������� ������� ����������� � ����� �������� ������ ��� �������� ��������� �� ������ �������� ������
	//(�.�. ��� ������ SetPointerToMessage() � ���������� objSPIMMessage � ����� ����������� ������ CmdReqParam),
	//�� ������ �� ����������. ��� ����������, �� ��������� �� ���, ������� ������ ����. ����������� ���� �������,
	//������� ������� � ���. ��. ���������� ��������: http://rsdn.ru/forum/cpp/2230679.all
	return(((SPIMMessage*)this)->Body[0]);
	#endif
	return(objSPIMMessage->Body[0]);
}

uint8_t SPIMMessage::CmdReqParam::isAsynReqParam()
{
	if(MaskReqParam() & ASYNC_MASK_IN_REQ)
		return(1);
	else
		return(0);
}


uint8_t SPIMMessage::CmdReqParam::isOpModeReq()
{
	if(MaskReqParam() & OPMODE_MASK_IN_REQ)
		return(1);
	else
		return(0);
}

uint8_t SPIMMessage::CmdReqParam::isAudioReq()
{
	if(MaskReqParam() & AUDIO_MASK_IN_REQ)
		return(1);
	else
		return(0);	
}

uint8_t SPIMMessage::CmdReqParam::isRxFreqReq()
{
	if(MaskReqParam() & RXFREQ_MASK_IN_REQ)
		return(1);
	else
		return(0);	
}

uint8_t SPIMMessage::CmdReqParam::isTxFreqReq()
{
	if(MaskReqParam() & TXFREQ_MASK_IN_REQ)
		return(1);
	else
		return(0);		
}

uint8_t SPIMMessage::CmdReqParam::isFECModeReq()
{
	if(MaskReqParam() & FECMODE_MASK_IN_REQ)
		return(1);
	else
		return(0);			
}

uint8_t SPIMMessage::CmdReqParam::isRSSIReq()
{
	if(MaskReqParam() & RSSI_MASK_IN_REQ)
		return(1);
	else
		return(0);		
}

uint8_t SPIMMessage::CmdReqParam::isChanStateReq()
{
	if(MaskReqParam() & CHANSTATE_MASK_IN_REQ)
		return(1);
	else
		return(0);		
}

