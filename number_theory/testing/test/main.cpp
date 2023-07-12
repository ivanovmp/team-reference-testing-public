#include "gtest/gtest.h"

#include "../../../utils/testing.hpp"
#include "../../NT.hpp"
#include "naiveNT.hpp"

void eratosthenes_factor_test(const int bits) {
	const int N = 1000000,
	          T = 2000,
			  L = 1 << (bits - 1),
              R = (L << 1) - 1;
	NT::Eratosthenes e(N);
	static mt19937_64 rng;
	uniform_int_distribution<int> seg(L, R);
	for (int j = 0; j < T; ++j) {
		int n = T >= R - L + 1 ? j + L : seg(rng);
        if (n > R) break;
		GTEST_ASSERT_EQ(NaiveNT::factor(n), e.factor(n));
	}
}


#define EratosthenesFactorTest(bits)           \
TEST(NumberTheory, EratosthenesFactor##bits) { \
    eratosthenes_factor_test(bits);            \
}

EratosthenesFactorTest(1)
EratosthenesFactorTest(2)
EratosthenesFactorTest(3)
EratosthenesFactorTest(4)
EratosthenesFactorTest(5)
EratosthenesFactorTest(6)
EratosthenesFactorTest(7)
EratosthenesFactorTest(8)
EratosthenesFactorTest(9)
EratosthenesFactorTest(10)
EratosthenesFactorTest(11)
EratosthenesFactorTest(12)
EratosthenesFactorTest(13)
EratosthenesFactorTest(14)
EratosthenesFactorTest(15)
EratosthenesFactorTest(16)
EratosthenesFactorTest(17)
EratosthenesFactorTest(18)
EratosthenesFactorTest(19)
EratosthenesFactorTest(20)
EratosthenesFactorTest(21)
EratosthenesFactorTest(22)
EratosthenesFactorTest(23)
EratosthenesFactorTest(24)
EratosthenesFactorTest(25)
EratosthenesFactorTest(26)
EratosthenesFactorTest(27)
EratosthenesFactorTest(28)
EratosthenesFactorTest(29)
EratosthenesFactorTest(30)

void eratosthenes_fast_construction_test() {
    auto start_time = chrono::high_resolution_clock::now();
    NT::Eratosthenes e(4e7);
    auto end_time = chrono::high_resolution_clock::now();
    const auto TL = 4.s;
    GTEST_ASSERT_LE((end_time - start_time) / 1.ms, TL / 1.ms);
}

TEST(NumberTheory, EratosthenesFastConstruction) {
    eratosthenes_fast_construction_test();
}

void eratosthenes_fast_factor_test(const int bits) {
    const int N = 100000,
			  L = 1 << (bits - 1),
              R = (L << 1) - 1;
	const auto t = 150ns, s = 2500ns;
	auto expected_time = [&](const int n) {
		return t * sqrtl(n) + s;
	};
	const int T = .5s / expected_time(R);
	NT::Eratosthenes e(N);
	uniform_int_distribution<ul> seg(L, R);
	static mt19937_64 rng;
    vector<ld_nano> times;
	for (int i = 0; i < T; ++i) {
        int n = seg(rng);
        auto start_time = chrono::high_resolution_clock::now();
        e.factor(n);
        auto end_time = chrono::high_resolution_clock::now();
        times.push_back(end_time - start_time);
	}
    ld_nano length = sample_time(times);
    ld_nano et = expected_time(R);
    cerr << "Eratosthenes Factorization for " << bits <<
        " bits (overall " << T << " tries) " <<
        " took " << length <<
        " (expected " << et << ')' << endl;
	GTEST_ASSERT_LE(length / 1.ms, et / 1.ms);
}

#define EratosthenesFastFactorTest(bits)                  \
TEST(NumberTheory, EratosthenesFastFactorization##bits) { \
    eratosthenes_fast_factor_test(bits);                  \
}

