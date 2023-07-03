#pragma once

namespace FFT {
    class NaiveNTT {
    public:
        static const int MOD = 998244353;
        static int multiply_nth(const vi& A, const vi& B, const int n) {
            int ans = 0;
            for (int i = max<int>(0, n - B.size() + 1); i < min<int>(n + 1, A.size()); ++i)
                ans = NT::sum(ans, NT::prod(A[i], B[n - i], MOD), MOD);
            return ans;
        }
        static vi multiply(const vi &A, const vi &B) {
            vi ans(max<int>(0, A.size() + B.size() - 1));
            for (int i = 0; i < ans.size(); ++i)
                ans[i] = multiply_nth(A, B, i);
            return ans;
        }
    };
}
