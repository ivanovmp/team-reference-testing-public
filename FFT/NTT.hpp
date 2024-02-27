#pragma once
namespace FFT {
    class NTT {
    public:
        inline static const int MOD = 998244353, G = 3;
        inline static const int imaginary_unit = 911660635; // i^2 = MOD - 1
        vi rt;
        int n() const {
            return rt.size() - 1;
        }
        NTT(const int n = 20) : rt(n + 1) {
            rt[n] = NT::powmod(G, MOD >> n, MOD);
            for (int i = n - 1; i >= 0; --i) rt[i] = NT::prod(rt[i + 1], rt[i + 1], MOD);
        }
        void fft(vector<int> &poly, int k) {
            poly.resize(1 << k);
            static vi r;
            r.resize(1 << k);
            for (int i = 1; i < 1 << k; ++i) {
                r[i] = (r[i >> 1] >> 1) + ((i & 1) << k - 1);
                if (i < r[i]) swap(poly[i], poly[r[i]]);
            }
            for (int l = 0; l < k; ++l) {
                int pr = 1;
                for (int i = 0; i < 1 << k; ++i) {
                    int j = i ^ 1 << l;
                    if (j <= i) {
                        pr = 1;
                        continue;
                    }
                    int x = poly[i], y = NT::prod(poly[j], pr, MOD);
                    poly[i] = NT::sum(x, y, MOD); poly[j] = NT::dif(x, y, MOD);
                    pr = NT::prod(pr, rt[l + 1], MOD);
                }
            }
        }
        void rev_fft(vector<int> &poly, int k) {
            fft(poly, k);
            reverse(poly.begin() + 1, poly.end());
            int t = NT::inv(1 << k, MOD);
            for (int& i : poly) i = NT::prod(i, t, MOD);
        }
        vi multiply(vi A, vi B, int k) {
            if (A.empty() || B.empty()) return vi(1 << k);
            fft(A, k);
            fft(B, k);
            for (int i = 0; i < 1 << k; ++i)
                A[i] = NT::prod(A[i], B[i], MOD);
            rev_fft(A, k);
            return A;
        }
        template<class Iter>
        vi multiply(Iter Ab, Iter Ae, Iter Bb, Iter Be, int k) {
            return multiply(vi(Ab, Ae), vi(Bb, Be), k);
        }

        // returns a vector of size exactly n
        vi prod(vi A, vi B, const int n) {
            if (A.empty() || B.empty()) return vi(n);
            const int as = min<int>(A.size(), n), bs = min<int>(B.size(), n);
            A.resize(as); B.resize(bs);
            vi ans = multiply(std::move(A), std::move(B), 32 - __builtin_clz(as + bs - 2));
            ans.resize(n);
            return ans;
        }
        // returns a vector of size A.size() + B.size() - 1 or 0
        vi prod(vi A, vi B) {
            if (A.empty() || B.empty()) return {};
            const int n = A.size() + B.size() - 1;
            return prod(std::move(A), std::move(B), n);
        }

        static vi sum(const vi &A, const vi &B) {
            vi ans(max(A.size(), B.size()));
            for (int i = 0; i < ans.size(); ++i)
                if (i >= A.size()) ans[i] = B[i];
                else if (i >= B.size()) ans[i] = A[i];
                else ans[i] = NT::sum(A[i], B[i], MOD);
            return ans;
        }

        static vi dif(const vi &A, const vi &B) {
            vi ans(max(A.size(), B.size()));
            for (int i = 0; i < ans.size(); ++i)
                if (i >= A.size()) ans[i] = NT::dif(0, B[i], MOD);
                else if (i >= B.size()) ans[i] = A[i];
                else ans[i] = NT::dif(A[i], B[i], MOD);
            return ans;
        }
        // A[0] != 0
        vi inverse(const vi &A, const int n) {
            vi a(1, A[0]);
            vi b(1, NT::inv(A[0], MOD));
            for (int bits = 1; b.size() < n; ++bits) {
                copy(A.begin() + min<int>(A.size(), b.size()), A.begin() + min<int>(1 << bits, A.size()), back_inserter(a));
                b = prod(b, dif(vi(1, 2), prod(a, b, 1 << bits)), min(n, 1 << bits));
            }
            return b;
        }

