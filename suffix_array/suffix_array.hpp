vi build_suffix_array(const string& s, const int max_code = 128) { // Codes of all characters from 32 to 127, 0-indexing, 300ms for strings less than 4e5
    const int n = s.size();
    vpii cur_array(n + 1), new_cur_array(n + 1);
    vi cnt(max(n + 1, max_code)), ptr(max(n + 1, max_code)),  value(n + 1);

    for (auto c : s) cnt[c]++;
    ptr[0] = 1, cur_array[0] = {n, 0};
    for (int i = 1; i < max_code; i++) ptr[i] = ptr[i - 1] + cnt[i - 1];
    for (int i = 0; i < n; i++) cur_array[ptr[s[i]]++] = {i, s[i]};
    for (int i = 1; i <= n; i++) {
        cur_array[i].second = cur_array[i - 1].second;
        if (i == 1 || s[cur_array[i].first] != s[cur_array[i - 1].first]) cur_array[i].second++;
    }

    int length = 1;
    while (length < n + 1) {
        for (int i = 0; i <= n; i++) cnt[i] = 0, value[cur_array[i].first] = cur_array[i].second;
        for (auto key : cur_array) cnt[key.second]++;
        ptr[0] = 0;
        for (int i = 1; i <= n; i++) ptr[i] = ptr[i - 1] + cnt[i - 1];

        for (int i = 0; i <= n; i++) {
            int pos = cur_array[i].first - length;
            if (pos < 0) pos += (n + 1);
            new_cur_array[ptr[value[pos]]++] = {pos, cur_array[i].second};
        }

        int tmp = -1, position = 0;
        for (int i = 0; i <= n; i++) {
            int stop = ptr[i];
            if (position == stop) continue;
            tmp++;
            for (int j = position + 1; j < stop; j++) {
                if (new_cur_array[j].second == new_cur_array[j - 1].second) new_cur_array[j - 1].second = tmp;
                else new_cur_array[j - 1].second = tmp++;
            }
            new_cur_array[stop - 1].second = tmp, position = stop;
        }

        cur_array = new_cur_array;
        length *= 2;
    }

    vi suffix_array;
    for (int i = 1; i < (int)cur_array.size(); i++) suffix_array.push_back(cur_array[i].first);
    return suffix_array;
}

vi build_lcp(const string& s, const vi& suffix_array) { // 0-indexing
    int n = s.size(), ptr = 0;
    vi ans(n - 1), pos(n);
    for (int i = 0; i < n; i++) pos[suffix_array[i]] = i;

    for (int i = 0; i < n; i++) {
        int cur = pos[i];
        if (cur == n - 1) {
            ptr = 0;
            continue;
        }
        else ptr = max(0, ptr - 1);
        while (max(i, suffix_array[cur + 1]) + ptr < n && s[i + ptr] == s[suffix_array[cur + 1] + ptr]) ptr++;
        ans[cur] = ptr;
    }

    return ans;
}
