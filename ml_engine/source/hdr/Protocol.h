#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <iostream>

namespace NS_PROTOCOL
{
	//定义协议枚举值
	typedef enum PROTOCOL
	{
		EM_PROTOCOL_START = -1,               //枚举边界保护
		EM_ETH2_PROTOCOL,                     //Eth2协议值
		EM_IPV4_PROTOCOL,                     //Ipv4协议值
		EM_IPV6_PROTOCOL,                     //Ipv6协议值
		EM_PROTOCOL_END                       //枚举边界保护
	}EM_PROTOCOL;
}

#endif 
