/*
https://atcoder.jp/contests/practice2/submissions/38095389
*/

template <class T>
class Fenwick_tree final : private std::vector<T> {
private:
    std::function<T(T, T)> operation;
    T identity;

public:
    Fenwick_tree(std::size_t n, std::function<T(T, T)> opr, T id)
        : std::vector<T>(n + 1, id), operation(opr), identity(id)
    {
    }
    void operate(std::size_t idx, T val)
    {
        for (std::size_t i = idx + 1; i < this->size(); i += (i & -i)) {
            (*this)[i] = operation((*this)[i], val);
        }
    }
    T accumulate(std::size_t idx) const
    {
        T result = identity;
        for (std::size_t i = idx; i != 0; i -= (i & -i)) {
            result = operation(result, (*this)[i]);
        }
        return result;
    }
    static Fenwick_tree<T> sum(std::size_t n)
    {
        return Fenwick_tree<T>(n, std::plus<T>(), static_cast<T>(0));
    }
};
