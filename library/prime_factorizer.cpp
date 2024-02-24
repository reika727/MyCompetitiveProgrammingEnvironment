/*
https://atcoder.jp/contests/abc342/submissions/50620656
*/

class prime_factorizer final
{
private:
    std::vector<bool> is_prime{false, false, true};
    std::vector<std::size_t> primes{2};

    void expand(const std::size_t n)
    {
        assert(n != std::numeric_limits<std::size_t>::max());
        if (is_prime.size() > n) {
            return;
        }
        const auto previous_max = is_prime.size() - 1;
        is_prime.resize(n + 1, true);
        for (const auto prime : primes) {
            if (prime > n / prime) {
                break;
            }
            for (
                auto i = std::max(previous_max / prime + 1, prime);
                i <= n / prime;
                ++i
            ) {
                is_prime[i * prime] = false;
            }
        }
        for (auto prime = primes.back() + 1; prime <= n; ++prime) {
            if (is_prime[prime]) {
                primes.push_back(prime);
                for (auto i = prime; i <= n / prime; ++i) {
                    is_prime[i * prime] = false;
                }
            }
        }
    }

public:
    prime_factorizer() = default;

    prime_factorizer(const std::size_t n)
    {
        expand(n);
    }

    std::span<const std::size_t> get_primes(const std::size_t n = 0)
    {
        expand(n);
        return primes;
    }

    bool check(const std::size_t n)
    {
        expand(n);
        return is_prime[n];
    }

    struct factor final {
        const std::size_t prime, exponent;
    };

    std::vector<factor> factorize(std::size_t n)
    {
        std::vector<factor> factors;
        for (const auto prime : get_primes(n)) {
            if (n <= 1) {
                break;
            }
            if (is_prime[n]) {
                factors.emplace_back(n, 1);
                break;
            }
            if (n % prime == 0) {
                std::size_t exponent = 0;
                for (; n % prime == 0; n /= prime) {
                    ++exponent;
                }
                factors.emplace_back(prime, exponent);
            }
        }
        return factors;
    }
};
