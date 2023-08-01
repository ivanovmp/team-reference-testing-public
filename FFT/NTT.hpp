#pragma once

namespace FFT {
    class NTT {
    public:
        const int n; // <= 23
        const int size; // == 1 << n
        const int MOD = 998244353, G = 3;
        const int ROOT; // = 565042129 for n == 20;

        NTT(int n = 20) : n(n), size(1 << n), ROOT(NT::binpow(G, (MOD - 1) >> n, MOD)), revers(size), root(size, 1),
                fftA(size), fftB(size) {
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

        template<class Iter>
        vector<int> multiply(Iter Ab, Iter Ae, Iter Bb, Iter Be, int step) {
            if (Ab == Ae || Bb == Be) return {};
            fill(fftA.begin(), fftA.begin() + (1 << step), 0);
            fill(fftB.begin(), fftB.begin() + (1 << step), 0);
            for (auto it = Ab; it != Ae; ++it) fftA[it - Ab] = *it % MOD;
            for (auto it = Bb; it != Be; ++it) fftB[it - Bb] = *it % MOD;

            fft(fftA, step);
            fft(fftB, step);
            for (int i = 0; i < (1 << step); i++) fftA[i] = (ll) fftA[i] * fftB[i] % MOD;
            rev_fft(fftA, step);

            vector<int> result(1 << step);
            for (int i = 0; i < (1 << step); i++) result[i] = fftA[i];
            return result;
        }

        vector<int> multiply(const vector<int> &A, const vector<int> &B, int step) {
            return multiply(A.begin(), A.end(), B.begin(), B.end(), step);
        }

        // returns a vector of size exactly n
        vi prod(const vi &A, const vi &B, const int n) {
            int as = min<int>(A.size(), n), bs = min<int>(B.size(), n);
            vi ans = multiply(A.begin(), A.begin() + as, B.begin(), B.begin() + bs, 32 - __builtin_clz(as + bs - 2));
            ans.resize(n);
            return ans;
        }

        vi prod(const vi &A, const vi &B) {
            if (A.empty() || B.empty()) return {};
            return prod(A, B, A.size() + B.size() - 1);
        }

        vi sum(const vi &A, const vi &B) const {
            vi ans(max(A.size(), B.size()));
            for (int i = 0; i < ans.size(); ++i)
                if (i >= A.size()) ans[i] = B[i];
                else if (i >= B.size()) ans[i] = A[i];
                else ans[i] = NT::sum(A[i], B[i], MOD);
            return ans;
        }

        vi dif(const vi &A, const vi &B) const {
            vi ans(max(A.size(), B.size()));
            for (int i = 0; i < ans.size(); ++i)
                if (i >= A.size()) ans[i] = NT::dif(0, B[i], MOD);
                else if (i >= B.size()) ans[i] = A[i];
                else ans[i] = NT::dif(A[i], B[i], MOD);
            return ans;
        }

        vi inverse(const vi &A, const int n) {
            vi a(1, A[0]);
            vi b(1, NT::inv(A[0], MOD));
            for (int bits = 1; b.size() < n; ++bits) {
                copy(A.begin() + min<int>(A.size(), b.size()), A.begin() + min<int>(1 << bits, A.size()), back_inserter(a));
                b = prod(b, dif(vi(1, 2), prod(a, b, 1 << bits)), min(n, 1 << bits));
            }
            return b;
        }

        vi derivative(const vi &A) const {
            if (A.size() <= 1) return {};
            vi B(A.size() - 1);
            for (int i = 1; i < A.size(); ++i)
                B[i - 1] = NT::prod(A[i], i, MOD);
            return B;
        }

        vi factorials(const int n) const {
            vi fact(n, 1);
            for (int i = 2; i < n; ++i)
                fact[i] = NT::prod(i, fact[i - 1], MOD);
            return fact;
        }

        vi inverses(const int n) const {
            vi fact = factorials(n);
            int ifact = NT::inv(fact.back(), MOD);
            for (int i = n - 1; i >= 2; --i) {
                fact[i] = NT::prod(fact[i - 1], ifact, MOD);
                ifact = NT::prod(ifact, i, MOD);
            }
            return fact;
        }

        // ans[0] == 0
        vi integral(const vi &A) const {
            vi B(A.size() + 1);
            vi invs = inverses(B.size());
            for (int i = 1; i <= A.size(); ++i)
                B[i] = NT::prod(A[i - 1], invs[i], MOD);
            return B;
        }

        // A[0] == 1, ans[0] = 0
        vi logarithm(vi A, const int n) {
            A.resize(min<int>(A.size(), n)); if (n == 0) return A;
            A = prod(derivative(A), inverse(A, n - 1), n - 1);
            return integral(A);
        }

        // A[0] == 0, ans[0] == 1
        vi exponent(const vi &A, const int n) {
            vi a(1);
            vi b(1, 1);
            for (int bits = 1; b.size() < n; ++bits) {
                vi ln_b = logarithm(b, min(1 << bits, n));
                copy(A.begin() + b.size(), A.begin() + min<int>(1 << bits, A.size()), back_inserter(a));
                ln_b = dif(a, ln_b);
                ln_b[0] = NT::sum(ln_b[0], 1, MOD);
                b = prod(b, ln_b, min(1 << bits, n));
            }
            return b;
        }

        // A[0] == 1, ans[0] == 1, alpha is rational
        vi power(vi A, const int alpha, const int n) {
            A = logarithm(A, n);
            for (int &a: A)
                a = NT::prod(a, alpha, MOD);
            A = exponent(A, n);
            return A;
        }

        const int imaginary_unit = 911660635;
        // A[0] == 0, ans[0] == 0
        vi sin(const vi &A, const int n) {
            static int MINUS_I_HALF = NT::prod(imaginary_unit, (MOD - 1) / 2, MOD);
            vi a = A; a.resize(min<int>(n, A.size()));
            for (int &i: a) i = NT::prod(i, imaginary_unit, MOD);
            vi exp_1 = exponent(a, n);
            for (int &i : a) i = NT::dif(0, i, MOD);
            vi exp_2 = exponent(a, n);
            exp_1 = dif(exp_1, exp_2);
            for (int &i : exp_1) i = NT::prod(i, MINUS_I_HALF, MOD);
            return exp_1;
        }

        // A[0] == 0, ans[0] == 1
        vi cos(const vi &A, const int n) {
            static int HALF = (MOD + 1) / 2;
            vi a = A; a.resize(min<int>(n, A.size()));
            for (int &i: a) i = NT::prod(i, imaginary_unit, MOD);
            vi exp_1 = exponent(a, n);
            for (int &i : a) i = NT::dif(0, i, MOD);
            vi exp_2 = exponent(a, n);
            exp_1 = sum(exp_1, exp_2);
            for (int &i : exp_1) i = NT::prod(i, HALF, MOD);
            return exp_1;
        }

        // A[0] == 0, ans[0] == 0
        vi arcsin(const vi &A, const int n) {
            static int MINUS_HALF = (MOD - 1) / 2;
            return integral(prod(derivative(A), power(dif(vi(1, 1), prod(A, A, n - 1)), MINUS_HALF, n - 1), n - 1));
        }

        vi EGF(vi ogf) {
            int ifact = 1, n = ogf.size();
            for (int i = 2; i < n; ++i) ifact = NT::prod(ifact, i, MOD);
            ifact = NT::inv(ifact, MOD);
            for (int i = n - 1; i >= 2; --i) {
                ogf[i] = NT::prod(ogf[i], ifact, MOD);
                ifact = NT::prod(ifact, i, MOD);
            }
            return ogf;
        }

        vi OGF(vi egf) {
            int fact = 1, n = egf.size();
            for (int i = 2; i < n; ++i) {
                fact = NT::prod(fact, i, MOD);
                egf[i] = NT::prod(egf[i], fact, MOD);
            }
            return egf;
        }
        // A[0] == 0, B[0] == 0
        vi euler_semitransform(const vi &A, const int n) {
            vi B(n);
            vi invs = inverses(n);
            for (int k = 1; k < n; ++k)
                for (int i = 1, j = k; j < n; ++i, j += k)
                    B[j] = NT::sum(B[j], NT::prod(A[i], invs[k], MOD), MOD);
            return B;
        }
        // A[0] == 0, B[0] == 1
        vi euler_transform(const vi &A, const int n) {
            return exponent(euler_semitransform(A, n), n);
        }
        // A[0] == 0, B[0] == 0
        vi inverse_euler_semitransform(vi B, const int n) {
            vi fact(n, 1), invs(n, 1);
            for (int i = 2; i < n; ++i) fact[i] = NT::prod(i, fact[i - 1], MOD);
            int ifact = NT::inv(fact[n - 1], MOD);
            for (int i = n - 1; i >= 2; --i) {
                invs[i] = NT::prod(ifact, fact[i - 1], MOD);
                ifact = NT::prod(ifact, i, MOD);
            }
            for (int i = 1; i < n; ++i)
                for (int k = 2, j = 2 * i; j < n; ++k, j += i)
                    B[j] = NT::dif(B[j], NT::prod(B[i], invs[k], MOD), MOD);
            return B;
        }
        // A[0] == 0, B[0] == 1
        vi inverse_euler_transform(const vi &B, const int n) {
            return inverse_euler_semitransform(logarithm(B, n), n);
        }

    private:
        vector<int> root, revers, fftA, fftB;
    };
}
