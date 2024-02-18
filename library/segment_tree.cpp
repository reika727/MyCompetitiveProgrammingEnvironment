/*
https://atcoder.jp/contests/abc285/submissions/50432712
https://atcoder.jp/contests/abc340/submissions/50432774
https://atcoder.jp/contests/abc341/submissions/50432795
*/

template <typename T>
class segment_tree final : private std::vector<T> {
private:
    std::function<T(T, T)> operation;
    T identity;

public:
    segment_tree(const std::size_t n, const auto opr, const T id)
        : operation(opr), identity(id)
    {
        std::size_t _n = 1;
        while (_n < n) _n <<= 1;
        this->resize((_n << 1) - 1, identity);
    }
    void update(std::size_t idx, const T val)
    {
        idx += this->size() / 2;
        (*this)[idx] = val;
        while (idx) {
            idx = (idx - 1) / 2;
            (*this)[idx] = operation((*this)[idx * 2 + 1], (*this)[idx * 2 + 2]);
        }
    }
    T query(std::size_t l, std::size_t r) const
    {
        l += this->size() / 2;
        r += this->size() / 2;
        T L = identity, R = identity;
        while (l < r) {
            if (l % 2 == 0) L = operation(L, (*this)[l++]);
            if (r % 2 == 0) R = operation((*this)[--r], R);
            l >>= 1;
            r >>= 1;
        }
        return operation(L, R);
    }
    T at(const std::size_t idx) const
    {
        return (*this)[idx + this->size() / 2];
    }
    static segment_tree<T> sum(const std::size_t n)
    {
        return segment_tree<T>(n, std::plus<T>(), static_cast<T>(0));
    }
    static segment_tree<T> product(const std::size_t n)
    {
        return segment_tree<T>(n, std::multiplies<T>(), static_cast<T>(1));
    }
    static segment_tree<T> max(const std::size_t n, const T id = std::numeric_limits<T>::min())
    {
        return segment_tree<T>(n, static_cast<const T &(*)(const T &, const T &)>(std::max), id);
    }
    static segment_tree<T> min(const std::size_t n, const T id = std::numeric_limits<T>::max())
    {
        return segment_tree<T>(n, static_cast<const T &(*)(const T &, const T &)>(std::min), id);
    }
    static segment_tree<T> all(const std::size_t n)
    {
        return segment_tree<T>(n, std::logical_and(), true);
    }
    static segment_tree<T> any(const std::size_t n)
    {
        return segment_tree<T>(n, std::logical_or(), false);
    }
};
