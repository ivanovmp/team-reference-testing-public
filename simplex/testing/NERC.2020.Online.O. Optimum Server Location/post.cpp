int main() {
    int n, m;
    cin >> n >> m;
    vi a(m);
    for (int j = 0; j < m; ++j)
        cin >> a[j];
    vvi c(n, vi(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> c[i][j];
    vvi d(n, vi(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> d[i][j];

    const int VARS = n * (m + 1), CONDS = 0;
    // vars:
    //   0..n-1 are the positions of servers
    //   n..n+mn-1 are distances between regular and CDN servers:
    //     n + i * m + j is the distance between regular i-th server and CDN j-th server
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    // servers are non-decreasing
    for (int i = 0; i < n - 1; ++i) {
        A[i][i] = 1;
        A[i][i + 1] = -1;
    }
    // (i,j) distance is at least x[i]-y[j] and at least y[j]-x[i]
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            A[n - 1 + 2 * (i * m + j)][i] = 1;
            A[n - 1 + 2 * (i * m + j)][n + i * m + j] = -1;
            B[n - 1 + 2 * (i * m + j)] = -a[j];

            A[n - 1 + 2 * (i * m + j)][i] = -1;
            A[n - 1 + 2 * (i * m + j)][n + i * m + j] = -1;
            B[n - 1 + 2 * (i * m + j)] = a[j];
        }
    // We're trying to maximize MINUS ans
    vf C(VARS);
    // firstly, the distances between servers
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            C[j] -= d[i][j];
            C[i] += d[i][j];
        }
    // secondly, the pairwise distances between servers and CDN servers
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            C[n + i * m + j] = -c[i][j];
        }
    Simplex<ld> simplex(A, B, C);
    vf ans;
    ld num_ans = -simplex.solve(ans);
    cout << fixed << setprecision(0) << num_ans << '\n';
    for (int i = 0; i < n; ++i)
        cout << ans[i] << " \n"[i == n - 1];
}
