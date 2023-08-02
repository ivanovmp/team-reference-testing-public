int main() {
    int n;
    cin >> n;
    int VARS = n * n, CONDS = 2 * n;
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    vf C(VARS);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int x;
            cin >> x;
            A[i][i * n + j] = 1;
            A[j + n][i * n + j] = -1;
            C[i * n + j] = -x;
        }
        B[i] = 1;
        B[i + n] = -1;
    }
    Simplex<db> simplex(A, B, C, 1e-7);
    vf ans; db ans_db = simplex.solve(ans);
    cout << -int(round(ans_db)) << '\n';
}
