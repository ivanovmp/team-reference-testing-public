int main() {
    int n;
    cin >> n;

    vvi outcomes(n, vi(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> outcomes[i][j];

    // x0 ... x{n-1} - optimal probabilities
    // x{n}: the optimal loss by us
    const int VARS = n + 1, CONDS = n + 1;
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    vf C(VARS);

    // We are minimizing the loss
    C[n] = -1;

    // The sum of probabilities must be [at least] one
    fill(A[0].begin(), A[0].begin() + n, -1);
    B[0] = -1;

    // Whatever the player choses, we lose at most x{n}
    for (int strategy = 0; strategy < n; ++strategy) {
        // The player chose the strategy **strategy**, so we lose the according money
        for (int i = 0; i < n; ++i)
            A[strategy + 1][i] = outcomes[strategy][i];
        // If instead we just lost x{n},
        A[strategy + 1][n] = -1;
        // then the total loss should be reduced
        B[strategy + 1] = 0;
    }

    // Now the meat begins!
    Simplex<float> simplex(A, B, C, 1e-7);
    vf ans; simplex.solve(ans);

    cout << fixed << setprecision(5);
    for (int i = 0; i < n; ++i) cout << ans[i] << '\n';
}