EratosthenesFastFactorTest(1)
EratosthenesFastFactorTest(2)
EratosthenesFastFactorTest(3)
EratosthenesFastFactorTest(4)
EratosthenesFastFactorTest(5)
EratosthenesFastFactorTest(6)
EratosthenesFastFactorTest(7)
EratosthenesFastFactorTest(8)
EratosthenesFastFactorTest(9)
EratosthenesFastFactorTest(10)
EratosthenesFastFactorTest(11)
EratosthenesFastFactorTest(12)
EratosthenesFastFactorTest(13)
EratosthenesFastFactorTest(14)
EratosthenesFastFactorTest(15)
EratosthenesFastFactorTest(16)
EratosthenesFastFactorTest(17)
EratosthenesFastFactorTest(18)
EratosthenesFastFactorTest(19)
EratosthenesFastFactorTest(20)
EratosthenesFastFactorTest(21)
EratosthenesFastFactorTest(22)
EratosthenesFastFactorTest(23)
EratosthenesFastFactorTest(24)
EratosthenesFastFactorTest(25)
EratosthenesFastFactorTest(26)
EratosthenesFastFactorTest(27)
EratosthenesFastFactorTest(28)
EratosthenesFastFactorTest(29)
EratosthenesFastFactorTest(30)

void deterministic_miller_rabin_primality(const int bits) {
	const ul L = ul(1) << (bits - 1),
             R = (L << 1) - 1;
	static mt19937_64 rng;
	const auto t = 40ns, s = 1000ns;
	auto expected_time = [&](ul n) {
        return t * sqrtl(n) + s;
	};
	const ul T = .5s / expected_time(R);
    //cerr << '[' << T << " tests]" << endl;
	uniform_int_distribution<ul> seg(L, R);
	for (int j = 0; j < T; ++j) {
		ul n = T >= R - L + 1 ? j + L : seg(rng);
        if (n > R) break;
		GTEST_ASSERT_EQ(NaiveNT::is_prime(n),
			NT::is_prime(n));
	}
}

#define DeterministicMillerRabinPrimality(bits)  \
TEST(NumberTheory, MillerRabinPrimality##bits) { \
    deterministic_miller_rabin_primality(bits);  \
}

DeterministicMillerRabinPrimality(1)
DeterministicMillerRabinPrimality(2)
DeterministicMillerRabinPrimality(3)
DeterministicMillerRabinPrimality(4)
DeterministicMillerRabinPrimality(5)
DeterministicMillerRabinPrimality(6)
DeterministicMillerRabinPrimality(7)
DeterministicMillerRabinPrimality(8)
DeterministicMillerRabinPrimality(9)
DeterministicMillerRabinPrimality(10)
DeterministicMillerRabinPrimality(11)
DeterministicMillerRabinPrimality(12)
DeterministicMillerRabinPrimality(13)
DeterministicMillerRabinPrimality(14)
DeterministicMillerRabinPrimality(15)
DeterministicMillerRabinPrimality(16)
DeterministicMillerRabinPrimality(17)
DeterministicMillerRabinPrimality(18)
DeterministicMillerRabinPrimality(19)
DeterministicMillerRabinPrimality(20)
DeterministicMillerRabinPrimality(21)
DeterministicMillerRabinPrimality(22)
DeterministicMillerRabinPrimality(23)
DeterministicMillerRabinPrimality(24)
DeterministicMillerRabinPrimality(25)
DeterministicMillerRabinPrimality(26)
DeterministicMillerRabinPrimality(27)
DeterministicMillerRabinPrimality(28)
DeterministicMillerRabinPrimality(29)
DeterministicMillerRabinPrimality(30)
DeterministicMillerRabinPrimality(31)
DeterministicMillerRabinPrimality(32)
DeterministicMillerRabinPrimality(33)
DeterministicMillerRabinPrimality(34)
DeterministicMillerRabinPrimality(35)
DeterministicMillerRabinPrimality(36)
DeterministicMillerRabinPrimality(37)
DeterministicMillerRabinPrimality(38)
DeterministicMillerRabinPrimality(39)
DeterministicMillerRabinPrimality(40)
DeterministicMillerRabinPrimality(41)
DeterministicMillerRabinPrimality(42)
DeterministicMillerRabinPrimality(43)
DeterministicMillerRabinPrimality(44)

