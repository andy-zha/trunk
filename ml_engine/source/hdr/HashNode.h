#ifndef _HASHNODE_H_
#define _HASHNODE_H_

#include "RetCodeDefine.h"
#include "MemoryDefine.h"
#include <vector>
#include <map>

namespace NS_HASHNODE
{
	//正则表达式个数
	static const uint16_t REX_NUM = 6;
	//标准请求方法个数
	static const uint16_t METHOD_NUM = 9;

	/** 非标请求方法统计结点 **/
	class Method
	{
		public:
			/**
			 * @brief 构造函数
			 */
			Method()
			{
				m_total = 0;
			}

			/**
			 * @brief 析构函数
			 */
			~Method()
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
			std::string m_method;                     //请求方法
			uint32_t m_total;                         //请求方法统计次数
	};
	
	/** 请求参数统计结点 **/
	class Args
	{
		public:
			/**
			 * @brief 构造函数
			 */
			Args()
			{}

			/**
			 * @brief 析构函数
			 */
			~Args()
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
			std::string m_args;
			uint32_t m_argsType[NS_HASHNODE::REX_NUM];
			std::map<uint16_t, uint32_t> m_argsLen;
	};
}

class HashNode
{
	public:
		/**
		 * @brief 构造函数
		 */
		HashNode()
		{
			m_reqNum = 0;
		}

		/**
		 * @brief 析构函数
		 */
		virtual ~HashNode()
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
		uint32_t m_reqNum;                                           //url请求次数
		std::string m_urlId;                                         //url主键
		std::string m_url;                                           //url
		std::string m_businessId;                                    //业务主键
		std::string m_siteId;                                        //站点主键
		uint32_t m_methodList[NS_HASHNODE::METHOD_NUM];              //标准方法次数统计
		std::vector<NS_HASHNODE::Method> m_espMethodList;            //非标准请求方法统计
		std::vector<NS_HASHNODE::Args> m_reqlineArgs;                //请求行参数统计
		std::vector<NS_HASHNODE::Args> m_cookieArgs;                 //cookie参数统计
		std::vector<NS_HASHNODE::Args> m_reqbodyArgs;                //请求体参数统计 
};

#endif
