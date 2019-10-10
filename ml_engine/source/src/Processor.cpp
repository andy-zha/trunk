#include "Processor.h"

/** 构造函数 **/
Processor::Processor()
{
	m_run = false;
	m_queueMaxSize = 0;
	m_pktNull = 0;
	m_parserFailed = 0;
	m_execQueryFailed = 0;
	m_connectDbFailed = 0;
	m_proTotal = 0;
	m_proSuc = 0;
	m_deletePkt = 0;
}

/** 析构函数 **/
Processor::~Processor()
{
}

/** 初始化接口 **/
int32_t Processor::init()
{
	//http解析器初始化
	if (RET::SUC != _parser.init()) {
		std::cout<<"HttpParser init failed!"<<std::endl;
		return RET::FAIL;
	}

	int32_t iValue = 0;
	if (RET::SUC != Config::getCfg(NS_CONFIG::EM_CFGID_QUEUE_MAX_SIZE, iValue))
	{
		std::cout<<"Processor: Read queue max size failed!"<<std::endl;
		return RET::FAIL;
	}
	m_queueMaxSize = iValue < QUEUE_MAX_SIZE ? iValue : QUEUE_MAX_SIZE;

	return RET::SUC;
}

/** 启动接口 **/
int32_t Processor::start()
{
	//唤醒线程状态
	m_run = true;

	//启动线程
	m_thread = std::thread(std::bind(&Processor::process, this));

	return RET::SUC;
}

/** 线程处理函数 **/
void Processor::process()
{
	while (m_run) {
		//规则加载
		IPDRuleMgr::getInstance().process();

		//输出日志
		writeLog();

		//内存检查
//#ifdef _MEMCHECK_
//		memCheck();
//#endif

		//判断队列是否为空，空则线程休眠5秒
		if (_queue.empty()) {
			sleep(5);
			continue;
		}

		//取出数据体
		InputPacket *pInputPkt = _queue.front();
		m_proTotal++;

		//异常判断
		if (nullptr == pInputPkt) {
			_queue.pop();
			m_pktNull++;
			continue;
		}

		//解析
		if (RET::SUC != _parser.start(pInputPkt)) {
			_queue.pop();

			//释放内存
			if (nullptr != pInputPkt) {
				delete pInputPkt;
				pInputPkt = nullptr;
			}
			
			m_parserFailed++;
			continue;
		}

		_queue.pop();
		m_proSuc++;
		//释放内存
		if (nullptr != pInputPkt) {
			delete pInputPkt;
			pInputPkt = nullptr;	
		}
	}
}

/** 输出日志函数 **/
void Processor::writeLog()
{
	std::ofstream io;
	time_t nowtime = time(NULL);
	struct tm *local = localtime(&nowtime);
	std::string log_path = "/data/logs/ml_engine/ml_" 
			+ std::to_string(local->tm_year + 1900) + "."
			+ std::to_string(local->tm_mon + 1) + "."
			+ std::to_string(local->tm_mday) + "_"
			+ std::to_string(getpid()) + ".log";
	io.open(log_path, std::ios::app);

	//输出日志头
	char header[1024];
	snprintf(header, 1024, "ML_ENGINE[%d] %04d/%02d/%02d %02d:%02d:%02d", getpid(),
					local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
					local->tm_hour, local->tm_min, local->tm_sec);
	io<<header<<std::endl;

	std::string pro_log;
	//输出主处理模块日志
	sprintfLogStream(pro_log);
	io<<pro_log<<std::endl;
	//输出接收模块日志
	Accepter::getInstance().sprintfLogStream(pro_log);
	io<<pro_log<<std::endl;
	
	io<<std::endl<<std::endl;
	io.close();
}

/** 线程join **/
void Processor::threadJoin()
{
	m_thread.join();
}

/** push数据到缓冲区 **/
int32_t Processor::pushData(InputPacket *pInputPkt)
{
	if (_queue.size() > m_queueMaxSize) {
		m_deletePkt++;
		return RET::FAIL;
	}

	_queue.push(pInputPkt);
	return RET::SUC;
}

/** 输出日志接口 **/
void Processor::sprintfLogStream(std::string &log)
{
	log = "Processor: ProTotal[" + std::to_string(m_proTotal)
			+ "] ProSuc[" + std::to_string(m_proSuc)
			+ "] ParserFailed[" + std::to_string(m_parserFailed)
			+ "] PktNull[" + std::to_string(m_pktNull)
			+ "] DeletePkt[" + std::to_string(m_deletePkt)
			+ "] ConnectDbFailed[" + std::to_string(m_connectDbFailed)
			+ "] ExecQueryFailed[" + std::to_string(m_execQueryFailed)
			+ "]";
}

/** 内存检测函数 **/
#ifdef _MEMCHECK_

void Processor::memCheck()
{
	DbAdmin m_db;
	//数据库连接
	if (RET::SUC != m_db.connect()) {
		m_connectDbFailed++;
		return;
	}
	
	//查询工具开关状态
	MYSQL_RES *pResult = nullptr;
	std::string Sql = "SELECT memcheck FROM cmd;";
	if (RET::SUC != m_db.execQuery(Sql, pResult))
	{
		m_execQueryFailed++;
		m_db.close();
		return;
	}

	//判断开关
	if (nullptr != pResult && 0 != mysql_num_rows(pResult)
					&& 1 == mysql_num_fields(pResult))
	{
		MYSQL_ROW row = mysql_fetch_row(pResult);
		if (1 == std::stoul(row[0]))
		{
			MemCheck::GetInstance().WriteLog();
			Sql = "UPDATE cmd SET memcheck = 0;";
			m_db.execSql(Sql);
		}
	}

	mysql_free_result(pResult);
	pResult = nullptr;	
	m_db.close();
}

#endif