ul minimum_prime_at_most(ul R) {
    while (!NT::is_prime(R)) --R;
    return R;
}

ul generate_prime_number(const ul L, ul R) {
    R = minimum_prime_at_most(max<ul>(R, 2));
    if (L > R) return R;
    static mt19937_64 rng;
    uniform_int_distribution<ul> seg(L, R);
    while (true) {
        ul ans = seg(rng);
        if (NT::is_prime(ans))
            return ans;
    }
}

vu generate_prime_primality_dataset(const ul L, const ul R, const ul n) {
    vu ans(n);
    for (int i = 0; i < n; ++i)
        ans[i] = generate_prime_number(L, R);
    return ans;
}

vu generate_square_primality_dataset(const ul L, const ul R, const ul n) {
    vu ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = generate_prime_number(NT::isqrt(L - 1) + 1, NT::isqrt(R));
        ans[i] *= ans[i];
    }
    return ans;
}

vu generate_halfprime_primality_dataset(const ul L, const ul R, const ul n) {
    if (R <= 3) return vu(n, R);
    vu ans(n);
    ul realL = NT::isqrt(L / 2), realR = ul(2) * NT::isqrt(R / 2 + 1);
    for (int i = 0; i < n; ++i) {
        while (true) {
            ul p1 = generate_prime_number(realL, realR), p2 = generate_prime_number(realL, realR);
            if (R / p1 < p2) continue;
            if (p1 * p2 < L) continue;
            ans[i] = p1 * p2;
            break;
        }
    }
    return ans;
}

vu generate_random_dataset(const ul L, const ul R, const ul n) {
    static mt19937_64 rng;
    uniform_int_distribution<ul> seg(L, R);
    vu ans(n);
    for (int i = 0; i < n; ++i)
        ans[i] = seg(rng);
    return ans;
}

vu generate_random_primality_dataset(const ul L, const ul R, const ul n) {
    return generate_random_dataset(L, R, n);
}

void compare_miller_rabin_primality(const vu& dataset,
                                    const string& dataset_name,
                                    const ld_nano et_det,
                                    const ld_nano et_ran,
                                    const int bits) {
    vector<ld_nano> times_det;
    vector<bool> ans_det;
    for (ul n : dataset) {
        auto start_time = chrono::high_resolution_clock::now();
        bool ans = NT::deterministic_miller_rabin(n);
        auto end_time = chrono::high_resolution_clock::now();
        times_det.push_back(end_time - start_time);
        ans_det.push_back(ans);
    }

    vector<ld_nano> times_ran;
    vector<bool> ans_ran;
    for (ul n : dataset) {
        auto start_time = chrono::high_resolution_clock::now();
        bool ans = NT::miller_rabin(n);
        auto end_time = chrono::high_resolution_clock::now();
        times_ran.push_back(end_time - start_time);
        ans_ran.push_back(ans);
    }

    for (int i = 0; i < dataset.size(); ++i)
        GTEST_ASSERT_EQ(ans_det[i], ans_ran[i]);

    const ld_nano length_det = sample_time(times_det),
                  length_ran = sample_time(times_ran);
    cerr << "Deterministic Miller-Rabin for " << bits <<
         " bits (overall " << dataset.size() << " tries)" <<
         " on " << dataset_name << " took " << length_det <<
         " (expected " << et_det << ')' << endl;
    cerr << "Random Miller-Rabin for " << bits <<
         " bits (overall " << dataset.size() << " tries)" <<
         " on " << dataset_name << " took " << length_ran <<
         " (expected " << et_ran << ')' << endl;
    GTEST_ASSERT_LE(length_det / 1.ms, et_det / 1.ms);
    GTEST_ASSERT_LE(length_ran / 1.ms, et_ran / 1.ms);
}

