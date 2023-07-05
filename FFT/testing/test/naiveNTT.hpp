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
                for (int i = 0; i < n; ++i) {
                    ans[i] = NT::sum(ans[i], NT::prod(pw[i], coef, MOD), MOD);
                }
            }
            return ans;
        }
    };
}
