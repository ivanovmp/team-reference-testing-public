int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    string s, t;
    getline(cin, s);
    getline(cin, s);
    getline(cin, t);
    const string g = s + ' ' + t;
    const vi suf = build_suffix_array(g, 256);
    const vi lcp = build_lcp(g, suf);

    vi possible(n); {
        int s_position = find(suf.begin(), suf.end(), 0) - suf.begin();
        int common = n;
        for (int i = s_position - 1; i >= 0; --i) {
            common = min(common, lcp[i]);
            if (suf[i] + common == 2 * n + 1)
                ++possible[n - common];
        }
    } {
        int t_position = find(suf.begin(), suf.end(), n + 1) - suf.begin();
        int common = n;
        for (int i = t_position - 1; i >= 0; --i) {
            common = min(common, lcp[i]);
            if (suf[i] + common == n)
                ++possible[common];
        }
    }
    int ans = find(possible.begin(), possible.end(), 2) - possible.begin();
    if (ans == n) ans = -1;
    cout << ans << '\n';
}
