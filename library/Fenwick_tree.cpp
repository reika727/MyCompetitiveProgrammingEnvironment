/*
https://atcoder.jp/contests/practice2/submissions/50952454
*/

template <class T>
class Fenwick_tree final {
private:
    std::vector<T> data;
    std::function<T(T, T)> operation;
    T identity;

public:
    Fenwick_tree(
        const std::size_t n,
        const std::function<T(T, T)> opr,
        const T id
    )
        : data(n + 1, id), operation(opr), identity(id)
    {
    }

    void set(const std::size_t idx, const T val)
    {
        for (auto i = idx + 1; i < data.size(); i += i & -i) {
            data[i] = operation(data[i], val);
        }
    }

    T fold_prefix(const std::size_t end) const
    {
        auto result = identity;
        for (auto i = end; i; i &= i - 1) {
            result = operation(result, data[i]);
        }
        return result;
    }

    T fold(const std::size_t begin, const std::size_t end) const
    {
        return fold_prefix(end) - fold_prefix(begin);
    }

    static Fenwick_tree<T> sum(
        const std::size_t n,
        const T id = static_cast<T>(0)
    )
    {
        return Fenwick_tree(n, std::plus<T>(), id);
    }
};
