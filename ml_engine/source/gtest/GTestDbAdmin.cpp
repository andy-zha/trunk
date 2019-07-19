#include "DbAdmin.h"
#include "gtest/gtest.h"

namespace 
{
	class TestDbAdmin : public testing::Test
	{
		void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

TEST_F(TestDbAdmin, Test_DbAdmin)
{
	DbAdmin m_DbAdmin;
	ASSERT_EQ(RET::SUC, m_DbAdmin.Connect());

	std::string Sql = "CREATE TABLE test(t int);";
	ASSERT_EQ(RET::SUC, m_DbAdmin.ExecSql(Sql));

	MYSQL_RES *pResult = NULL;
	Sql = "SELECT * FROM test;";
	ASSERT_EQ(RET::SUC, m_DbAdmin.ExecQuery(Sql, pResult));
	
	ASSERT_EQ(RET::SUC, m_DbAdmin.Close());
}
