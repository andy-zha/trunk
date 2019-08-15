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

	std::string Sql = "CREATE TABLE IF NOT EXISTS test(t int);";
	ASSERT_EQ(RET::SUC, m_DbAdmin.ExecSql(Sql));

	MYSQL_RES *pResult = nullptr;
	Sql = "SELECT * FROM test;";
	ASSERT_EQ(RET::SUC, m_DbAdmin.ExecQuery(Sql, pResult));

	std::string str = "\"DROP TABLE test;";
	Sql = "UPDATE trustip SET ip = '" + str + "' WHERE b_id = 1";
	ASSERT_EQ(RET::SUC, m_DbAdmin.ExecSql(Sql));

	std::string str1 = "'dsas'; 'dsdas' DROP TABLE test;";
	Sql = "UPDATE trustip SET b_id = 2 WHERE ip = '" + str1 + "';";
	ASSERT_EQ(RET::FAIL, m_DbAdmin.ExecSql(Sql));

	std::string str2 = "dasdsad;' das;' DROP TABLE test;";
	Sql = "UPDATE untrustip SET ip = ? WHERE b_id = 1;";
	std::vector<std::string> Vec;
	Vec.push_back(str2);
	ASSERT_EQ(RET::SUC, m_DbAdmin.StmtExecSql(Sql, Vec));

	Sql = "UPDATE untrustip SET b_id = 3 WHERE ip = ?;";
	ASSERT_EQ(RET::SUC, m_DbAdmin.StmtExecSql(Sql, Vec));

	Sql = "SELECT ip FROM untrustip WHERE ip = ?;";
	std::vector<std::vector<std::string>> Res;
	ASSERT_EQ(RET::SUC, m_DbAdmin.StmtExecQuery(Sql, Vec, 1, Res));
//	std::vector<std::vector<std::string>>::iterator iter = Res.begin();
	ASSERT_STREQ(str2.c_str(), Res[0][0].c_str());

	ASSERT_EQ(RET::SUC, m_DbAdmin.Close());
}
