#include "gtest/gtest.h"

#include "../../../utils/testing.hpp"
#include "../../../number_theory/NT.hpp"
#include "../../FFT.hpp"
#include "../../NTT.hpp"
#include "naiveFFT.hpp"
#include "naiveNTT.hpp"

FFT::NTT ntt;

TEST(NTT, CorrectPowersTest) {
    ASSERT_TRUE(NT::is_prime(ntt.MOD));
    ASSERT_EQ(1, ntt.MOD & (1 << ntt.n) - 1);
    ASSERT_EQ(ntt.ROOT, NT::binpow(ntt.g, ntt.MOD >> ntt.n, ntt.MOD));
    ASSERT_EQ(1, NT::binpow(ntt.ROOT, 1 << ntt.n, ntt.MOD));
    ASSERT_NE(1, NT::binpow(ntt.ROOT, 1 << ntt.n - 1, ntt.MOD));
}

void NTTcorrectAnswersCheck(const vector<int>& a, const vector<int>& b) {
    const int TRIES = 6;
    const int steps = 32 - __builtin_clz(max<int>(1, a.size()) + max<int>(1, b.size()) - 2);
    vi fast_ans = ntt.multiply(a, b, steps);
    vi indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(a.size() + b.size() - 2);
    indices.push_back(a.size() + b.size() - 1);
    indices.push_back(a.size() + b.size());
    int sz = max<int>(a.size() + b.size() - 1, 0);
    for (int index : choose_n_numbers(1, sz - 1, min(sz - 1, TRIES)))
        indices.push_back(index);
    for (int index : choose_n_numbers(sz, (1 << steps) - 1, min((1 << steps) - sz, TRIES)))
        indices.push_back(index);
    for (int index : indices) {
        int correct_ans = FFT::NaiveNTT::multiply_nth(a, b, index);
        int f_ans = index < fast_ans.size() ? fast_ans[index] : 0;
        ASSERT_EQ(correct_ans, f_ans) << "a.size() == " << a.size() << ", b.size() == " << b.size() << ", index == " << index;
    }
//    cerr << "Prod of " << a.size() << " and " << b.size() << " is correct on these indices:";
//    for (int i : indices)
//        cerr << ' ' << i;
//    cerr << '\n';
}

vi gen_int_vector(int n) {
    static mt19937 rng;
    uniform_int_distribution<int> seg(0, ntt.MOD - 1);
    vi ans(n);
    for (int i = 0; i < n; ++i)
        ans[i] = seg(rng);
    return ans;
}

void NTTcorrectAnswersCheck(const int n) {
    static mt19937 rng;
    int min_n = (1 << n - 1) + 1, max_n = 1 << n;
    for (int k = 1; k <= n; ++k) {
        int min_k = (1 << k - 1) + 1, max_k = 1 << k;
        for (int diff = 0; diff < 5; ++diff) {
            int real_k, real_n;
            if (n < 20) {
                uniform_int_distribution int_k(max(min_k, max_k - diff), max_k);
                uniform_int_distribution int_n(max(min_n, max_n - diff), max_n);
                real_k = int_k(rng);
                real_n = int_n(rng);
            } else {
                uniform_int_distribution seg(1, 1 << n);
                real_k = seg(rng);
                real_n = max(1, (1 << n) + 1 - real_k - diff);
            }
            vi vk = gen_int_vector(real_k), vn = gen_int_vector(real_n);
            NTTcorrectAnswersCheck(vk, vn);
            NTTcorrectAnswersCheck(vn, vk);
        }
    }
}

#define NTTCorrectAnswersTestMacro(bits) \
TEST(NTT, NTTCorrectAnswersTest##bits) { \
    NTTcorrectAnswersCheck(bits);        \
}

NTTCorrectAnswersTestMacro(1)
NTTCorrectAnswersTestMacro(2)
NTTCorrectAnswersTestMacro(3)
NTTCorrectAnswersTestMacro(4)
NTTCorrectAnswersTestMacro(5)
NTTCorrectAnswersTestMacro(6)
NTTCorrectAnswersTestMacro(7)
NTTCorrectAnswersTestMacro(8)
NTTCorrectAnswersTestMacro(9)
NTTCorrectAnswersTestMacro(10)
NTTCorrectAnswersTestMacro(11)
NTTCorrectAnswersTestMacro(12)
NTTCorrectAnswersTestMacro(13)
NTTCorrectAnswersTestMacro(14)
NTTCorrectAnswersTestMacro(15)
NTTCorrectAnswersTestMacro(16)
NTTCorrectAnswersTestMacro(17)
NTTCorrectAnswersTestMacro(18)
NTTCorrectAnswersTestMacro(19)
NTTCorrectAnswersTestMacro(20)

void NTTFastCheck(const int bits) {
    const auto t = 200ns, s = 1400ns;
    auto expected_time = [&](ul n) {
        return (n << n) * t + s;
    };
    const ul T = 3.5s / expected_time(bits);

    vector<pair<vi, vi>> datasets(T);
    for (int i = 0; i < T; ++i)
        datasets[i] = {gen_int_vector(1 << bits), gen_int_vector(1 << bits)};

    ld_nano et = expected_time(bits);
    vector<ld_nano> times;
    for (const auto& dataset : datasets) {
        auto start_time = chrono::high_resolution_clock::now();
        auto ans = ntt.multiply(dataset.first, dataset.second, bits + 1);
        auto end_time = chrono::high_resolution_clock::now();
        times.emplace_back(end_time - start_time);
        ASSERT_NE(ans[0], -1);
    }
    ld_nano actual_time = sample_time(times);
    cerr << "bits == " << bits << ": n lon n coefficient == " << actual_time / (bits << bits) / 1us << " us\n";

    cerr << "NNT multiplication for " << bits <<
         " bits (overall " << T << " tries) " <<
         " took " << actual_time <<
         " (expected " << et << ')' << endl;
    ASSERT_LE(actual_time / 1.ms, et / 1.ms);
}

#define NTTFastTestMacro(bits) \
TEST(NTT, NTTFastTest##bits) { \
    NTTFastCheck(bits);        \
}

NTTFastTestMacro(1)
NTTFastTestMacro(2)
NTTFastTestMacro(3)
NTTFastTestMacro(4)
NTTFastTestMacro(5)
NTTFastTestMacro(6)
NTTFastTestMacro(7)
NTTFastTestMacro(8)
NTTFastTestMacro(9)
NTTFastTestMacro(10)
NTTFastTestMacro(11)
NTTFastTestMacro(12)
NTTFastTestMacro(13)
NTTFastTestMacro(14)
NTTFastTestMacro(15)
NTTFastTestMacro(16)
NTTFastTestMacro(17)
NTTFastTestMacro(18)
NTTFastTestMacro(19)

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
