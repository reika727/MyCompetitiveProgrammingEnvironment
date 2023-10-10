/*
https://atcoder.jp/contests/atc001/submissions/38084424
*/

class union_find final {
private:
    std::vector<unsigned long long> par, sz, rank;
    unsigned long long cnt;

public:
    union_find(unsigned long long n)
        : par(n), sz(n, 1), rank(n, 0), cnt(n)
    {
        iota(par.begin(), par.end(), 0);
    }
    unsigned long long find(unsigned long long n)
    {
        return par[n] == n ? par[n] : par[n] = find(par[n]);
    }
    void unite(unsigned long long x, unsigned long long y)
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
                if (rank[x] == rank[y]) ++rank[x];
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
};
