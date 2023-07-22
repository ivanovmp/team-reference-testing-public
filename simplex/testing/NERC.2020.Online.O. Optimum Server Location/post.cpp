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

    const int VARS = n * (m + 1) + n * (n - 1) / 2, CONDS = n * (2 * m + n - 1);
    // vars:
    //   0..n-1 are the positions of servers
    //   n..n+mn-1 are distances between regular and CDN servers:
    //     n + i * m + j is the distance between regular i-th server and CDN j-th server
    // n+mn..n+mn+n(n-1)/2-1 are distances between regular servers
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    // (i,j) distance is at least x[i]-y[j] and at least y[j]-x[i]
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            A[2 * (i * m + j)][i] = 1;
            A[2 * (i * m + j)][n + i * m + j] = -1;
            B[2 * (i * m + j)] = a[j];

            A[2 * (i * m + j) + 1][i] = -1;
            A[2 * (i * m + j) + 1][n + i * m + j] = -1;
            B[2 * (i * m + j) + 1] = -a[j];
        }
    // (i,j) distance is at least x[i]-x[j] and at least x[j]-x[i]
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j) {
            A[2 * m * n + i * (i - 1) + 2 * j][i] = 1;
            A[2 * m * n + i * (i - 1) + 2 * j][j] = -1;
            B[2 * m * n + i * (i - 1) + 2 * j][i * (i - 1) / 2 + j] = -1;

            A[2 * m * n + i * (i - 1) + 2 * j + 1][i] = -1;
            A[2 * m * n + i * (i - 1) + 2 * j + 1][j] = 1;
            B[2 * m * n + i * (i - 1) + 2 * j + 1][i * (i - 1) / 2 + j] = -1;
        }
    // We're trying to maximize MINUS ans
    vf C(VARS);
    // firstly, the distances between servers
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < 9; ++j) {
            C[i * (i - 1) / 2 + j] -= d[i][j];
        }
    // secondly, the pairwise distances between servers and CDN servers
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            C[n + i * m + j] = -c[i][j];
        }
    Simplex<ld> simplex(A, B, C, 1e-15);
    vf ans;
    ld num_ans = -simplex.solve(ans);
    cout << fixed << setprecision(0) << num_ans << '\n';
    for (int i = 0; i < n; ++i)
        cout << max<ld>(0, ans[i]) << " \n"[i == n - 1];
}
