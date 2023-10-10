/*
https://atcoder.jp/contests/abc087/submissions/38084502
*/

class weighted_union_find final {
private:
    std::vector<unsigned long long> par, sz, rank;
    std::vector<long long> diff_weight;
    unsigned long long cnt;

public:
    weighted_union_find(unsigned long long n)
        : par(n), sz(n, 1), rank(n, 0), diff_weight(n, 0), cnt(n)
    {
        std::iota(par.begin(), par.end(), 0ull);
    }
    unsigned long long find(unsigned long long n)
    {
        if (par[n] == n) {
            return n;
        } else {
            unsigned long long root_n = find(par[n]);
            diff_weight[n] += diff_weight[par[n]];
            return par[n] = root_n;
        }
    }
    void unite(unsigned long long x, unsigned long long y, long long w)
    {
        long long root_x = find(x), root_y = find(y);
        if (root_x != root_y) {
            if (rank[root_x] < rank[root_y]) {
                diff_weight[root_x] = -(w + weight(x) - weight(y));
                par[root_x] = root_y;
                sz[root_y] += sz[root_x];
            } else {
                diff_weight[root_y] = w + weight(x) - weight(y);
                par[root_y] = root_x;
                sz[root_x] += sz[root_y];
                if (rank[root_x] == rank[root_y]) ++rank[root_x];
            }
            --cnt;
        }
    }
    bool is_same(unsigned long long x, unsigned long long y)
    {
        return find(x) == find(y);
    }
    unsigned long long size(unsigned long long n)
    {
        return sz[find(n)];
    }
    unsigned long long count() const
    {
        return cnt;
    }
    long long weight(unsigned long long n)
    {
        find(n);
        return diff_weight[n];
    }
    long long difference(unsigned long long x, unsigned long long y)
    {
        return weight(y) - weight(x);
    }
};
