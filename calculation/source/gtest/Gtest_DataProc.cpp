#include "gtest/gtest.h"
#include "DataProc.h"

namespace {
    class Test_DataProc : public testing::Test
	{
	    void SetUp() override{};

		void TearDown() override{};
	};
}

TEST_F(Test_DataProc, GTest_DataProc)
{
    uint32_t **Metrix = NULL;
	Metrix = new uint32_t* [3];
	for (uint32_t i = 0; i < 3; i++)
	{
		Metrix[i] = new uint32_t [3];
	}
	Metrix[0][0] = 10;
	Metrix[0][1] = 5;
	Metrix[0][2] = 5;
	Metrix[1][0] = 2;
	Metrix[1][1] = 35;
	Metrix[1][2] = 2;
	Metrix[2][0] = 8;
	Metrix[2][1] = 5;
	Metrix[2][2] = 15;
	ASSERT_FLOAT_EQ(0.41999999, DataProc::GetKappa(Metrix, 3, 3));

	for (uint32_t i = 0; i < 3; i++)
	{
		delete [] Metrix[i];
	}
	delete [] Metrix;
}
