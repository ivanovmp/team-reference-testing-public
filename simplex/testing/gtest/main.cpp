#include "gtest/gtest.h"

#include "../../../utils/testing.hpp"
#include "../../simplex.hpp"

using vf = vector<float>;
using vvf = vector<vf>;
TEST(Simplex, CheckSimpleOptimum) {
    vvf A = {{3, 1}, {1, 2}};
    vf B = {9, 8};
    vf C = {1, 1};
    Simplex<float> simplex(A, B, C, 1e-7);
    vf x; float ans = simplex.solve(x);
    GTEST_ASSERT_EQ(ans, 5);
    GTEST_ASSERT_EQ(x, vf({2., 3.}));
}

TEST(Simplex, CheckSimpleUnbounded) {
    vvf A = {{0, 0}};
    vf B = {1};
    vf C = {1, 1};
    Simplex<float> simplex(A, B, C, 1e-7);
    vf x; float ans = simplex.solve(x);
    GTEST_ASSERT_EQ(ans, 1./0.);
    GTEST_ASSERT_EQ(x, vf(0));
}

TEST(Simplex, CheckSimpleInfeasible) {
    vvf A = {{0, 0}};
    vf B = {-1};
    vf C = {1, 1};
    Simplex<float> simplex(A, B, C, 1e-7);
    vf x; float ans = simplex.solve(x);
    GTEST_ASSERT_EQ(ans, -1./0.);
    GTEST_ASSERT_EQ(x, vf(0));
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
