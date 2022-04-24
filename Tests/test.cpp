#include "pch.h"

extern int Codin();

TEST(ProjectSetupTest, LinkCodinLibrary)
{
  EXPECT_EQ(Codin(), 0);
}
