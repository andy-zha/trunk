#ifndef _INPUTPACKET_
#define _INPUTPACKET_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"

class InputPacket
{
	public:
		/**
		 * @brief 构造函数
		 */
		InputPacket()
		{
			pStr = NULL;
			uLength = 0;
			uPayload = 0;
			uOffset = 0;
			uType = 0;
		}

		/**
		 * @brief 析构函数
		 */
		~InputPacket()
		{
			free_ptr(pStr);
		}

		/**
		 * @brief 重载 内存分配
		 *
		 * @prame size 内存大小
		 *
		 * @return 内存地址
		 */
		static void * operator new(size_t size)
		{
			void *p = (void*)_MEM_NEW_(size);
			return p;
		}

		/**
		 * @brief 重载 内存释放
		 *
		 * @prame p 释放地址
		 */
		static void operator delete(void *p)
		{
			_MEM_DEL_(p);
		}

	public:
		char *pStr;                          //报文数据
		uint32_t uLength;                    //报文长度
		uint32_t uPayload;                   //负载长度
		uint32_t uOffset;                    //偏移长度
		uint32_t uType;                      //协议值
		char SrcMac[18];                     //源mac地址
		char DstMac[18];                     //宿mac地址
};

#endif
