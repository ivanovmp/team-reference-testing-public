#pragma once

namespace FFT {
    class NaiveFFT {
    public:
        using db = double;
        static db multiply_nth(const vector<db>& A, const vector<db>& B, const int n) {
            db ans = 0;
            for (int i = max<int>(0, n - B.size() + 1); i < min<int>(n + 1, A.size()); ++i)
                ans += A[i] * B[n - i];
            return ans;
        }
        static vector<db> multiply(const vector<db>& A, const vector<db>& B) {
            vector<db> ans(max<int>(0, A.size() + B.size() - 1));
            for (int i = 0; i < ans.size(); ++i)
                ans[i] = multiply_nth(A, B, i);
            return ans;
        }
    };
}
