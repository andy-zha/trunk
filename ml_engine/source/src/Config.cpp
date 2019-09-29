#include "Config.h"

//构造函数
Config::Config()
{}

//析构函数
Config::~Config()
{}

//获取配置值
int32_t Config::getCfg(NS_CONFIG::EM_CFGID CfgId, int32_t &iValue)
{
	//异常判断
	if (CfgId >= NS_CONFIG::EM_CFGID_END 
					|| CfgId <= NS_CONFIG::EM_CFGID_START
					|| CfgId != g_stCfgInfo[CfgId].CfgId)
	{
		return RET::FAIL;
	}

	//读取文件流
	yajl_val node;
	char errbuf[2048] = {0};
	try
	{
		std::string Str = GetStringFromFile(g_stCfgInfo[CfgId].filename);
		node = yajl_tree_parse(Str.data(), errbuf, sizeof(errbuf));
		if (nullptr == node)
		{
			if (strlen(errbuf))
			{
				std::cout<<errbuf<<std::endl;
			}
			else
			{
				std::cout<<"unknown error!"<<std::endl;
			}
			return RET::FAIL;
		}
	}
	catch(...)
	{
		return RET::FAIL;
	}

	//解析json树
	yajl_val var = yajl_tree_get(node, 
					const_cast<const char**>(g_stCfgInfo[CfgId].cfgpath), 
					yajl_t_number);
	if (var && YAJL_IS_INTEGER(var))
	{
		iValue = YAJL_GET_INTEGER(var);
		yajl_tree_free(node);
		return RET::SUC;
	}

	yajl_tree_free(node);
	return RET::FAIL;
}

//获取配置值(string)
int32_t Config::getCfg(NS_CONFIG::EM_CFGID CfgId, std::string &Value)
{
	//异常判断
	if (CfgId >= NS_CONFIG::EM_CFGID_END 
					|| CfgId <= NS_CONFIG::EM_CFGID_START
					|| CfgId != g_stCfgInfo[CfgId].CfgId)
	{
		return RET::FAIL;
	}

	//读取文件流
	yajl_val node;
	char errbuf[2048] = {0};
	try
	{
		std::string Str = GetStringFromFile(g_stCfgInfo[CfgId].filename);
		node = yajl_tree_parse(Str.data(), errbuf, sizeof(errbuf));
		if (nullptr == node)
		{
			if (strlen(errbuf))
			{
				std::cout<<errbuf<<std::endl;
			}
			else
			{
				std::cout<<"unknown error!"<<std::endl;
			}
			return RET::FAIL;
		}
	}
	catch(...)
	{
		return RET::FAIL;
	}

	//解析json树
	yajl_val var = yajl_tree_get(node, 
					const_cast<const char**>(g_stCfgInfo[CfgId].cfgpath), 
					yajl_t_string);
	if (var && YAJL_IS_STRING(var))
	{
		Value = std::string(YAJL_GET_STRING(var));
		yajl_tree_free(node);
		return RET::SUC;
	}

	yajl_tree_free(node);
	return RET::FAIL;
}

//获取文件流
std::string Config::GetStringFromFile(std::string filename)
{
	std::ifstream it(filename);
	std::string str((std::istreambuf_iterator<char>(it)), std::istreambuf_iterator<char>());
	it.close();
	return str;
}
