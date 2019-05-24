#include "gtest/gtest.h"

GTEST_API_ int32_t main(int argc, char **argv)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
