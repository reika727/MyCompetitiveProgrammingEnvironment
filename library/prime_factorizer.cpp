/*
https://atcoder.jp/contests/abc342/submissions/50642672
*/

class prime_factorizer final
{
private:
    std::vector<bool> is_prime_offset2;
    std::vector<std::size_t> primes;

    void expand(const std::size_t n)
    {
        const auto previous_max = is_prime_offset2.size() + 1;
        if (n <= previous_max) {
            return;
        }
        is_prime_offset2.resize(n - 1, true);
        for (const auto prime : primes) {
            if (prime > n / prime) {
                break;
            }
            for (
                auto i = std::max(previous_max / prime + 1, prime);
                i <= n / prime;
                ++i
            ) {
                is_prime_offset2[i * prime - 2] = false;
            }
        }
        for (auto prime = previous_max + 1; prime <= n; ++prime) {
            if (is_prime_offset2[prime - 2]) {
                primes.push_back(prime);
                for (auto i = prime; i <= n / prime; ++i) {
                    is_prime_offset2[i * prime - 2] = false;
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

    bool is_prime(const std::size_t n)
    {
        expand(n);
        return n >= 2 && is_prime_offset2[n - 2];
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
            if (is_prime(n)) {
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
