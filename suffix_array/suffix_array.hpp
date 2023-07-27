template<class Char, class Iter> // if beg and en are string, then Char should be uint8_t
vi build_suffix_array(Iter beg, Iter en, const int alphabet = 128) { // Codes of all characters from 32 to 127, 0-indexing, 300ms for strings less than 4e5
    const int n = en - beg;
    vpii cur_array(n + 1), new_cur_array(n + 1);
    vi cnt(max(n + 1, alphabet)), ptr(max(n + 1, alphabet)), value(n + 1);

    for (auto it = beg; it != en; ++it) cnt[(Char)*it]++;
    ptr[0] = 1, cur_array[0] = {n, 0};
    for (int i = 1; i < alphabet; i++) ptr[i] = ptr[i - 1] + cnt[i - 1];
    for (int i = 0; i < n; i++) cur_array[ptr[(Char)beg[i]]++] = {i, (Char)beg[i]};
    for (int i = 1; i <= n; i++) {
        cur_array[i].second = cur_array[i - 1].second;
        if (i == 1 || beg[cur_array[i].first] != beg[cur_array[i - 1].first]) cur_array[i].second++;
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
template<class Iter>
vi build_lcp(Iter beg, Iter en, const vi& suffix_array) { // 0-indexing
    int n = en - beg, ptr = 0;
    vi ans(n - 1), pos(n);
    for (int i = 0; i < n; i++) pos[suffix_array[i]] = i;

    for (int i = 0; i < n; i++) {
        int cur = pos[i];
        if (cur == n - 1) {
            ptr = 0;
            continue;
        }
        else ptr = max(0, ptr - 1);
        while (max(i, suffix_array[cur + 1]) + ptr < n && beg[i + ptr] == beg[suffix_array[cur + 1] + ptr]) ptr++;
        ans[cur] = ptr;
    }

    return ans;
}