void deterministic_and_random_miller_rabin_primality(const int bits) {
    const ul L = ul(1) << (bits - 1),
             R = (L << 1) - 1;
    const auto t_det = .3ns, s_det = 11000.ns;
    const auto t_ran = 2.5ns, s_ran = 21000.ns;
    auto expected_time_det = [&](ul n) {
        return t_det * pow(log(n + 1), 3) + s_det;
    };
    auto expected_time_ran = [&](ul n) {
        return t_ran * pow(log(n + 1), 3) + s_ran;
    };
    ld_nano et_det = expected_time_det(R),
            et_ran = expected_time_ran(R);
    const ul T = .2s / et_ran;
    vu pd = generate_prime_primality_dataset(L, R, T),
       sd = generate_square_primality_dataset(L, R, T),
       hd = generate_halfprime_primality_dataset(L, R, T),
       rd = generate_random_primality_dataset(L, R, T);
    compare_miller_rabin_primality(pd, "prime dataset", et_det, et_ran, bits);
    compare_miller_rabin_primality(sd, "square dataset", et_det, et_ran, bits);
    compare_miller_rabin_primality(hd, "halfprime dataset", et_det, et_ran, bits);
    compare_miller_rabin_primality(rd, "random dataset", et_det, et_ran, bits);
}

#define zDeterministicAndRandomMillerRabinPrimality(bits)   \
TEST(NumberTheory, CompareMillerRabinPrimality##bits) {    \
    deterministic_and_random_miller_rabin_primality(bits); \
}

DeterministicAndRandomMillerRabinPrimality(1)
DeterministicAndRandomMillerRabinPrimality(2)
DeterministicAndRandomMillerRabinPrimality(3)
DeterministicAndRandomMillerRabinPrimality(4)
DeterministicAndRandomMillerRabinPrimality(5)
DeterministicAndRandomMillerRabinPrimality(6)
DeterministicAndRandomMillerRabinPrimality(7)
DeterministicAndRandomMillerRabinPrimality(8)
DeterministicAndRandomMillerRabinPrimality(9)
DeterministicAndRandomMillerRabinPrimality(10)
DeterministicAndRandomMillerRabinPrimality(11)
DeterministicAndRandomMillerRabinPrimality(12)
DeterministicAndRandomMillerRabinPrimality(13)
DeterministicAndRandomMillerRabinPrimality(14)
DeterministicAndRandomMillerRabinPrimality(15)
DeterministicAndRandomMillerRabinPrimality(16)
DeterministicAndRandomMillerRabinPrimality(17)
DeterministicAndRandomMillerRabinPrimality(18)
DeterministicAndRandomMillerRabinPrimality(19)
DeterministicAndRandomMillerRabinPrimality(20)
DeterministicAndRandomMillerRabinPrimality(21)
DeterministicAndRandomMillerRabinPrimality(22)
DeterministicAndRandomMillerRabinPrimality(23)
DeterministicAndRandomMillerRabinPrimality(24)
DeterministicAndRandomMillerRabinPrimality(25)
DeterministicAndRandomMillerRabinPrimality(26)
DeterministicAndRandomMillerRabinPrimality(27)
DeterministicAndRandomMillerRabinPrimality(28)
DeterministicAndRandomMillerRabinPrimality(29)
DeterministicAndRandomMillerRabinPrimality(30)
DeterministicAndRandomMillerRabinPrimality(31)
DeterministicAndRandomMillerRabinPrimality(32)
DeterministicAndRandomMillerRabinPrimality(33)
DeterministicAndRandomMillerRabinPrimality(34)
DeterministicAndRandomMillerRabinPrimality(35)
DeterministicAndRandomMillerRabinPrimality(36)
DeterministicAndRandomMillerRabinPrimality(37)
DeterministicAndRandomMillerRabinPrimality(38)
DeterministicAndRandomMillerRabinPrimality(39)
DeterministicAndRandomMillerRabinPrimality(40)
DeterministicAndRandomMillerRabinPrimality(41)
DeterministicAndRandomMillerRabinPrimality(42)
DeterministicAndRandomMillerRabinPrimality(43)
DeterministicAndRandomMillerRabinPrimality(44)
DeterministicAndRandomMillerRabinPrimality(45)
DeterministicAndRandomMillerRabinPrimality(46)
DeterministicAndRandomMillerRabinPrimality(47)
DeterministicAndRandomMillerRabinPrimality(48)
DeterministicAndRandomMillerRabinPrimality(49)
DeterministicAndRandomMillerRabinPrimality(50)
DeterministicAndRandomMillerRabinPrimality(51)
DeterministicAndRandomMillerRabinPrimality(52)
DeterministicAndRandomMillerRabinPrimality(53)
DeterministicAndRandomMillerRabinPrimality(54)
DeterministicAndRandomMillerRabinPrimality(55)
DeterministicAndRandomMillerRabinPrimality(56)
DeterministicAndRandomMillerRabinPrimality(57)
DeterministicAndRandomMillerRabinPrimality(58)
DeterministicAndRandomMillerRabinPrimality(59)
DeterministicAndRandomMillerRabinPrimality(60)
DeterministicAndRandomMillerRabinPrimality(61)
DeterministicAndRandomMillerRabinPrimality(62)
DeterministicAndRandomMillerRabinPrimality(63)
DeterministicAndRandomMillerRabinPrimality(64)

