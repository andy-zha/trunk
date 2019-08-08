#include "ConsoleMgr.h"

//构造函数
ConsoleMgr::ConsoleMgr()
{}

//析构函数
ConsoleMgr::~ConsoleMgr()
{}

//初始化接口
int32_t ConsoleMgr::Init()
{
	//数据库连接
	if (RET::SUC != m_db.Connect())
	{
		return RET::FAIL;
	}
	
	std::string Sql = "CREATE table IF NOT EXISTS cmd(id int, memcheck int);";
	m_db.ExecSql(Sql);
	Sql = "INSERT INTO cmd (id, memcheck) SELECT 1, 0 FROM DUAL WHERE NOT \
		   EXISTS (SELECT memcheck FROM cmd WHERE id = 1);";
	m_db.ExecSql(Sql);

	return RET::SUC;
}

//工具入口
int32_t ConsoleMgr::Start()
{
	std::string cmd;
	std::cout<<"[*********************************************************]"<<std::endl;
	std::cout<<"[*****************                     *******************]"<<std::endl;  
	std::cout<<"[               MACHINE LEARNING CONSOLE                  ]"<<std::endl;
	std::cout<<"[ Boot: ./ml_console -args                                ]"<<std::endl;
	std::cout<<"[ args: -help|-h: get tips                                ]"<<std::endl;
	std::cout<<"[*********************************************************]"<<std::endl;
	std::cout<<"[andy@ml_console ~]# ";
	while (true)
	{
		std::cin>>cmd;
		if (cmd == std::string("memcheck"))
		{
			MemCheckConsole();
		}
		else if (cmd == std::string("quit"))
		{
			break;
		}
		else
		{
			std::cout<<"[usage]: try 'help' to get tips"<<std::endl;
			std::cout<<"[andy@ml_console ~]# ";
		} 
	}
	return RET::SUC;
}

void ConsoleMgr::MemCheckConsole()
{
	std::string cmd;
	std::cout<<"[andy@ml_console memcheck]# ";
	while (true)
	{
		std::cin>>cmd;
		if (cmd == std::string("help"))
		{
			std::cout<<"[MEMCHECK USER COMMANDS] "<<std::endl;
			std::cout<<"[COMMANDS]"<<std::endl;
			std::cout<<"       log | help"<<std::endl;
			std::cout<<"[DESCRIPTION]"<<std::endl; 
			std::cout<<"       log: write log to file;"<<std::endl;
			std::cout<<"       help: to get tips."<<std::endl;
			std::cout<<std::endl;	
			std::cout<<"[andy@ml_console memcheck]# ";
		}
		else if (cmd == std::string("log"))
		{
			std::string Sql = "UPDATE cmd SET memcheck = 1;";
			m_db.ExecSql(Sql);
			std::cout<<"Generate log file within 30s(Executable directory)."<<std::endl;
			std::cout<<"[andy@ml_console memcheck]# ";
		}
		else if (cmd == std::string("quit"))
		{
			std::cout<<"[andy@ml_console ~]# ";
			return;
		}
		else
		{
			std::cout<<"[usage]: try 'help' to get tips"<<std::endl;
			std::cout<<"[andy@ml_console memcheck]# ";
		} 
	}

	m_db.Close();

	return;
}
