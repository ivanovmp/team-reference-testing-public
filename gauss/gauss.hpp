template<class T, class U>
int gauss (vector<vector<T>>& a, vector<T>& ans, vi& where, vi& perm,
           T (*sum)(T, T), T (*dif)(T, T), T (*prod)(T, T), T (*inv)(T), U (*norm)(T), bool (*invertible)(T)) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;
    where.assign(m, -1);
    perm.resize(n); for (int i = 0; i < n; ++i) perm[i] = i;
    for (int col=0, row=0; col<m && row<n; ++col) {
        int sel = row;
        for (int i=row; i<n; ++i)
            if (norm (a[i][col]) > norm (a[sel][col]))
                sel = i;
        if (!invertible(a[sel][col]))
            continue;
        swap(perm[sel], perm[row]);
        for (int i=col; i<=m; ++i)
            swap (a[sel][i], a[row][i]);
        where[col] = row;
        T dv = inv(a[row][col]);
        for (int i=0; i<n; ++i)
            if (i != row) {
                T c = prod(a[i][col], dv);
                for (int j=col; j<=m; ++j)
                    a[i][j] = dif(a[i][j], prod(a[row][j], c));
            }
        ++row;
    }
    ans.assign (m, 0);
    for (int i=0; i<m; ++i)
        if (where[i] != -1)
            ans[i] = prod(a[where[i]][m], inv(a[where[i]][i]));
    for (int i=0; i<n; ++i) {
        T s = 0;
        for (int j=0; j<m; ++j)
            s = sum(s, prod(ans[j], a[i][j]));
        if (invertible(dif(s, a[i][m])))
            return 0;
    }
    for (int i=0; i<m; ++i)
        if (where[i] == -1)
            return 2e9;
    return 1;
}
