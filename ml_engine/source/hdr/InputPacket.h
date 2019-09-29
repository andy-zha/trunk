#ifndef _INPUTPACKET_
#define _INPUTPACKET_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"

#define _READ_PACKET_

namespace NS_INPUTPACKET
{
	class KeyValue
	{
		public:
			/**
			 * @brief 构造函数
			 */
			KeyValue() 
			{}

			/**
			 * @brief 析构函数
			 */
			~KeyValue()
			{}

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
			std::string key;                            
			std::string value;
	};
}

/** 数据包结构 **/
class InputPacket
{
	public:
		/**
		 * @brief 构造函数
		 */
		InputPacket()
		{
#ifdef _READ_PACKET_
			uPayload = 0;
			uOffset = 0;
			uType = 0;
#endif
			pStr = nullptr;
			uLength = 0;
			MinorVer = -1;
			m_ClientPort = 0;
			m_ServerPort = 0;
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
#ifdef _READ_PACKET_ 
		uint32_t uPayload;                   //负载长度
		uint32_t uOffset;                    //偏移长度
		uint32_t uType;                      //协议值
		char SrcMac[18];                     //源mac地址
		char DstMac[18];                     //宿mac地址
#endif
		uint32_t uLength;                    //报文长度
		int32_t MinorVer;                    //版本号 
		uint32_t m_ClientPort;               //源端口 	
		uint32_t m_ServerPort;               //宿端口
		char *pStr;                          //报文数据
		std::string m_ClientIp;              //源ip
		std::string m_ServerIp;              //宿ip
		std::string m_HttpMethod;            //请求方法
		std::string m_Url;                   //Url
		std::string m_Query;                 //http请求
		std::string m_Host;                  //域名
		std::string m_SiteId;                //站点id
		std::string m_BusinessId;            //业务id
};

#endif
