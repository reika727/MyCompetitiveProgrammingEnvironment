/*
https://atcoder.jp/contests/abc087/submissions/51001873
*/

template <std::unsigned_integral UInt>
class weighted_union_find final {
private:
    std::vector<UInt> par;
    std::vector<std::size_t> sz, rank;
    std::vector<std::ptrdiff_t> diff_weight;
    std::size_t cnt;

public:
    weighted_union_find(const std::size_t n)
        : par(n), sz(n, 1), rank(n, 0), diff_weight(n, 0), cnt(n)
    {
        std::iota(par.begin(), par.end(), static_cast<UInt>(0));
    }

    UInt find(const UInt n)
    {
        if (par[n] == n) {
            return n;
        } else {
            const auto root_n = find(par[n]);
            diff_weight[n] += diff_weight[par[n]];
            return par[n] = root_n;
        }
    }

    void unite(const UInt x, const UInt y, const std::ptrdiff_t w)
    {
        const auto root_x = find(x), root_y = find(y);
        if (root_x != root_y) {
            if (rank[root_x] < rank[root_y]) {
                diff_weight[root_x] = -(w + weight(x) - weight(y));
                par[root_x] = root_y;
                sz[root_y] += sz[root_x];
            } else {
                diff_weight[root_y] = w + weight(x) - weight(y);
                par[root_y] = root_x;
                sz[root_x] += sz[root_y];
                if (rank[root_x] == rank[root_y]) {
                    ++rank[root_x];
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

    std::ptrdiff_t weight(const UInt n)
    {
        find(n);
        return diff_weight[n];
    }

    std::ptrdiff_t difference(const UInt x, const UInt y)
    {
        return weight(y) - weight(x);
    }
};
