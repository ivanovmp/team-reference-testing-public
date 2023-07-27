using Automaton = suffix::TemplateAutomaton<suffix::MapVertex<map>>;

vb accepted_prefixes(const Automaton a, const vi& s) {
    const int n = s.size();
    int state = 0;
    vb ans(n + 1);
    ans[0] = true;
    vb term = a.get_terminal_states();
    for (int i = 0; i < n; ++i) {
        state = a.next_step[state].get_val(s[i]);
        if (state == -1)
            return ans;
        if (term[state])
            ans[i + 1] = true;
    }
    return ans;
}

vb accepted_prefixes(const vi& si, const vi& ti) {
    const int A = 256;
    return accepted_prefixes(Automaton(si, A), ti);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    string s, t;
    getline(cin, s);
    getline(cin, s);
    getline(cin, t);
    vi si(s.begin(), s.end());
    vi ti(t.begin(), t.end());
    for (int i = 0; i < n; ++i) {
        if (si[i] < 0) si[i] += 256;
        if (ti[i] < 0) ti[i] += 256;
    }
    vb sc = accepted_prefixes(si, ti), tc = accepted_prefixes(ti, si);
    reverse(tc.begin(), tc.end());
    int ans = -1;
    for (int i = 0; i < n; ++i)
        if (sc[i] && tc[i]) {
            ans = i;
            break;
        }
    cout << ans << '\n';
}
