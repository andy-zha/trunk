#include "PatternMgr.h"

/** 构造函数 **/
PatternMgr::PatternMgr()
{
	m_rexTotal = 0;
	int32_t jit = -1;
	pcre_free_study(nullptr);
	pcre_config(PCRE_CONFIG_JIT, &jit);
	if (1 == jit) {
		m_jitenabled = true;
	} else {
		m_jitenabled = false;
	}
}

/** 析构函数 **/
PatternMgr::~PatternMgr()
{}

/** 初始化接口 **/
int32_t PatternMgr::init()
{
	//读取正则表达式总个数
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_REX_TOTAL, m_rexTotal))
	{
		std::cout<<"PatternMgr: read rex total failed!"<<std::endl;
		return RET::FAIL;
	}

	uint16_t uIndex = 0;
	std::string path;
	std::string name;
	std::string pattern;
	std::string cfgFile = "../etc/pattern.json";
	while (uIndex < m_rexTotal) {
		path = std::to_string(uIndex);

		//读取正则名称
		const char *pNamePath[CFG_MAX_DEPTH] = {"NAME", path.c_str(), (const char*)0};
		if (RET::SUC != Config::getCfg(cfgFile, pNamePath, name))
		{
			uIndex++;
			continue;
		}

		//读取正则
		const char *pPatternPath[CFG_MAX_DEPTH] = {"PATTERN", path.c_str(), (const char*)0};
		if (RET::SUC != Config::getCfg(cfgFile, pPatternPath, pattern))
		{
			uIndex++;
			continue;
		}

		//正则入数据库
		uint16_t uPatId = 0;
		if (RET::SUC != writeToDb(name, pattern, uPatId)) {
			uIndex++;
			continue;
		}
	
		//赋值模式内容	
		NS_PATTERNMGR::Pattern pat;
		pat.m_name = name;
		pat.m_pattern = pattern;

		//编译正则
		const char *errptr = nullptr;
		int32_t erroffset = -1;
		pat.m_reg = pcre_compile(pattern.c_str(), PCRE_DOTALL|PCRE_MULTILINE,
						&errptr, &erroffset, nullptr);
		if (errptr) {
			free((char*)errptr);
			uIndex++;
			continue;
		}

		if (0 > pcre_fullinfo(pat.m_reg, nullptr, PCRE_INFO_CAPTURECOUNT, &pat.m_captures))
		{
			uIndex++;
			continue;
		}
	
		if (m_jitenabled) {
			pat.m_extra = pcre_study(pat.m_reg, PCRE_STUDY_JIT_COMPILE, &errptr);
		} else {
			pat.m_extra = pcre_study(pat.m_reg, 0, &errptr);
		}

		if (errptr) {
			free((char*)errptr);
			uIndex++;
			continue;
		}
		
		m_pattern[uPatId] = pat;
		uIndex++;
	}

	return RET::SUC;
}

int32_t PatternMgr::writeToDb(std::string name, std::string pattern, uint16_t &uPatId)
{
	return RET::SUC;
}
