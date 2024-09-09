#include <gtest/gtest.h>
#include "PreemoRoot.h"

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	testing::FLAGS_gtest_brief = false;
	testing::FLAGS_gtest_print_time = true;
	return RUN_ALL_TESTS();
}


TEST(PreemoTest, SampleTest) {
	EXPECT_EQ(1, 1);
}

TEST(PreemoTest, TestRootInit) {
	preemo::Root root;
	EXPECT_TRUE(root.StartUp(nullptr));
	//EXPECT_TRUE(root.Initialize(nullptr));

	//EXPECT_EQ(1, 1);
}