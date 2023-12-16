/*
https://atcoder.jp/contests/abc162/submissions/11866750
https://atcoder.jp/contests/abc145/submissions/11866908
*/
template <unsigned long long mod>
class basic_mod_int {
private:
    unsigned long long num;

public:
    static basic_mod_int pow(basic_mod_int x, unsigned long long y)
    {
        return y == 0 ? 1 : y % 2 == 0 ? pow(x * x, y / 2) : x * pow(x, y - 1);
    }
    basic_mod_int() = default;
    basic_mod_int(unsigned long long n)
        : num(n % mod)
    {
    }
    explicit operator unsigned long long() const
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
        return pow(*this, mod - 2);
    }
    basic_mod_int operator+(const basic_mod_int &mi) const
    {
        return num + mi.num;
    }
    basic_mod_int operator-(const basic_mod_int &mi) const
    {
        return num + mod - mi.num;
    }
    basic_mod_int operator*(const basic_mod_int &mi) const
    {
        return num * mi.num;
    }
    basic_mod_int operator/(const basic_mod_int &mi) const
    {
        return num * (~mi).num;
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
template <unsigned long long mod>
class combination_modulo final : basic_mod_int<mod> {
    using mod_int = basic_mod_int<mod>;

private:
    std::vector<mod_int> factorial_mod;

public:
    combination_modulo(unsigned long long max_n)
        : factorial_mod(max_n + 1)
    {
        factorial_mod[0] = 1;
        for (unsigned long long i = 1; i <= max_n; ++i) factorial_mod[i] = factorial_mod[i - 1] * i;
    }
    mod_int operator()(unsigned long long n, unsigned long long r)
    {
        return n < r ? 0 : factorial_mod[n] / (factorial_mod[r] * factorial_mod[n - r]);
    }
};