        static vi derivative(const vi &A) {
            if (A.size() <= 1) return {};
            vi B(A.size() - 1);
            for (int i = 1; i < A.size(); ++i)
                B[i - 1] = NT::prod(A[i], i, MOD);
            return B;
        }

        static vi factorials(const int n) {
            vi fact(n, 1);
            for (int i = 2; i < n; ++i)
                fact[i] = NT::prod(i, fact[i - 1], MOD);
            return fact;
        }

        static vi inverses(const int n) {
            vi fact = factorials(n);
            int ifact = NT::inv(fact.back(), MOD);
            for (int i = n - 1; i >= 2; --i) {
                fact[i] = NT::prod(fact[i - 1], ifact, MOD);
                ifact = NT::prod(ifact, i, MOD);
            }
            return fact;
        }
        // ans[0] == 0
        static vi integral(const vi &A) {
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
        // A[0] == 0, ans[0] == 0
        vi sin(const vi &A, const int n) {
            static int MINUS_I_HALF = NT::prod(imaginary_unit, (MOD - 1) / 2, MOD);
            vi a = A; a.resize(min<int>(n, A.size()));
            for (int &i: a) i = NT::prod(i, imaginary_unit, MOD);
            vi exp_1 = exponent(a, n);
            vi exp_2 = inverse(exp_1, n);
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
            vi exp_2 = inverse(exp_1, n);
            exp_1 = sum(exp_1, exp_2);
            for (int &i : exp_1) i = NT::prod(i, HALF, MOD);
            return exp_1;
        }
        // A[0] == 0, ans[0] == 0; tg(x) = 2i / (e^{2ix} + 1) - i
        vi tg(vi A, const int n) {
            const int TWO_I = NT::sum(imaginary_unit, imaginary_unit, MOD);
            for (int &i : A) i = NT::prod(i, TWO_I, MOD);
            A = exponent(A, n); A[0] = NT::sum(A[0], 1, MOD); A = inverse(A, n);
            for (int &i : A) i = NT::prod(i, TWO_I, MOD);
            A[0] = NT::dif(A[0], imaginary_unit, MOD); return A;
        }
        // A[0] == 0, ans[0] == 0
        vi arcsin(const vi &A, const int n) {
            static int MINUS_HALF = (MOD - 1) / 2;
            return integral(prod(derivative(A), power(dif(vi(1, 1), prod(A, A, n - 1)), MINUS_HALF, n - 1), n - 1));
        }
        // A[0] == 0, ans[0] == 0
        vi arctg(const vi &A, const int n) {
            if (n <= 1) return vi(n);
            vi A2P1 = prod(A, A, n - 1); A2P1[0] = NT::sum(A2P1[0], 1, MOD);
            return integral(prod(derivative(A), inverse(A2P1, n - 1),n - 1));
        }
        // A[0] == 0, ans[0] == 0
        vi sh(vi A, const int n) {
            static int HALF = (MOD + 1) / 2;
            A = exponent(A, n); A = dif(A, inverse(A, n));
            for (int &i: A) i = NT::prod(i, HALF, MOD);
            return A;
        }
        // A[0] == 0, ans[0] == 1
        vi ch(vi A, const int n) {
            static int HALF = (MOD + 1) / 2;
            A = exponent(A, n); A = sum(A, inverse(A, n));
            for (int &i: A) i = NT::prod(i, HALF, MOD);
            return A;
        }
        // A[0] == 0, ans[0] == 0; 1 - 2 / (e^{2x} + 1)
        vi th(vi A, const int n) {
            A = sum(A, A); A = exponent(A, n);
            A[0] = NT::sum(A[0], 1, MOD); A = inverse(A, n);
            A = sum(A, A); A = dif(vi(1, 1), A);
            return A;
        }
        // A[0] == 0, ans[0] == 0
        vi arcsh(const vi &A, const int n) {
            static int MINUS_HALF = (MOD - 1) / 2;
            return integral(prod(derivative(A), power(sum(vi(1, 1), prod(A, A, n - 1)), MINUS_HALF, n - 1), n - 1));
        }
        // A[0] == 0, ans[0] == 0
        vi arcth(const vi &A, const int n) {
            vi A2P1 = prod(A, A, n - 1); A2P1 = dif(vi(1, 1),A2P1);
            return integral(prod(derivative(A), inverse(A2P1, n - 1),n - 1));
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
    };
}