vu generate_square_isqrt_dataset(const ul L, const ul R, const ul n) {
    vu ans = generate_random_dataset(NaiveNT::usqrt(L), NaiveNT::isqrt(R), n);
    for (ul i = 0; i < n; ++i)
        ans[i] *= ans[i];
    return ans;
}

vu generate_square_minus_one_isqrt_dataset(const ul L, const ul R, const ul n) {
    ul RL = NaiveNT::usqrt(L + 1) - 1, RR = NaiveNT::isqrt(R + 1) - 1;
    if (RR < RL)
        return generate_random_dataset(L, R, n);
    vu ans = generate_random_dataset(RL, RR, n);
    for (ul i = 0; i < n; ++i)
        ans[i] = (ans[i] + 1) * (ans[i] + 1) - 1;
    return ans;
}

vu generate_random_isqrt_dataset(const ul L, const ul R, const ul n) {
    return generate_random_dataset(L, R, n);
}

vu generate_near_square_isqrt_dataset(const ul L, const ul R, const ul n) {
    const ul RL = max<ul>(NaiveNT::usqrt(L), 1) - 1, RR = min<ul>(NaiveNT::isqrt(R), -2) + 1;
    uniform_int_distribution<ul> seg_big(RL, RR);
    uniform_int_distribution<ll> seg_small(-4, 4);
    static mt19937_64 rng;
    vu ans(n);
    for (int i = 0; i < n; ++i) {
        while (ans[i] < L || ans[i] > R) {
            ans[i] = seg_big(rng);
            ans[i] = ans[i] * ans[i] + seg_small(rng);
        }
    }
    return ans;
}

void check_isqrt_approx_results(const vu& dataset,
                         const string& dataset_name,
                         const ld_nano et,
                         const int bits) {
    vector<ld_nano> times;
    vector<ui> anses;
    for (ul n : dataset) {
        auto start_time = chrono::high_resolution_clock::now();
        ui ans = NT::isqrt_approx(n);
        auto end_time = chrono::high_resolution_clock::now();
        times.push_back(end_time - start_time);
        anses.push_back(ans);
    }

    for (int i = 0; i < dataset.size(); ++i) {
        if (!NaiveNT::check_isqrt(anses[i], dataset[i]))
            cerr << anses[i] << ' ' << dataset[i] << endl;
        GTEST_ASSERT_TRUE(NaiveNT::check_isqrt(anses[i], dataset[i]));
    }

    const ld_nano length = sample_time(times);
    cerr << "Integer square root for " << bits <<
         " bits (overall " << dataset.size() << " tries)" <<
         " on " << dataset_name << " took " << length <<
         " (expected " << et << ')' << endl;
    GTEST_ASSERT_LE(length / 1.ms, et / 1.ms);
}

