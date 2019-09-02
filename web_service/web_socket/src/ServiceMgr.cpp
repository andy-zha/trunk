#include "ServiceMgr.h"

//构造函数
ServiceMgr::ServiceMgr()
{}

//析构函数
ServiceMgr::~ServiceMgr()
{}

int32_t ServiceMgr::Init()
{
	return RET::SUC;
}

ServiceBase *ServiceMgr::GetService(uint16_t uServiceId)
{
	return nullptr;
}
