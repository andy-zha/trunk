#ifndef _HTTPPARSER_H_
#define _HTTPPARSER_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include "InputPacket.h"
#include "IPDRuleMgr.h"
#include "Timer.h"
#include "picohttpparser.h"
#include "StrProc.h"

namespace NS_HTTPPARSER
{
	//host
	static const std::string HOST = "Host";
	//cookie
	static const std::string COOKIE = "Cookie";
	//Content-Type
	static const std::string CONTENT_TYPE = "Content-Type";

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

#ifdef _GTEST_
	public:
#else
	private:
#endif
		/**
		 * @brief 解析uri
		 *
		 * @prame uri; pInputPkt 包体 
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ParserUri(std::string uri, InputPacket *pInputPkt);
	
		/**
		 * @brief 过滤url请求参数
		 *
		 * @prame url
		 *
		 * @return 过滤结果
		 */
		std::string FilterUrl(std::string url);

		/**
		 * @brief 解析query
		 *
		 * @prame query
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ParserQuery(std::string query);

		/**
		 * @brief 解析cookie
		 *
		 * @prame cookie
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ParserCookie(std::string cookie);

		/**
		 * @brief 解析http body
		 *
		 * @prame http_body http请求体
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t ParserBody(std::string http_body);

	private:
		/**
		 * @brief 数据库操作对象
		 */
		DbAdmin db;
};

#endif
