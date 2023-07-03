#include<bits/stdc++.h>

using namespace std;

vector < int > build_odd_palindromes(string s) {
    int n = s.size();
    vector < int > length(n, 1);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) length[i] = min(length[l + r - i], r - i + 1);
        while (i - length[i] >= 0 && i + length[i] < n && s[i - length[i]] == s[i + length[i]]) length[i]++;
        if (i + length[i] - 1 > r) {
            l = i - length[i] + 1, r = i + length[i] - 1;
        }
    }
    return length;
}

vector < int > build_even_palindromes(string s) {
    int n = s.size();
    vector < int > length(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i <= r) length[i] = min(length[l + r - i + 1], r - i + 1);
        while (i - length[i] - 1 >= 0 && i + length[i] < n && s[i - length[i] - 1] == s[i + length[i]]) length[i]++;
        if (i + length[i] - 1 > r) {
            l = i - length[i], r = i + length[i] - 1;
        }
    }
    return length;
}
