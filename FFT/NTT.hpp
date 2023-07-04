#pragma once

namespace FFT {
    class NTT {
    public:
        const int n = 20;
        const int size = (1 << n);
        const int MOD = 998244353, g = 3;
        const int ROOT = 565042129;

        NTT() : revers(size), root(size),
                fftA(size), fftB(size) {
            root[0] = 1;
            for (int i = 1; i < size; i++)
                root[i] = (ll) root[i - 1] * ROOT % MOD;
        }

        void fft(vector<int> &poly, int newN) {
            revers[0] = 0;
            for (int i = 1; i < (1 << newN); i++) {
                if (i % 2 == 0) revers[i] = revers[i / 2] / 2;
                else revers[i] = revers[i / 2] / 2 + (1 << (newN - 1));
                if (revers[i] < i) swap(poly[revers[i]], poly[i]);
            }

            for (int level = 1; level <= newN; level++)
                for (int block = 0; block < (1 << (newN - level)); block++)
                    for (int step = 0; step < (1 << (level - 1)); step++) {
                        int num1 = (block << level) + step;
                        int num2 = num1 + (1 << (level - 1));
                        int valA = poly[num1];
                        int valB = (ll) root[step << (n - level)] * poly[num2] % MOD;
                        poly[num1] = (valA + valB) % MOD;
                        poly[num2] = (valA - valB + MOD) % MOD;
                    }
        }

        void rev_fft(vector<int> &poly, int step) {
            fft(poly, step);
            reverse(poly.begin() + 1, poly.begin() + (1 << step));
            int inv_size = NT::binpow((1 << step), MOD - 2, MOD);
            for (int i = 0; i < (1 << step); i++) poly[i] = (ll) poly[i] * inv_size % MOD;
        }

        vector<int> prod(const vector<int> &A, const vector<int> &B, int step) {
            fill(fftA.begin(), fftA.begin() + (1 << step), 0);
            fill(fftB.begin(), fftB.begin() + (1 << step), 0);
            for (int i = 0; i < (int) A.size(); i++) fftA[i] = A[i] % MOD;
            for (int i = 0; i < (int) B.size(); i++) fftB[i] = B[i] % MOD;

            fft(fftA, step);
            fft(fftB, step);
            for (int i = 0; i < (1 << step); i++) fftA[i] = (ll) fftA[i] * fftB[i] % MOD;
            rev_fft(fftA, step);

            vector<int> result(1 << step);
            for (int i = 0; i < (1 << step); i++) result[i] = fftA[i];
            return result;
        }

        vi add(const vi& A, const vi& B) {
            vi ans(max(A.size(), B.size()));
            for (int i = 0; i < ans.size(); ++i)
                if (i >= A.size()) ans[i] = B[i];
                else if (i >= B.size()) ans[i] = A[i];
                else ans[i] = NT::sum(A[i], B[i], MOD);
        }

        vi dif(const vi& A, const vi& B) {
            vi ans(max(A.size(), B.size()));
            for (int i = 0; i < ans.size(); ++i)
                if (i >= A.size()) ans[i] = NT::dif(0, B[i], MOD);
                else if (i >= B.size()) ans[i] = A[i];
                else ans[i] = NT::dif(A[i], B[i], MOD);
        }

        vi inverse(const vi &A, const int n) {
            vi a(1, A[0]);
            vi b(1, NT::inv(A[0], MOD));
            for (int bits = 1; b.size() < n; ++bits) {
                b.resize(1 << (bits - 1));
                for (int i = b.size(); i < min<int>(1 << bits, A.size()); ++i)
                    a.push_back(A[i]);
                vi ab = prod(a, b, bits + 1); ab.resize(1 << bits);
                b = prod(b, dif(vi(1, 2), ab), bits + 1);
            }
            b.resize(n);
            return b;
        }

        vi derivative(const vi &A) {
            if (A.size() <= 1) return {};
            vi B(A.size() - 1);
            for (int i = 1; i < A.size(); ++i)
                B[i - 1] = NT::prod(A[i], i, MOD);
            return B;
        }

        vi factorials(const int n) {
            vi fact(n, 1);
            for (int i = 2; i < n; ++i)
                fact[i] = NT::prod(i, fact[i - 1], MOD);
            return fact;
        }

        vi inverses(const int n) {
            vi fact = factorials(n);
            int ifact = NT::inv(fact.back(), MOD);
            for (int i = n - 1; i >= 2; --i) {
                fact[i] = NT::prod(fact[i - 1], ifact, MOD);
                ifact = NT::prod(ifact, i, MOD);
            }
            return fact;
        }

        vi integral(const vi &A) {
            vi B(A.size() + 1);
            vi invs = inverses(B.size());
            for (int i = 1; i <= A.size(); ++i)
                B[i] = NT::prod(A[i - 1], invs[i], MOD);
            return B;
        }

    private:
        vector<int> root, revers, fftA, fftB;
    };
}
