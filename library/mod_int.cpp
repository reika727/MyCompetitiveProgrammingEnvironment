/*
https://atcoder.jp/contests/abc162/submissions/50959288
https://atcoder.jp/contests/abc145/submissions/51218626
https://atcoder.jp/contests/abc333/submissions/50959297
https://atcoder.jp/contests/abc003/submissions/51227162
*/

template <std::unsigned_integral auto Mod>
class basic_mod_int final {
public:
    using integer_type = decltype(Mod);

private:
    inline static std::map<integer_type, basic_mod_int> inverse_cache;
    constexpr inline static auto max = std::numeric_limits<integer_type>::max();
    integer_type num;

public:
    constexpr inline static auto mod = Mod;

    basic_mod_int() = default;

    basic_mod_int(integer_type n)
        : num(n < mod ? n : n % mod)
    {
    }

    integer_type value() const
    {
        return num;
    }

    bool operator==(const basic_mod_int &mi) const
    {
        return num == mi.num;
    }

    bool operator!=(const basic_mod_int &mi) const
    {
        return !operator==(mi);
    }

    basic_mod_int operator~() const
    {
        if (inverse_cache.contains(num)) {
            return inverse_cache[num];
        } else {
            return inverse_cache[num] = pow(mod - 2);
        }
    }

    basic_mod_int operator+(const basic_mod_int &mi) const
    {
        if constexpr (mod - 1 <= max - (mod - 1)) {
            return num + mi.num;
        } else {
            if (num <= mod - mi.num) {
                return num + mi.num;
            } else {
                return num - (mod - mi.num);
            }
        }
    }

    basic_mod_int operator-(const basic_mod_int &mi) const
    {
        return operator+(mod - mi.num);
    }

    basic_mod_int operator*(const basic_mod_int &mi) const
    {
        if constexpr (mod - 1 <= max / (mod - 1)) {
            return num * mi.num;
        } else {
            if (num <= mod / mi.num) {
                return num * mi.num;
            } else {
                basic_mod_int mult = 1;
                auto a = *this;
                for (auto b = mi.num; b; b >>= 1, a += a) {
                    if (b & 1) {
                        mult += a;
                    }
                }
                return mult;
            }
        }
    }

    basic_mod_int operator/(const basic_mod_int &mi) const
    {
        return operator*(~mi);
    }

    template <std::unsigned_integral T>
    basic_mod_int pow(T b) const
    {
        basic_mod_int ans = 1;
        for (auto a = *this; b; a *= a, b >>= 1) {
            if (b & 1) {
                ans *= a;
            }
        }
        return ans;
    }

    basic_mod_int &operator+=(const basic_mod_int &mi)
    {
        return *this = operator+(mi);
    }

    basic_mod_int &operator-=(const basic_mod_int &mi)
    {
        return *this = operator-(mi);
    }

    basic_mod_int &operator*=(const basic_mod_int &mi)
    {
        return *this = operator*(mi);
    }

    basic_mod_int &operator/=(const basic_mod_int &mi)
    {
        return *this = operator/(mi);
    }
};

template <std::unsigned_integral auto Mod>
class basic_combination_modulo final {
private:
    using mod_int = basic_mod_int<Mod>;

    std::vector<mod_int> factorial_mod;

public:
    basic_combination_modulo(const std::size_t max_n)
    {
        const auto i = std::views::iota(1uz, max_n + 2);
        factorial_mod.reserve(i.size());
        std::transform_exclusive_scan(
            i.begin(), i.end(),
            std::back_inserter(factorial_mod),
            mod_int{1},
            std::multiplies{},
            [](const auto x){return mod_int{x};}
        );
    }

    mod_int operator()(const std::size_t n, const std::size_t r) const
    {
        return n < r ? 0 : factorial_mod[n] / (factorial_mod[r] * factorial_mod[n - r]);
    }
};
