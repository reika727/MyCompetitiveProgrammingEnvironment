/*
https://atcoder.jp/contests/abc342/submissions/51443675
https://atcoder.jp/contests/abc284/submissions/51443696
https://atcoder.jp/contests/abc179/submissions/51443628
https://atcoder.jp/contests/abc368/submissions/57101810
*/

class prime_factorizer final {
private:
    std::vector<std::size_t> primes{2};

    class primality_logger final {
    private:
        std::vector<bool> log;

    public:
        void expand(const std::size_t n)
        {
            if (n > max()) {
                log.resize((n - 1) / 2, true);
            }
        }

        void exclude(const std::size_t n)
        {
            if (n > 2 && n % 2 != 0) {
                expand(n);
                log[n / 2 - 1] = false;
            }
        }

        std::size_t max()
        {
            return log.size() * 2 + 2;
        }

        bool check(const std::size_t n)
        {
            return n == 2 || (n > 2 && n % 2 != 0 && log[n / 2 - 1]);
        }
    } primalities;

    void sieve_of_Eratosthenes(const std::size_t n)
    {
        const auto previous_max = primalities.max();
        if (n <= previous_max) {
            return;
        }
        primalities.expand(n);
        for (const auto prime : primes | std::views::drop(1) /* HACK */) {
            if (prime > n / prime) {
                break;
            }
            for (const auto i : std::views::iota(std::max(previous_max / prime + 1, prime))) {
                if (i > n / prime) {
                    break;
                }
                primalities.exclude(i * prime);
            }
        }
        for (
            const auto prime :
            std::views::iota(previous_max + 1, n + 1) |
            std::views::filter([this](const auto i){return primalities.check(i);})
        ) {
            primes.push_back(prime);
            for (const auto i : std::views::iota(prime)) {
                if (i > n / prime) {
                    break;
                }
                primalities.exclude(i * prime);
            }
        }
    }

    // NOT TESTED YET
    /*
    template<std::weakly_incrementable Out>
        requires std::indirectly_writable<Out, std::size_t>
    void get_divisors(
        const std::map<std::size_t, std::size_t>::const_iterator begin,
        const std::map<std::size_t, std::size_t>::const_iterator end,
        Out out,
        std::size_t divisor = 1
    )
    {
        if (begin == end) {
            *out++ = divisor;
            return;
        }
        const auto [prime, exponent] = *begin;
        for (std::size_t i = 0; i <= exponent; ++i) {
            get_divisors(std::next(begin), end, out, divisor);
            divisor *= prime;
        }
    }
    */

public:
    prime_factorizer() = default;

    prime_factorizer(const std::size_t n)
    {
        sieve_of_Eratosthenes(n);
    }

    std::span<const std::size_t> get_known_primes()
    {
        return primes;
    }

    std::span<const std::size_t> get_primes(const std::size_t n)
    {
        sieve_of_Eratosthenes(n);
        return primes;
    }

    bool is_prime(const std::size_t n)
    {
        sieve_of_Eratosthenes(n);
        return primalities.check(n);
    }

    std::map<std::size_t, std::size_t> factorize(std::size_t n)
    {
        std::map<std::size_t, std::size_t> factors;
        for (const auto prime : get_primes(n)) {
            if (n <= 1) {
                break;
            }
            if (is_prime(n)) {
                factors.emplace(n, 1);
                break;
            }
            std::size_t exponent = 0;
            for (; n % prime == 0; n /= prime) {
                ++exponent;
            }
            if (exponent != 0) {
                factors.emplace(prime, exponent);
            }
        }
        return factors;
    }

    // NOT TESTED YET
    /*
    template<std::weakly_incrementable Out>
        requires std::indirectly_writable<Out, std::size_t>
    void get_divisors(const std::size_t n, Out out)
    {
        const auto factors = factorize(n);
        get_divisors(factors.begin(), factors.end(), out);
    }
    */

    std::size_t count_factors(const std::size_t n)
    {
        const auto factors = factorize(n);
        return std::accumulate(
            factors.begin(), factors.end(), 1uz,
            [](const auto acc, const auto &f) {
                return acc * (f.second + 1);
            }
        );
    }

    std::size_t count_prime_factors_with_multiplicity(const std::size_t n)
    {
        const auto factors = factorize(n);
        return std::accumulate(
            factors.begin(), factors.end(), 0uz,
            [](const auto acc, const auto &f) {
                return acc + f.second;
            }
        );
    }
};
