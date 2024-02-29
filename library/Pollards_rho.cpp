/*
https://atcoder.jp/contests/abc284/submissions/50713480
*/

template <std::unsigned_integral T>
T mod_addition(T a, T b, T mod)
{
    a %= mod;
    b %= mod;
    if (a < mod - b) {
        return a + b;
    } else {
        return a - (mod - b);
    }
};

template <std::unsigned_integral T>
T mod_multiply(T a, T b, T mod)
{
    T ans = 0;
    for (; b; b >>= 1, a = mod_addition(a, a, mod)) {
        if (b & 1) {
            ans = mod_addition(ans, a, mod);
        }
    }
    return ans;
}

template <std::unsigned_integral T>
T mod_pow(T a, T b, T mod)
{
    T ans = 1;
    for (; b; b >>= 1, a = mod_multiply(a, a, mod)) {
        if (b & 1) {
            ans = mod_multiply(ans, a, mod);
        }
    }
    return ans;
}

class Miller_Rabin final {
private:
    template <std::unsigned_integral T>
    static bool is_possibly_prime(
        const T n,
        std::ranges::input_range auto &&witnesses
    )
    {
        if (n == 2) {
            return true;
        } else if (n < 2 || n % 2 == 0) {
            return false;
        }
        const auto m = n - 1;
        const auto s = std::countr_zero(m);
        const auto d = m >> s;
        for (const T a : witnesses) {
            if (a >= n) {
                continue;
            }
            auto x = mod_pow(a, d, n);
            if (x == 1) {
                continue;
            }
            for (auto r = 0; x != m; ++r, x = mod_pow(x, static_cast<T>(2), n)) {
                if (r == s - 1 || x == 1) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    template <
        std::unsigned_integral T,
        std::unsigned_integral U,
        std::uniform_random_bit_generator Generator = std::mt19937
    >
    static bool test(
        const T n,
        const U repitition,
        const Generator::result_type seed = std::random_device{}()
    )
    {
        /* TODO: use std::generator after C++23 */
        Generator engine(seed);
        std::uniform_int_distribution<T> dist(2, n - 1);
        std::vector<T> random_numbers;
        random_numbers.reserve(repitition);
        while (random_numbers.size() < repitition) {
            random_numbers.push_back(dist(engine));
        }
        return is_possibly_prime(n, random_numbers);
    }

    template <std::unsigned_integral T>
    static bool test_uint64(const T n)
    {
        static_assert(std::numeric_limits<T>::digits <= 64);
        constexpr char witnesses[] = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
        };
        return is_possibly_prime(n, witnesses);
    }

    Miller_Rabin() = delete;
};

class Pollards_rho final {
private:
    template <std::unsigned_integral T>
    static std::optional<T> search_nontrivial_factor(const T n)
    {
        if (n % 2 == 0) {
            return 2;
        }
        for (T c = 1; c < n; ++c) {
            const auto f = [n, c](const T x) {
                return mod_addition(mod_pow(x, static_cast<T>(2), n), c, n);
            };
            for (T x = f(0), y = f(f(0)); ; x = f(x), y = f(f(y))) {
                const auto g = std::gcd(std::max(x, y) - std::min(x, y), n);
                if (g == 1) {
                    continue;
                }
                if (g == n) {
                    break;
                }
                return g;
            }
        }
        return std::nullopt;
    }

    template <std::unsigned_integral T, class P>
        requires std::predicate<P, T>
    static std::optional<T> search_prime_factor(const T n, const P check_primality)
    {
        for (std::optional<T> f = n; f; f = search_nontrivial_factor(*f)) {
            if (check_primality(*f)) {
                return *f;
            } else if (check_primality(n / *f)) {
                return n / *f;
            }
        }
        return std::nullopt;
    }

public:
    template <std::unsigned_integral T, class P>
        requires std::predicate<P, T>
    static std::map<T, T> factorize(T n, const P check_primality)
    {
        std::map<T, T> factors;
        while (n > 1) {
            const auto p = search_prime_factor(n, check_primality);
            if (!p) {
                break;
            }
            for (; n % *p == 0; n /= *p) {
                ++factors[*p];
            }
        }
        return factors;
    }

    template <std::unsigned_integral T>
    static std::map<T, T> factorize_uint64(const T n)
    {
        return factorize(n, [](const auto n){return Miller_Rabin::test_uint64(n);});
    }

    Pollards_rho() = delete;
};
