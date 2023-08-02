int main() {
    int n, m;
    cin >> n >> m;
    int VARS = m, CONDS = m + n - 1;
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    vf C(VARS);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        --a; --b;
        A[i][i] = 1;
        B[i] = c;
        if (a != 0)
            A[m + a - 1][i] = 1;
        if (a == n - 1)
            C[i] = -1;
        if (b != 0)
            A[m + b - 1][i] = -1;
        if (b == n - 1)
            C[i] = 1;
    }
    Simplex<db> simplex(A, B, C, 1e-11);
    vf ans; db ans_db = simplex.solve(ans);
    cout << int(round(ans_db)) << '\n';
    for (int i = 0; i < m; ++i)
        cout << int(round(ans[i])) << " \n"[i == m - 1];
}
