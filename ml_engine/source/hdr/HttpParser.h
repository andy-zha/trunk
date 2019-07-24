#ifndef _HTTPPARSER_H_
#define _HTTPPARSER_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "InputPacket.h"
#include <http_parser.h>

namespace NS_HTTPPASER
{
	//请求方法枚举
	typedef enum HTTPMETHOD
	{
		EM_METHOD_START = -1,              //枚举保护
		EM_DELETE = 0,                     //DELETE请求方法 
		EM_GET,	                           //GET请求方法
		EM_HEAD,                           //HEAD请求方法
		EM_POST,                           //POST请求方法
		EM_PUT,                            //PUT请求方法
		EM_CONNECT,                        //CONNECT请求方法
		EM_OPTIONS,                        //OPTIONS请求方法
		EM_TRACE,                          //TRACE请求方法
		EM_METHOD_END                      //枚举保护
	}EM_HTTPMETHOD;

	static const char * const HttpMethod [] = 
	{
		"DELETE",
		"GET",
		"HEAD",
		"POST",
		"PUT",
		"CONNECT",
		"OPTIONS",
		"TRACEE"
	};
}

class HttpParser
{
	public:
		/**
		 * @brief 构造函数
		 */
		HttpParser();

		/**
		 * @brief 析构函数
		 */
		~HttpParser();

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
		
		/**
		 * @brief 解析器初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Init();

		/**
		 * @brief 解析器入口
		 *
		 * @prame pInputPkt 数据主体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Start(InputPacket *pInputPkt);

	private:
		/**
		 * @brief url解析回调
		 *
		 * @prame _parse 解析器; at 数据指针; uLength 数据长度
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t OnUrl(http_parser *_parse, const char* at, size_t uLength);

		/**
		 * @brief header解析回调(field)
		 *
		 * @prame _parse 解析器; at 数据指针; uLength 数据长度
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t OnHeaderField(http_parser *_parse, const char* at, size_t uLength);

		/**
		 * @brief header解析回调(value)
		 *
		 * @prame _parse 解析器; at 数据指针; uLength 数据长度
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t OnHeaderValue(http_parser *_parse, const char* at, size_t uLength);

		/**
		 * @brief body解析回调
		 *
		 * @prame _parse 解析器; at 数据指针; uLength 数据长度
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		static int32_t OnBody(http_parser *_parse, const char* at, size_t uLength);

	private:
		/**
		 * @brief http_parser
		 */
		http_parser _parser;

		/**
		 * @brief 解析器回调
		 */
		static http_parser_settings Settings;
};

#endif
