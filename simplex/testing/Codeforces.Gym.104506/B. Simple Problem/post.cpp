int main() {
    freopen("simplex.in", "r", stdin);
    freopen("simplex.out", "w", stdout);
    int VARS, CONDS;
    cin >> VARS >> CONDS;
    vvf A(CONDS, vf(VARS));
    vf B(CONDS);
    vf C(VARS);
    for (int cond = 0; cond < CONDS; ++cond) {
        for (int var = 0; var < VARS; ++var) {
            db x;
            cin >> x;
            A[cond][var] = x;
        }
        db x;
        cin >> x;
        B[cond] = x;
    }
    for (int var = 0; var < VARS; ++var) {
        db x;
        cin >> x;
        C[var] = x;
    }
    Simplex<db> simplex(A, B, C, 1e-8);
    vf ans; db ans_db = simplex.solve(ans);
    cout << fixed << setprecision(13);
    if (ans.empty())
        if (ans_db == numeric_limits<db>::infinity())
            cout << "Unbounded\n";
        else
            cout << "No solution\n";
    else {
        cout << "Bounded\n";
        cout << fixed << setprecision(11);
        for (int var = 0; var < VARS; ++var)
            cout << ans[var] << " \n"[var == VARS - 1];
    }
}
