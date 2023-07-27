int main() {
    freopen("matrix.in", "r", stdin);
    freopen("matrix.out", "w", stdout);
    int n;
    cin >> n;
    vvi v(n, vi(n));
    const int LB = 101;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            cin >> v[i][j];
            v[i][j] += LB;
        }
    const int CONDS = n + 1, VARS = n + 1;
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    vf C(VARS);
    // x >= 0
    // Ax <= b
    // cT x -> max

    // sum of probabilities is at most 1
    for (int i = 0; i < n; ++i)
        A[0][i] = 1;
    B[0] = 1;

    // for any Steven's pure strategy we win at least x[n]
    for (int i = 0; i < n; ++i) {
        // Steven chose y_i = 1
        // we win x[n]
        A[i + 1][n] = 1;
        // now we lose money for every row of our choice
        for (int j = 0; j < n; ++j)
            A[i + 1][j] = -v[j][i];
        // we have to get nonpositive after this, so B[i + 1] = 0
    }

    // maximize profit
    C[n] = 1;

    // solve
    Simplex<ld> simplex(A, B, C, 1e-8);
    vf ans;
    ld num_ans = simplex.solve(ans);

    cout << fixed << setprecision(5) << num_ans - LB << '\n';
    for (int i = 0; i < n; ++i)
        cout << ans[i] << " \n"[i == n - 1];
}
