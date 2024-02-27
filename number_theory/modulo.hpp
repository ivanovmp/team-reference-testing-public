#pragma once
namespace NT {
    int prod(int a, int b, int m) {
        return (ul)a * b % m;
    }
    int inv(int a, int m) {
        if (a == 1) return 1;
        return (1 - ll(inv(m % a, a)) * m) / a + m;
    }
    int sum(int a, int b, int m) {
        a += b; if (a >= m || a < b) a -= m;
        return a;
    }
    int dif(int a, int b, int m) {
        b = a - b; if (b < 0 || b > a) b += m;
        return b;
    }
    ul sumul(ul a, ul b, ul m) {
        a += b; if (a >= m || a < b) a -= m;
        return a;
    }
    ul diful(ul a, ul b, ul m) {
        if ((b = a - b) > a) b += m;
        return b;
    }
#ifdef __GNUC__
    ul produl(ul a, ul b, ul m) {
        return (__uint128_t)a * b % m;
    }
#endif
    int powmod(int a, ul b, int m) {
        int ans = 1;
        while (b) {
            if (b & 1) ans = prod(ans, a, m);
            if (b >>= 1) a = prod(a, a, m);
        }
        return ans;
    }
#ifdef __GNUC__
    ul powmodul(ul a, ul b, ul m) {
        ul ans = 1;
        while (b) {
            if (b & 1) ans = produl(ans, a, m);
            if (b >>= 1) a = produl(a, a, m);
        }
        return ans;
    }
#endif
}