void checkApproxIsqrt(const int bits) {
    const ul L = ul(1) << (bits - 1),
             R = (L << 1) - 1;
    GTEST_ASSERT_EQ(NaiveNT::isqrt(0), NT::isqrt_approx(0));
    const auto t = 200ns;
    auto expected_time = [&](const ul R) {
        return t;
    };
    const ld_nano et = expected_time(R);
    const ul T = .1s / et;
    vu rd = generate_random_isqrt_dataset(L, R, T),
       sd = generate_square_isqrt_dataset(L, R, T),
       md = generate_square_minus_one_isqrt_dataset(L, R, T),
       nd = generate_near_square_isqrt_dataset(L, R, T);
    check_isqrt_approx_results(rd, "random dataset", et, bits);
    check_isqrt_approx_results(sd, "square dataset", et, bits);
    check_isqrt_approx_results(md, "square minus one dataset", et, bits);
    check_isqrt_approx_results(nd, "near square dataset", et, bits);
}

#define CheckApproxIsqrt(x)                \
TEST(NumberTheory, CheckApproxIsqrt##x) {  \
    checkApproxIsqrt(x);                   \
}

CheckApproxIsqrt(1)
CheckApproxIsqrt(2)
CheckApproxIsqrt(3)
CheckApproxIsqrt(4)
CheckApproxIsqrt(5)
CheckApproxIsqrt(6)
CheckApproxIsqrt(7)
CheckApproxIsqrt(8)
CheckApproxIsqrt(9)
CheckApproxIsqrt(10)
CheckApproxIsqrt(11)
CheckApproxIsqrt(12)
CheckApproxIsqrt(13)
CheckApproxIsqrt(14)
CheckApproxIsqrt(15)
CheckApproxIsqrt(16)
CheckApproxIsqrt(17)
CheckApproxIsqrt(18)
CheckApproxIsqrt(19)
CheckApproxIsqrt(20)
CheckApproxIsqrt(21)
CheckApproxIsqrt(22)
CheckApproxIsqrt(23)
CheckApproxIsqrt(24)
CheckApproxIsqrt(25)
CheckApproxIsqrt(26)
CheckApproxIsqrt(27)
CheckApproxIsqrt(28)
CheckApproxIsqrt(29)
CheckApproxIsqrt(30)
CheckApproxIsqrt(31)
CheckApproxIsqrt(32)
CheckApproxIsqrt(33)
CheckApproxIsqrt(34)
CheckApproxIsqrt(35)
CheckApproxIsqrt(36)
CheckApproxIsqrt(37)
CheckApproxIsqrt(38)
CheckApproxIsqrt(39)
CheckApproxIsqrt(40)
CheckApproxIsqrt(41)
CheckApproxIsqrt(42)
CheckApproxIsqrt(43)
CheckApproxIsqrt(44)
CheckApproxIsqrt(45)
CheckApproxIsqrt(46)
CheckApproxIsqrt(47)
CheckApproxIsqrt(48)
CheckApproxIsqrt(49)
CheckApproxIsqrt(50)
CheckApproxIsqrt(51)
CheckApproxIsqrt(52)
// CheckApproxIsqrt(53)

void check_isqrt_results(const vu& dataset,
                                const string& dataset_name,
                                const ld_nano et,
                                const int bits) {
    vector<ld_nano> times;
    vector<ui> anses;
    for (ul n : dataset) {
        auto start_time = chrono::high_resolution_clock::now();
        ui ans = NT::isqrt(n);
        auto end_time = chrono::high_resolution_clock::now();
        times.push_back(end_time - start_time);
        anses.push_back(ans);
    }

    for (int i = 0; i < dataset.size(); ++i)
        GTEST_ASSERT_TRUE(NaiveNT::check_isqrt(anses[i], dataset[i]));

    const ld_nano length = sample_time(times);
    cerr << "Integer square root for " << bits <<
         " bits (overall " << dataset.size() << " tries)" <<
         " on " << dataset_name << " took " << length <<
         " (expected " << et << ')' << endl;
    GTEST_ASSERT_LE(length / 1.ms, et / 1.ms);
}

