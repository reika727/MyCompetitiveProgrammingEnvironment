/*
https://onlinejudge.u-aizu.ac.jp/status/users/s18ti016/submissions/1/DSL_2_B/judge/4052931/C++14
*/
template <typename T>
class bucket final : private std::vector<T> {
private:
    using typename std::vector<T>::size_type;
    using std::vector<T>::begin;
    std::function<T(T, T)> operation;
    T identity;
    size_type buc_cap;
    std::vector<T> buc;

public:
    using std::vector<T>::operator[];
    bucket(size_type n, size_type b_cap, std::function<T(T, T)> opr, T id)
        : std::vector<T>(n, id),
          operation(opr), identity(id),
          buc_cap(b_cap), buc((n + b_cap - 1) / b_cap, id)
    {
    }
    void update()
    {
        for (size_type b = 0; b < buc.size(); ++b) {
            auto itr = begin() + b * buc_cap;
            buc[b] = accumulate(itr, itr + buc_cap, identity, operation);
        }
    }
    void update(size_type idx, T val)
    {
        (*this)[idx] = val;
        size_type b = idx / buc_cap;
        auto itr = begin() + b * buc_cap;
        buc[b] = accumulate(itr, itr + buc_cap, identity, operation);
    }
    T query(size_type l, size_type r) const
    {
        T ret = identity;
        for (size_type b = 0; b < buc.size(); ++b) {
            if (size_type lt = std::max(l, b * buc_cap), rt = std::min(r, (b + 1) * buc_cap); lt < rt) {
                if (rt - lt == buc_cap)
                    ret = operation(ret, buc[b]);
                else
                    for (size_type i = lt; i < rt; ++i) ret = operation(ret, (*this)[i]);
            }
        }
        return ret;
    }
    static bucket<T> sum(size_type n, size_type buc_cap)
    {
        return bucket<T>(n, buc_cap, std::plus<T>(), static_cast<T>(0));
    }
    static bucket<T> product(size_type n, size_type buc_cap)
    {
        return bucket<T>(n, buc_cap, std::multiplies<T>(), static_cast<T>(1));
    }
    static bucket<T> max(size_type n, size_type buc_cap, T id = std::numeric_limits<T>::min())
    {
        return bucket<T>(n, buc_cap, static_cast<const T &(*)(const T &, const T &)>(std::max), id);
    }
    static bucket<T> min(size_type n, size_type buc_cap, T id = std::numeric_limits<T>::max())
    {
        return bucket<T>(n, buc_cap, static_cast<const T &(*)(const T &, const T &)>(std::min), id);
    }
};
