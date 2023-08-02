#include "../../../utils/testing.hpp"
#include "../../../number_theory/NT.hpp"
#include "../../FFT.hpp"
#include "../../NTT.hpp"

FFT::FFT<float> fft_float(23);
FFT::FFT<double> fft_double(23);
FFT::FFT<long double> fft_long_double(23);
FFT::NTT ntt(23);

template<class T>
vector<T> generate_float_vector(const int n) {
    static mt19937 rng;
    vector<T> ans(n);
    uniform_real_distribution<T> seg(-1000, 1000);
    for (int i = 0; i < n; ++i)
        ans[i] = seg(rng);
    return ans;
}

template<class T>
vector<T> generate_int_vector(const int n) {
    static mt19937 rng;
    vector<T> ans(n);
    uniform_int_distribution<T> seg(0, 2000);
    for (int i = 0; i < n; ++i)
        ans[i] = seg(rng);
    return ans;
}

std::function<long long()> generate_test(int n, const string type) {
    if (type == "multiply_float") {
        vector<float> f = generate_float_vector<float>(1 << n),
                      g = generate_float_vector<float>(1 << n);
        return [f, g, n]() {
            auto a = fft_float.prod(f, g, n + 1);
            ll ans = 0;
            return (long long)a[n];
        };
    } else if (type == "multiply_double") {
        vector<double> f = generate_float_vector<double>(1 << n),
                g = generate_float_vector<double>(1 << n);
        return [f, g, n]() {
            auto a = fft_double.prod(f, g, n + 1);
            ll ans = 0;
            return (long long)a[n];
        };
    } else if (type == "multiply_long_double") {
        vector<long double> f = generate_float_vector<long double>(1 << n),
                g = generate_float_vector<long double>(1 << n);
        return [f, g, n]() {
            auto a = fft_long_double.prod(f, g, n + 1);
            ll ans = 0;
            return (long long)a[n];
        };
    } else if (type == "multiply_int") {
        vector<int> f = generate_int_vector<int>(1 << n),
                    g = generate_int_vector<int>(1 << n);
        return [f, g, n]() {
            auto a = ntt.prod(f, g);
            return (long long)a[n];
        };
    } else if (type == "exp_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            auto a = ntt.exponent(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "log_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 1;
        return [f, n]() {
            auto a = ntt.logarithm(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "sin_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            auto a = ntt.sin(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "arcsin_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            auto a = ntt.arcsin(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "cos_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            auto a = ntt.cos(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "arccos_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 1;
        return [f, n]() {
            //auto a = ntt.arccos(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "tg_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.tg(f, 1 << n);
            //return (long long)a[n];
            return 0;
        };
    } else if (type == "arctg_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.arctg(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "sh_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.sh(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "arcsh_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.arcsh(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "ch_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.ch(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "arcch_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 1;
        return [f, n]() {
            //auto a = ntt.arcch(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "th_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.th(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "arcth_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            //auto a = ntt.arcth(f, 1 << n);
            //return (long long)a[n];
            return 0ll;
        };
    } else if (type == "inverse_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        ++f[0];
        return [f, n]() {
            auto a = ntt.inverse(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "euler_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 0;
        return [f, n]() {
            auto a = ntt.euler_transform(f, 1 << n);
            return (long long)a[n];
        };
    } else if (type == "inverse_euler_int") {
        vector<int> f = generate_int_vector<int>(1 << n);
        f[0] = 1;
        return [f, n]() {
            auto a = ntt.inverse_euler_transform(f, 1 << n);
            return (long long)a[n];
        };
    } else
        throw runtime_error("Unknown type: " + type);
}

int main() {
    const int N = 18;
    const int MAX_TESTS = 30, TESTS = 800;
    vector<string> types = {
            "multiply_float",
            "multiply_double",
            "multiply_long_double",
            "multiply_int",
            "inverse_int",
            "exp_int",
            "log_int",
            "sin_int",
            "cos_int",
            "tg_int",
            "arcsin_int",
            "arctg_int",
            "sh_int",
            "ch_int",
            "th_int",
            "arcsh_int",
            "arcth_int",
            "euler_int",
            "inverse_euler_int",
    };
    vector<vector<vector<long long>>> times(types.size(), vector<vector<long long>>(N));
    long long sum = 0;
    for (int test = 0; test < TESTS; ++test) {
        cerr << "Done " << test << " tests out of " << TESTS << '\n';
        int curN;
        for (curN = 0; curN < N; ++curN)
            if (test >= MAX_TESTS - MAX_TESTS / 3 + (MAX_TESTS / 3 << (N - 1 - curN)))
                break;
        for (int n = 0; n < curN; ++n) {
            cerr << "\tDone " << n << " sizes out of " << curN << '\n';
            for (int type_i = 0; type_i < types.size(); ++type_i) {
                string type = types[type_i];
                auto task = generate_test(n, type);
                auto beginning_time = chrono::high_resolution_clock::now();
                long long ans = task();
                auto ending_time = chrono::high_resolution_clock::now();
                sum += ans;
                times[type_i][n].push_back((ending_time - beginning_time).count());
            }
        }
    }
    cout << "Check sum: " << sum << '\n';

    cout << "[";
    for (int type_i = 0; type_i < types.size(); ++type_i) {
        cout << "(";
        cout << "\"" << types[type_i] << "\",";
        cout << "[";
        for (int n = 0; n < N; ++n) {
            cout << "[";
            for (long long t : times[type_i][n])
                cout << t << ",";
            cout << "],";
        }
        cout << "]";
        cout << "),";
    }
    cout << "]\n";
}
