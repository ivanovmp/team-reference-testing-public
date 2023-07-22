#pragma once
/* Thanks to Um_nik!
 * problem: x >= 0 and A x <= b
 * returns:
   * inf if unbounded above
   * -inf if infeasible
   * max (cT x) otherwise
 */
template<class db>
struct Simplex {
    using VD = vector<db>;
    using VVD = vector<VD>;
    // EPS = 1e-7 for float, EPS = 1e-12 for double
    const db EPS, INF = numeric_limits<db>::infinity();
    int m, n; vi B, N; VVD D;
    Simplex(const VVD& A, const VD& b, const VD& c, db eps) : m(b.size()), n(c.size()), B(m), N(n + 1, -1), D(m + 2, VD(n + 2)), EPS(eps) {
        for (int i = 0; i < m; ++i) {
            copy(A[i].begin(), A[i].end(), D[i].begin());
            B[i] = n + i;
            D[i][n] = -1;
            D[i][n + 1] = b[i];
        }
        for (int j = 0; j < n; ++j) {
            N[j] = j;
            D[m][j] = -c[j];
        }
        D[m + 1][n] = 1;
    }
    void pivot(int r, int s) {
        for (int i = 0; i < m + 2; ++i) if (i != r)
            for (int j = 0; j < n + 2; ++j) if (j != s)
                D[i][j] -= D[r][j] * D[i][s] / D[r][s];
            for (int j = 0; j < n + 2; ++j) if (j != s)
                D[r][j] /= D[r][s];
            for (int i = 0; i < m + 2; ++i) if (i != r)
                D[i][s] /= -D[r][s];
            D[r][s] = 1. / D[r][s];
            swap(B[r], N[s]);
    }
    bool simplex(int p) {
        int x = m + (p == 1);
        while (true) {
            int s = -1; db v = -1;
            for (int j = 0; j <= n; ++j) {
                if (p == 2 && N[j] == -1) continue;
                db q = 0;
                for (int k = 0; k <= m; ++k)
                    q += pow(D[k][j], 2);
                q = max(q, EPS);
                db vj = D[x][j] / sqrt(q);
                if (s == -1 || mp(vj, N[j]) < mp(v, N[s])) s = j, v = vj;
            }
            if (D[x][s] >= -EPS) return true;
            int r = -1;
            for (int i = 0; i < m; ++i) if (D[i][s] > EPS)
                if (r == -1 || mp(D[i][n + 1] / D[i][s], B[i]) < mp(D[r][n + 1] / D[r][s], B[r])) r = i;
            if (r == -1) return false;
            pivot(r, s);
        }
    }
    db solve(VD& x) {
        int r = 0;
        for (int i = 1; i < m; ++i) if (D[i][n + 1] < D[r][n + 1]) r = i;
        if (D[r][n + 1] <= -EPS) {
            pivot(r, n);
            if (!simplex(1) || D[m + 1][n + 1] < -EPS) return -INF;
            for (int i = 0; i < m; ++i) if (B[i] == -1) {
                int s = -1;
                for (int j = 0; j <= n; ++j)
                    if (s == -1 || mp(D[i][j], N[j]) < mp(D[i][s], N[s])) s = j;
                pivot(i, s);
            }
        }
        if (!simplex(2)) return INF;
        x = VD(n);
        for (int i = 0; i < m; ++i) if (B[i] < n)
            x[B[i]] = D[i][n + 1];
        return D[m][n + 1];
    }
};
