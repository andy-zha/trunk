#include "Eth2Parser.h"

//构造函数
Eth2Parser::Eth2Parser()
{}

//析构函数
Eth2Parser::~Eth2Parser()
{}

//初始化接口
int32_t Eth2Parser::Init()
{
	return RET::SUC;
}

//解析器入口
int32_t Eth2Parser::Start(InputPacket *&InputPkt)
{
	//异常判断
	if (NULL == InputPkt->pStr || NS_ETH2PARSER::ETH_LEN >= InputPkt->uLength
			|| InputPkt->uLength <= InputPkt->uOffset 
			|| NS_ETH2PARSER::ETH_LEN >= InputPkt->uPayload)
	{
		return RET::FAIL;
	}

	//本层偏移长度
	uint32_t uOffset = 0;

	//提取源mac
	uint8_t uMacOne = *((uint8_t*)InputPkt->pStr);
	uint8_t uMacTwo = *((uint8_t*)InputPkt->pStr + 1);
	uint8_t uMacThree = *((uint8_t*)InputPkt->pStr + 2);
	uint8_t uMacFour = *((uint8_t*)InputPkt->pStr + 3);
	uint8_t uMacFive = *((uint8_t*)InputPkt->pStr + 4);
	uint8_t uMacSix = *((uint8_t*)InputPkt->pStr + 5);
	snprintf(InputPkt->SrcMac, NS_ETH2PARSER::MAC_LEN, "%2x:%2x:%2x:%2x:%2x:%2x",
			uMacOne, uMacTwo, uMacThree, uMacFour, uMacFive, uMacSix);
	uOffset += 6;

	//提取宿mac
	uMacOne = *((uint8_t*)InputPkt->pStr + 6);
	uMacTwo = *((uint8_t*)InputPkt->pStr + 7);
	uMacThree = *((uint8_t*)InputPkt->pStr + 8);
	uMacFour = *((uint8_t*)InputPkt->pStr + 9);
	uMacFive = *((uint8_t*)InputPkt->pStr + 10);
	uMacSix = *((uint8_t*)InputPkt->pStr + 11);
	snprintf(InputPkt->DstMac, NS_ETH2PARSER::MAC_LEN, "%2x:%2x:%2x:%2x:%2x:%2x",
			uMacOne, uMacTwo, uMacThree, uMacFour, uMacFive, uMacSix);
	uOffset += 6;		

	//取上层协议值	
	uint16_t uProNum = ntohs(*((uint16_t*)(InputPkt->pStr + uOffset)));
	uOffset += 2;

	//只处理ip协议，arp及rarp协议不处理
	if (NS_ETH2PARSER::IP_PROTOCOL == uProNum)
	{
		InputPkt->uType = NS_PROTOCOL::EM_IPV4_PROTOCOL;
	}
	else
	{
		InputPkt->uType = NS_PROTOCOL::EM_PROTOCOL_END;
		return RET::FAIL;
	}

	//计算包体负载偏移
	InputPkt->uPayload = InputPkt->uPayload - uOffset;
	InputPkt->uOffset += uOffset;

	return RET::SUC;
}