void checkIsqrt(const int bits) {
    const ul L = ul(1) << (bits - 1),
             R = (L << 1) - 1;
    GTEST_ASSERT_EQ(NaiveNT::isqrt(0), NT::isqrt(0));
    const auto t = 200ns;
    auto expected_time = [&](const ul R) {
        return t;
    };
    const ld_nano et = expected_time(R);
    const ul T = .1s / et;
    vu rd = generate_random_isqrt_dataset(L, R, T),
       sd = generate_square_isqrt_dataset(L, R, T),
       md = generate_square_minus_one_isqrt_dataset(L, R, T),
       nd = generate_near_square_isqrt_dataset(L, R, T);
    check_isqrt_results(rd, "random dataset", et, bits);
    check_isqrt_results(sd, "square dataset", et, bits);
    check_isqrt_results(md, "square minus one dataset", et, bits);
    check_isqrt_results(nd, "near square dataset", et, bits);
}

#define CheckIsqrt(x)               \
TEST(NumberTheory, CheckIsqrt##x) { \
    checkIsqrt(x);                  \
}

CheckIsqrt(1)
CheckIsqrt(2)
CheckIsqrt(3)
CheckIsqrt(4)
CheckIsqrt(5)
CheckIsqrt(6)
CheckIsqrt(7)
CheckIsqrt(8)
CheckIsqrt(9)
CheckIsqrt(10)
CheckIsqrt(11)
CheckIsqrt(12)
CheckIsqrt(13)
CheckIsqrt(14)
CheckIsqrt(15)
CheckIsqrt(16)
CheckIsqrt(17)
CheckIsqrt(18)
CheckIsqrt(19)
CheckIsqrt(20)
CheckIsqrt(21)
CheckIsqrt(22)
CheckIsqrt(23)
CheckIsqrt(24)
CheckIsqrt(25)
CheckIsqrt(26)
CheckIsqrt(27)
CheckIsqrt(28)
CheckIsqrt(29)
CheckIsqrt(30)
CheckIsqrt(31)
CheckIsqrt(32)
CheckIsqrt(33)
CheckIsqrt(34)
CheckIsqrt(35)
CheckIsqrt(36)
CheckIsqrt(37)
CheckIsqrt(38)
CheckIsqrt(39)
CheckIsqrt(40)
CheckIsqrt(41)
CheckIsqrt(42)
CheckIsqrt(43)
CheckIsqrt(44)
CheckIsqrt(45)
CheckIsqrt(46)
CheckIsqrt(47)
CheckIsqrt(48)
CheckIsqrt(49)
CheckIsqrt(50)
CheckIsqrt(51)
CheckIsqrt(52)
CheckIsqrt(53)
CheckIsqrt(54)
CheckIsqrt(55)
CheckIsqrt(56)
CheckIsqrt(57)
CheckIsqrt(58)
CheckIsqrt(59)
CheckIsqrt(60)
CheckIsqrt(61)
CheckIsqrt(62)
CheckIsqrt(63)
CheckIsqrt(64)

vu generate_manual_isqrt_dataset() {
    return { 18296940487774088227 };
}

TEST(NumberTheory, CheckIsqrtManual) {
    const int bits = 64;
    const ul L = ul(1) << (bits - 1),
             R = (L << 1) - 1;
    const auto t = 2000ns;
    auto expected_time = [&](const ul R) {
        return t;
    };
    const ld_nano et = expected_time(R);
    vu dataset = generate_manual_isqrt_dataset();
    check_isqrt_results(dataset, "manual dataset", et, 64);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
