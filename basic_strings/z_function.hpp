template<typename Iter>
vi z_function(Iter begin_, Iter end_) {
    const int n = end_ - begin_;
    vi z(n); int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        z[i] = max(0, min(r - i, z[i - l]));
        while (i + z[i] < n && *(begin_ + z[i]) == *(begin_ + i + z[i])) ++z[i];
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
