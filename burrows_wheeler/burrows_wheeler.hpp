namespace burrows_wheeler {
    pair<vi, int> transform(vi s, const int alphabet) {
        const int n = s.size();
        for (int i = 0; i < n; ++i) s.push_back(s[i]); // or use cyclic sufarr
        vi suf = build_suffix_array<uint8_t>(s.begin(), s.end(), alphabet);
        pair<vi, int> ans;
        ans.x.resize(n);
        int j = 0;
        for (int i = 0; i < n * 2; ++i)
            if (suf[i] < n)
                if (suf[i] == 0) {
                    ans.y = j;
                    ans.x[j++] = s.back();
                } else
                    ans.x[j++] = s[suf[i] - 1];
        return ans;
    }
    vi inverse_transform(const vi& t, int pos, const int alphabet) {
        const int n = t.size();
        vi occurrences(alphabet),
           repeats_before(n);
        for (int i = 0; i < n; ++i)
            repeats_before[i] = occurrences[t[i]]++;
        vi number_of_less(alphabet);
        for (int i = 0; i < alphabet - 1; ++i)
            number_of_less[i + 1] = number_of_less[i] + occurrences[i];
        vi ans(n);
        for (int i = n - 1; i >= 0; --i) {
            ans[i] = t[pos];
            pos = number_of_less[ans[i]] + repeats_before[pos];
        }
        return ans;
    }
}
