/*
https://atcoder.jp/contests/abc284/submissions/50724473
*/

template <std::unsigned_integral UInt>
UInt mod_addition(UInt a, UInt b, UInt mod)
{
    a %= mod;
    b %= mod;
    if (a < mod - b) {
        return a + b;
    } else {
        return a - (mod - b);
    }
};

template <std::unsigned_integral UInt>
UInt mod_multiply(UInt a, UInt b, UInt mod)
{
    UInt ans = 0;
    for (; b; b >>= 1, a = mod_addition(a, a, mod)) {
        if (b & 1) {
            ans = mod_addition(ans, a, mod);
        }
    }
    return ans;
}

template <std::unsigned_integral UInt>
UInt mod_pow(UInt a, UInt b, UInt mod)
{
    UInt ans = 1;
    for (; b; b >>= 1, a = mod_multiply(a, a, mod)) {
        if (b & 1) {
            ans = mod_multiply(ans, a, mod);
        }
    }
    return ans;
}

class Miller_Rabin final {
private:
    template <std::unsigned_integral UInt>
    static bool is_possibly_prime(
        const UInt n,
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
        for (const UInt a : witnesses) {
            if (a >= n) {
                continue;
            }
            auto x = mod_pow(a, d, n);
            if (x == 1) {
                continue;
            }
            for (auto r = 0; x != m; ++r, x = mod_pow(x, static_cast<UInt>(2), n)) {
                if (r == s - 1 || x == 1) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    template <
        std::unsigned_integral UInt,
        std::uniform_random_bit_generator Generator = std::mt19937
    >
    static bool test(
        const UInt n,
        const std::unsigned_integral auto repitition,
        const Generator::result_type seed = std::random_device{}()
    )
    {
        /* TODO: use std::generator after C++23 */
        Generator engine(seed);
        std::uniform_int_distribution<UInt> dist(2, n - 1);
        std::vector<UInt> random_numbers;
        random_numbers.reserve(repitition);
        while (random_numbers.size() < repitition) {
            random_numbers.push_back(dist(engine));
        }
        return is_possibly_prime(n, random_numbers);
    }

    template <std::unsigned_integral UInt>
    static bool test_uint64(const UInt n)
    {
        static_assert(std::numeric_limits<UInt>::digits <= 64);
        constexpr unsigned char witnesses[] = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
        };
        return is_possibly_prime(n, witnesses);
    }

    Miller_Rabin() = delete;
};

class Pollards_rho final {
private:
    template <std::unsigned_integral UInt, class Polynomial>
        requires std::is_invocable_r_v<UInt, Polynomial, UInt>
    static std::optional<UInt> search_nontrivial_factor(
        const UInt n,
        const Polynomial f
    )
    {
        for (UInt x = f(0), y = f(f(0)); ; x = f(x), y = f(f(y))) {
            const auto g = std::gcd(std::max(x, y) - std::min(x, y), n);
            if (g == 1) {
                continue;
            }
            if (g == n) {
                break;
            }
            return g;
        }
        return std::nullopt;
    }

    template <std::unsigned_integral UInt, class Primality, class Polynomial>
        requires std::predicate<Primality, UInt> &&
                 std::is_invocable_r_v<UInt, Polynomial, UInt>
    static std::optional<UInt> search_prime_factor(
        const UInt n,
        const Primality check_primality,
        const Polynomial f
    )
    {
        for (
            std::optional factor = n;
            factor;
            factor = search_nontrivial_factor(*factor, f)
       ) {
            if (check_primality(*factor)) {
                return *factor;
            } else if (check_primality(n / *factor)) {
                return n / *factor;
            }
        }
        return std::nullopt;
    }

    template <std::unsigned_integral UInt, class Primality>
        requires std::predicate<Primality, UInt>
    static std::optional<UInt> search_prime_factor_with_quadratics(
        const UInt n,
        const Primality check_primality
    )
    {
        if (n % 2 == 0) {
            return 2;
        }
        for (UInt c = 1; c < n; ++c) {
            const auto f = [n, c](const UInt x) {
                return mod_addition(mod_pow(x, static_cast<UInt>(2), n), c, n);
            };
            if (const auto p = search_prime_factor(n, check_primality, f)) {
                return p;
            }
        }
        return std::nullopt;
    }

public:
    template <std::unsigned_integral UInt, class Primality>
        requires std::predicate<Primality, UInt>
    static std::map<UInt, UInt> factorize(UInt n, const Primality check_primality)
    {
        std::map<UInt, UInt> factors;
        while (n > 1) {
            const auto p = search_prime_factor_with_quadratics(n, check_primality);
            if (!p) {
                break;
            }
            for (; n % *p == 0; n /= *p) {
                ++factors[*p];
            }
        }
        return factors;
    }

    template <std::unsigned_integral UInt>
    static std::map<UInt, UInt> factorize_uint64(const UInt n)
    {
        return factorize(n, static_cast<bool(*)(UInt)>(Miller_Rabin::test_uint64));
    }

    Pollards_rho() = delete;
};
