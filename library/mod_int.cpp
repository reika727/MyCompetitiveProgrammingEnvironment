/*
https://atcoder.jp/contests/abc162/submissions/50714576
https://atcoder.jp/contests/abc145/submissions/50714592
https://atcoder.jp/contests/abc333/submissions/50714598
*/

template <std::unsigned_integral auto Mod>
class basic_mod_int final {
public:
    using integer_type = decltype(Mod);

private:
    inline static std::map<integer_type, basic_mod_int> inverse_cache;
    constexpr inline static integer_type max = std::numeric_limits<integer_type>::max();
    integer_type num;

public:
    constexpr inline static integer_type mod = Mod;

    template <std::unsigned_integral T>
    static basic_mod_int pow(basic_mod_int x, T y)
    {
        basic_mod_int ans = 1;
        for (; y; x *= x, y >>= 1) {
            if (y & 1) {
                ans *= x;
            }
        }
        return ans;
    }

    basic_mod_int() = default;

    basic_mod_int(integer_type n)
        : num(n % mod)
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
            return inverse_cache[num] = pow(*this, mod - 2);
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
                auto b = mi.num;
                for (; b; b >>= 1, a += a) {
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
class combination_modulo final {
private:
    using mod_int = basic_mod_int<Mod>;

    std::vector<mod_int> factorial_mod;

    using size_type = decltype(factorial_mod)::size_type;

public:
    combination_modulo(const size_type max_n)
        : factorial_mod(max_n + 1)
    {
        factorial_mod[0] = 1;
        for (size_type i = 1; i <= max_n; ++i) {
            factorial_mod[i] = factorial_mod[i - 1] * i;
        }
    }

    mod_int operator()(const size_type n, const size_type r)
    {
        return n < r ? 0 : factorial_mod[n] / (factorial_mod[r] * factorial_mod[n - r]);
    }
};
