/*
https://onlinejudge.u-aizu.ac.jp/status/users/s18ti016/submissions/1/DSL_2_B/judge/8980463/C++17
*/

template <class T>
class bucket final {
private:
    std::vector<T> data;
    std::function<T(T, T)> operation;
    T identity;
    std::size_t buc_cap;
    std::vector<T> buc;

public:
    bucket(const std::size_t n, const std::size_t b_cap, const std::function<T(T, T)> opr, T id)
        : data(n, id),
          operation(opr), identity(id),
          buc_cap(b_cap), buc((n + b_cap - 1) / b_cap, id)
    {
    }

    void update()
    {
        for (std::size_t b = 0; b < buc.size(); ++b) {
            const auto itr = data.begin() + b * buc_cap;
            buc[b] = std::accumulate(itr, itr + buc_cap, identity, operation);
        }
    }

    void update(const std::size_t idx, T val)
    {
        data[idx] = val;
        const auto b = idx / buc_cap;
        const auto itr = data.begin() + b * buc_cap;
        buc[b] = std::accumulate(itr, itr + buc_cap, identity, operation);
    }

    T query(const std::size_t l, const std::size_t r) const
    {
        T ret = identity;
        for (std::size_t b = 0; b < buc.size(); ++b) {
            if (const auto lt = std::max(l, b * buc_cap), rt = std::min(r, (b + 1) * buc_cap); lt < rt) {
                if (rt - lt == buc_cap) {
                    ret = operation(ret, buc[b]);
                } else {
                    for (auto i = lt; i < rt; ++i) {
                        ret = operation(ret, data[i]);
                    }
                }
            }
        }
        return ret;
    }

    T at(const std::size_t idx) const
    {
        return data[idx];
    }

    static bucket<T> sum(const std::size_t n, const std::size_t buc_cap, const T id = 0)
    {
        return {n, buc_cap, std::plus<T>(), id};
    }

    static bucket<T> product(const std::size_t n, const std::size_t buc_cap, const T id = 1)
    {
        return {n, buc_cap, std::multiplies<T>(), id};
    }

    static bucket<T> max(const std::size_t n, const std::size_t buc_cap, const T id = std::numeric_limits<T>::min())
    {
        return {n, buc_cap, static_cast<const T &(*)(const T &, const T &)>(std::max), id};
    }

    static bucket<T> min(const std::size_t n, const std::size_t buc_cap, const T id = std::numeric_limits<T>::max())
    {
        return {n, buc_cap, static_cast<const T &(*)(const T &, const T &)>(std::min), id};
    }
};
