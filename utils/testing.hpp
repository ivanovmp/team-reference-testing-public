#pragma once
#include <ratio>
#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <random>
#include "typedefs.hpp"

typedef chrono::duration<ld, nano> ld_nano;

template<typename Rep, typename Period>
ostream& operator<<(ostream& out, const chrono::duration<Rep, Period>& interval) {
    if (interval >= 1.s)
        return out << interval / 1.s << " s";
    else if (interval >= 1.ms)
        return out << interval / 1.ms << " ms";
    else if (interval >= 1.us)
        return out << interval / 1.us << " us";
    else
        return out << interval / 1.ns << " ns";
}

ld_nano sample_time(vector<ld_nano> v) {
    if (v.empty()) return ld_nano(-numeric_limits<ld>::infinity());
    const int index = (v.size() - 1) * 3 / 4;
    nth_element(v.begin(), v.begin() + index, v.end());
    return v[index];
}

template<typename Long>
vector<Long> choose_n_numbers(const Long mn, const Long mx, const Long n) {
    if (n <= 0) return {};
    static mt19937 rng;
    unordered_set<Long> numbers;
    uniform_int_distribution<Long> seg(mn, mx);
    while (numbers.size() < n)
        numbers.insert(seg(rng));
    vector<Long> numbers_vector(numbers.begin(), numbers.end());
    sort(numbers_vector.begin(), numbers_vector.end());
    return numbers_vector;
}

#define mp make_pair
