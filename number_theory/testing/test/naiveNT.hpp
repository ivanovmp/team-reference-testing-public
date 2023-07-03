//
// Created by Михаил Иванов on 07.02.2023.
//

#ifndef TESTTEAMREFERENCE_NAIVENT_HPP
#define TESTTEAMREFERENCE_NAIVENT_HPP

namespace NaiveNT {
	vpui factor(ul n) {
		vpui ans;
		for (unsigned i = 2; ul(i) * i <= n; i = 1 | 1 + i)
			if (n % i == 0) {
				ans.emplace_back(i, 0);
				do {
					n /= i; ++ans.back().y;
				} while (n % i == 0);
			}
		if (n > 1) ans.emplace_back(n, 1);
		return ans;
	}
	bool is_prime(ul n) {
		for (unsigned i = 2; ul(i) * i <= n; i = 1 | 1 + i)
			if (n % i == 0)
				return false;
		return n > 1;
	}
    bool check_le_isqrt(const ui n, const ul s) {
        return (ul)n * n <= s;
    }
    bool check_ge_isqrt(const ui n, const ul s) {
        if (n == ui(-1)) return true;
        return !check_le_isqrt(n + 1, s);
    }
    bool check_isqrt(const ui n, const ul s) {
        return check_le_isqrt(n, s) && check_ge_isqrt(n, s);
    }
    ui isqrt(const ul s) {
        if (s <= 1) return s;
        ui L = 0, R = ui(-1);
        ui n = R - L >> 1;
        while (n) {
            n += L;
            if (check_ge_isqrt(n, s))
                R = n;
            else
                L = n;
            n = R - L >> 1;
        }
        return n + R;
    }
    ui usqrt(const ul s) {
        if (s <= 2) return s;
        return 1 + isqrt(s - 1);
    }
};

#endif //TESTTEAMREFERENCE_NAIVENT_HPP
