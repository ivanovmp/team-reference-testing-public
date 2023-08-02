#pragma once

namespace FFT {
    class NaiveNTT {
    public:
        static const int MOD = 998244353;
        static int prod_nth(const vi& A, const vi& B, const int n) {
            int ans = 0;
            for (int i = max<int>(0, n - B.size() + 1); i < min<int>(n + 1, A.size()); ++i)
                ans = NT::sum(ans, NT::prod(A[i], B[n - i], MOD), MOD);
            return ans;
        }
        static vi prod(const vi &A, const vi &B) {
            vi ans(max<int>(0, A.size() + B.size() - 1));
            for (int i = 0; i < ans.size(); ++i)
                ans[i] = prod_nth(A, B, i);
            return ans;
        }
        static vi logarithm(vi A, const int n) {
            A[0] = 0;
            A.resize(n);
            vi ans = A;
            vi pw = A;
            for (int i = 2; i < n; ++i) {
                pw = prod(pw, A);
                pw.resize(n);
                int coef = NT::inv(i, MOD);
                if (i % 2 == 0)
                    coef = NT::dif(0, coef, MOD);
                for (int j = 0; j < n; ++j)
                    ans[j] = NT::sum(ans[j], NT::prod(pw[j], coef, MOD), MOD);
            }
            return ans;
        }
        static vi exponent(const vi& A, const int n) {
            vi pw = A; pw.resize(n);
            vi ans = pw; ans.resize(n);
            ans[0] = NT::sum(ans[0], 1, MOD);
            int fact = 1;
            for (int i = 2; i < n; ++i) {
                pw = prod(pw, A);
                pw.resize(n);
                fact = NT::prod(fact, i, MOD);
                int ifact = NT::inv(fact, MOD);
                for (int j = 0; j < n; ++j)
                    ans[j] = NT::sum(ans[j], NT::prod(pw[j], ifact, MOD), MOD);
            }
            return ans;
        }
        static vi sin(const vi& A, const int n) {
            vi pw = A; pw.resize(n);
            vi ans = pw; ans.resize(n);
            int fact = 1;
            for (int i = 2; i < n; ++i) {
                pw = prod(pw, A);
                pw.resize(n);
                fact = NT::prod(fact, i, MOD);
                int ifact = NT::inv(fact, MOD);
                if (i % 2 == 0) continue;
                int coef = ifact;
                if (i % 4 == 3) coef = NT::dif(0, coef, MOD);
                for (int j = 0; j < n; ++j)
                    ans[j] = NT::sum(ans[j], NT::prod(pw[j], coef, MOD), MOD);
            }
            return ans;
        }
        static vi cos(const vi& A, const int n) {
            vi pw = A; pw.resize(n);
            vi ans(n); ans[0] = 1;
            int fact = 1;
            for (int i = 2; i < n; ++i) {
                pw = prod(pw, A);
                pw.resize(n);
                fact = NT::prod(fact, i, MOD);
                int ifact = NT::inv(fact, MOD);
                if (i % 2 == 1) continue;
                int coef = ifact;
                if (i % 4 == 2) coef = NT::dif(0, coef, MOD);
                for (int j = 0; j < n; ++j)
                    ans[j] = NT::sum(ans[j], NT::prod(pw[j], coef, MOD), MOD);
            }
            return ans;
        }
        static vi sh(const vi& A, const int n) {
            vi expA = exponent(A, n);
            vi B = A;
            for (int &i : B) i = NT::dif(0, i, MOD);
            vi expB = exponent(B, n);
            int HALF = NT::inv(2, MOD);
            for (int i = 0; i < n; ++i)
                expA[i] = NT::prod((int)NT::dif(expA[i], expB[i], MOD), HALF, MOD);
            return expA;
        }
        static vi ch(const vi& A, const int n) {
            vi expA = exponent(A, n);
            vi B = A;
            for (int &i : B) i = NT::dif(0, i, MOD);
            vi expB = exponent(B, n);
            int HALF = NT::inv(2, MOD);
            for (int i = 0; i < n; ++i)
                expA[i] = NT::prod((int)NT::sum(expA[i], expB[i], MOD), HALF, MOD);
            return expA;
        }
    };
}
