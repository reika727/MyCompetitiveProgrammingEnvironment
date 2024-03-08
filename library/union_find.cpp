/*
https://atcoder.jp/contests/atc001/submissions/51001813
*/

template <std::unsigned_integral UInt>
class union_find final {
private:
    std::vector<UInt> par;
    std::vector<std::size_t> sz, rank;
    std::size_t cnt;

public:
    union_find(const std::size_t n)
        : par(n), sz(n, 1), rank(n, 0), cnt(n)
    {
        std::iota(par.begin(), par.end(), static_cast<UInt>(0));
    }

    UInt find(const UInt n)
    {
        return par[n] == n ? par[n] : par[n] = find(par[n]);
    }

    void unite(UInt x, UInt y)
    {
        x = find(x);
        y = find(y);
        if (x != y) {
            if (rank[x] < rank[y]) {
                par[x] = y;
                sz[y] += sz[x];
            } else {
                par[y] = x;
                sz[x] += sz[y];
                if (rank[x] == rank[y]) {
                    ++rank[x];
                }
            }
            --cnt;
        }
    }

    bool is_same(const UInt x, const UInt y)
    {
        return find(x) == find(y);
    }

    std::size_t size(const UInt n)
    {
        return sz[find(n)];
    }

    std::size_t count() const noexcept
    {
        return cnt;
    }
};
