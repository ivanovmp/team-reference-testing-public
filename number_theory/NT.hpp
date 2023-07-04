#pragma once
namespace NT {
    struct Barrett {
        ul b, m;
        Barrett() : b(), m() {}
        Barrett(ul _b) : b(_b), m(ul(-1) / _b) {}
        ui reduce(ul x) const {
            ul q = (ul)((__uint128_t(m) * x) >> 64), r = x - q * b;
            return r - b * (r >= b);
        }
    };
    ui isqrt_approx(ul n) {
        return sqrt(n);
    }
    ui isqrt(ul n) {
        ul x = isqrt_approx(n);
        if (n < ul(1) << 52) return x;
        while (true) {
            ul y = n / x;
            if (x == y) return x;
            if (x == y + 1) return y * y > n ? y - 1 : y;
            x = y;
        }
    }
	template<class Long>
	Long powint(Long a, ul b) {
		Long ans = 1;
		while (b) {
			if (b & 1) ans *= a;
			a *= a; b >>= 1;
		}
		return ans;
	}
	int prod(int a, int b, int m) {
		return (ul)a * b % m;
	}
    int inv(int a, int m) {
        if (a == 1) return 1;
        return (1 - ll(inv(m % a, a)) * m) / a + m;
    }
    ul sum(ul a, ul b, ul m) {
        a += b; if (a >= m || a < b) a -= m;
        return a;
    }
    ul dif(ul a, ul b, ul m) {
        if ((b = a - b) > a) b += m;
        return b;
    }
	ul prod(ul a, ul b, ul m) {
		return (__uint128_t)a * b % m;
	}
    ul gcd(ul a, ul b) {
        while (b) { a %= b; swap(a, b); }
        return a;
    }
	template<class Long>
	Long binpow(Long a, ul b, Long M) {
		Long ans = 1;
		while (b) {
			if (b & 1) ans = prod(ans, a, M);
			a = prod(a, a, M); b >>= 1;
		}
		return ans;
	}
	struct Eratosthenes {
		vi m, pr;
		Eratosthenes(const int n) : m(n, 1) {
			for (int i = 2; i < n; ++i) {
				if (m[i] == 1) pr.pb(i), m[i] = i;
				for (int j : pr) {
					if (j > m[i]) break;
					if (i * j < n) m[i * j] = j;
					else break;
				}
			}
		}
		int N() const { return m.size(); }
		template<class Long>
		vpui factor(Long n) {
			vpui ans; int i = 0;
			while (n >= N()) {
				if (i >= pr.size()) {
					ans.emplace_back(n, 1); return ans;
				}
				if (n % pr[i] == 0) {
					ans.emplace_back(pr[i], 0);
					do {
						n /= pr[i]; ++ans.back().y;
					} while (n % pr[i] == 0);
				}
				++i;
			}
			while (n > 1) {
				if (ans.empty() || ans.back().x != m[n])
					ans.emplace_back(m[n], 0);
				++ans.back().y;
				n /= m[n];
			}
			return ans;
		}
	};
	int sigma0(const vpui& v) {
		int ans = 1;
		for (const pui& u : v) ans *= u.y + 1;
		return ans;
	}
	ul sigma1(const vpui& v) {
		ul ans = 1;
		for (const pui& u : v) {
			ul p = powint(u.x, u.y);
			ans *= p + (p - 1) / (u.x - 1);
		}
		return ans;
	}
	ul phi(const vpui& v) {
		ul ans = 1;
		for (const pui& u : v)
			ans *= powint(u.x, u.y - 1) * (u.x - 1);
		return ans;
	}
	vu divisors(const vpui& v) {
		vu ans(1, 1);
		for (const pui& u : v) {
			int n = ans.size();
			for (int i = 0; i < n * u.y; ++i)
				ans.push_back(ans[i] * u.x);
		}
		return ans;
	}
	template<typename Long>
	bool miller_rabin_iteration(const Long& n, const Long& x, const Long& odd_part, const int twos) {
		if (!x) return true;
		Long xodd = binpow(x, odd_part, n);
		if (xodd == 1) return true;
		for (int i = 0; i < twos; ++i) {
			if (xodd == n - 1) return true;
			if (i == twos - 1) return false;
			xodd = prod(xodd, xodd, n);
		}
		return false;
	}
    template<class Long>
    int ctz(const Long& n) {
        if constexpr (sizeof(n) == 8) return __builtin_ctzll(n);
        else return __builtin_ctz(n);
    }
	template<class Long>
	bool miller_rabin(const Long& n, const int iters = 25) {
		if (n <= 2 || !(n & 1)) return n == 2;
		static mt19937 rng; uniform_int_distribution<Long> seg(2, n - 1);
		int twos = ctz(n - 1); Long odd_part = n - 1 >> twos;
		for (int iter = 0; iter < 25; ++iter)
			if (!miller_rabin_iteration(n, seg(rng), odd_part, twos))
				return false;
		return true;
	}
	// https://miller-rabin.appspot.com
	template<typename Long>
	bool deterministic_miller_rabin(const Long& n) {
		if (n <= 2 || !(n & 1)) return n == 2;
		int twos = ctz(n - 1); Long odd_part = n >> twos;
		vu bounds = { 341530, 1050535500, 350269456336, 55245642489450,
					  7999252175582850, 585226005592931976, ul(-1) };
		vvu bases = { { 9345883071009581737 },
					  { 336781006125, 9639812373923155 },
					  { 4230279247111683200, 14694767155120705706, 16641139526367750375 },
					  { 2, 141889084524735, 1199124725622454117, 11096072698276303650 },
					  { 2, 4130806001517, 149795463772692060, 186635894390467037, 3967304179347715805 },
					  { 2, 123635709730000, 9233062284813009, 43835965440333360, 761179012939631437, 1263739024124850375 },
					  { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 } };
		for (int i = 0; i < bounds.size(); ++i)
			if (n <= bounds[i]) {
				for (ul u : bases[i])
					if (!miller_rabin_iteration(n, Long(u % n), odd_part, twos))
						return false;
				return true;
			}
	}
	template<typename Long>
	bool is_prime(const Long& n) {
		if (n <= numeric_limits<int>::max())
			return deterministic_miller_rabin<int>(n);
		else
			return deterministic_miller_rabin<ul>(n);
	}
    template<typename Long>
    Long find_small_divisor(const Long n) {
        const int M = 1000; static Eratosthenes m(M);
        if (n < M) return m.m[n];
        for (int i : m.pr) if (n % i == 0) return i;
        return 1;
    }
    template<typename Long>
    Long find_divisor(const Long n) {
        Long d = find_small_divisor(n); if (d != 1) return d;
        static mt19937 rng; uniform_int_distribution<Long> seg(2, n - 1);
        const int B = pow(n, .25);
        auto f = [&](Long x) { return sum(prod(x, x, n), 1, n); };
        while (true) {
            Long x = seg(rng), y = x;
            for (int i = 0; i < B; ++i) {
                x = f(x); y = f(f(y)); Long g = gcd(x, y);
                if (g == n) continue;
                if (g > 1) return g;
            }
        }
    }
    vpui fact_merge(const vpui& a, const vpui& b) {
        vpui ans;
        int i = 0, j = 0;
        while (i < a.size() || j < b.size()) {
            if (j == b.size()) ans.push_back(a[i++]);
            else if (i == a.size()) ans.push_back(b[j++]);
            else if (a[i].first < b[j].first) ans.push_back(a[i++]);
            else if (a[i].first > b[j].first) ans.push_back(b[j++]);
            else { ans.emplace_back(a[i].first, a[i].second + b[j].second);
                   ++i; ++j; } }
        return ans;
    }
	template<typename Long>
	vpui factor(const Long n) {
		if (n == 1)
			return {};
		if (deterministic_miller_rabin(n))
			return { { n, 1 } };
        Long divisor = find_divisor(n);
        return fact_merge(factor(divisor), factor(n / divisor));
	}
}
