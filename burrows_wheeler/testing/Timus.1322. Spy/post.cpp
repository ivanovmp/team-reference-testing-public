int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    string s;
    cin >> s;
    for (char c : burrows_wheeler::inverse_transform(vi(s.begin(), s.end()), n - 1, 256))
        cout << c;
    cout << '\n';
}
