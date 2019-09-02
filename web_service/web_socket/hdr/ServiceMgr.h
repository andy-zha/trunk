#ifndef _SERVICEMGR_H_
#define _SERVICEMGR_H_

#include "MemoryDefine.h"
#include "RetCodeDefine.h"
#include "ServiceBase.h"

class ServiceMgr
{
	public:
		/**
		 * @brief 析构函数
		 */
		~ServiceMgr();

		/**
		 * @brief 去除拷贝构造函数
		 */
		ServiceMgr(ServiceMgr const&) = delete;

		/**
		 * @brief 去除重载=
		 */
		void operator=(ServiceMgr const&) = delete;

		/**
		 * @brief 获取单例
		 *
		 * @return 单例
		 */
		static ServiceMgr &GetInstance()
		{
			static ServiceMgr Instance;
			return Instance;
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

		/**
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功; RET::FAIL 失败
		 */
		int32_t Init();
		
		/**
		 * @brief 获取websocket服务
		 *
		 * @prame 服务id
		 *
		 * @return 服务
		 */ 
		ServiceBase *GetService(uint16_t uServiceId);

	private:
		/**
		 * @brief 构造函数
		 */
		ServiceMgr();

};

#endif
