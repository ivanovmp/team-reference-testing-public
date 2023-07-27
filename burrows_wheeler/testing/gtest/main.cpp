#include "gtest/gtest.h"

#include "../../../utils/testing.hpp"
#include "../../../suffix_array/suffix_array.hpp"
#include "../../burrows_wheeler.hpp"

using vf = vector<float>;
using vvf = vector<vf>;
TEST(Simplex, CheckSimple) {
    string s = "tyaaabaabaaabaabbbdfgsdfgsaetssaAABab";
    vector<pair<string, int>> s_shifts;
    const int n = s.size();
    for (int i = 0; i < n; ++i)
        s_shifts.emplace_back(s.substr(i, n - i) + s.substr(0, i), i);
    sort(s_shifts.begin(), s_shifts.end());
    string bwt;
    for (const auto& [t, _] : s_shifts)
        bwt.push_back(t.back());
    vi bwt_int(bwt.begin(), bwt.end());
    for (int i = 0; i < n; ++i) {
        vi t_int(s_shifts[i].x.begin(), s_shifts[i].x.end());
        auto [bwt_int_fast, bwt_pos_fast] = burrows_wheeler::transform(t_int, 255);
        string bwt_fast(bwt_int_fast.begin(), bwt_int_fast.end());
        ASSERT_EQ(bwt, bwt_fast) << "i = " << i;
        ASSERT_EQ(bwt_pos_fast, i);
        vi initial_int = burrows_wheeler::inverse_transform(bwt_int_fast, bwt_pos_fast, 255);
        string initial(initial_int.begin(), initial_int.end());
        ASSERT_EQ(initial, s_shifts[i].x) << "i = " << i;
    }
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
