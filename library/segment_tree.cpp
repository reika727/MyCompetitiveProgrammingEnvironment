/*
https://atcoder.jp/contests/abc285/submissions/50960173
https://atcoder.jp/contests/abc340/submissions/50960174
https://atcoder.jp/contests/abc341/submissions/50960177
*/

template <class T>
class segment_tree final {
private:
    std::vector<T> data;
    std::function<T(T, T)> operation;
    T identity;

public:
    segment_tree(const std::size_t n, const auto opr, const T id)
        : data((std::bit_ceil(n) << 1) - 1, id),
          operation(opr),
          identity(id)
    {
    }

    void update(std::size_t idx, const T val)
    {
        idx += data.size() >> 1;
        data[idx] = val;
        while (idx) {
            idx = (idx - 1) >> 1;
            data[idx] = operation(data[idx << 1 | 1], data[(idx << 1) + 2]);
        }
    }

    T query(std::size_t l, std::size_t r) const
    {
        l += data.size() >> 1;
        r += data.size() >> 1;
        auto L = identity, R = identity;
        while (l < r) {
            if (!(l & 1)) {
                L = operation(L, data[l++]);
            }
            if (!(r & 1)) {
                R = operation(data[--r], R);
            }
            l >>= 1;
            r >>= 1;
        }
        return operation(L, R);
    }

    T at(const std::size_t idx) const
    {
        return data[idx + (data.size() >> 1)];
    }

    static segment_tree<T> sum(const std::size_t n, const T id = 0)
    {
        return {n, std::plus<T>(), id};
    }

    static segment_tree<T> product(const std::size_t n, const T id = 1)
    {
        return {n, std::multiplies<T>(), id};
    }

    static segment_tree<T> max(const std::size_t n, const T id = std::numeric_limits<T>::min())
    {
        return {n, static_cast<const T &(*)(const T &, const T &)>(std::max), id};
    }

    static segment_tree<T> min(const std::size_t n, const T id = std::numeric_limits<T>::max())
    {
        return {n, static_cast<const T &(*)(const T &, const T &)>(std::min), id};
    }

    static segment_tree<T> all(const std::size_t n, const T id = true)
    {
        return {n, std::logical_and(), id};
    }

    static segment_tree<T> any(const std::size_t n, const T id = false)
    {
        return {n, std::logical_or(), id};
    }
};
