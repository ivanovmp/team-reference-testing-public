int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    string s, t;
    getline(cin, s);
    getline(cin, s);
    getline(cin, t);
    const string g = s + t + t;
    vi z = z_function(g.begin(), g.end());
    for (int i = 0; i < n; ++i)
        if (z[i + n] >= n) {
            cout << i << '\n';
            return 0;
        }
    cout << "-1\n";
}
