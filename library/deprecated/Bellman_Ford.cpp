/*
https://atcoder.jp/contests/abc061/submissions/13189495
*/

class Bellman_Ford final {
private:
    using typename std::vector<long long>::size_type;
    static inline constexpr auto inf = std::numeric_limits<long long>::max();
    static inline constexpr auto minf = std::numeric_limits<long long>::min();
    std::vector<std::tuple<size_type, size_type, long long>> edge;
    std::vector<long long> dist;

public:
    Bellman_Ford(size_type node_cnt)
        : dist(node_cnt)
    {
    }
    void add_edge(size_type from, size_type to, long long cost)
    {
        edge.emplace_back(from, to, cost);
    }
    void update(size_type start = 0)
    {
        std::fill(dist.begin(), dist.end(), inf);
        dist[start] = 0;
        bool is_updated = false;
        for (size_type i = 0; i < dist.size(); ++i) {
            for (auto [from, to, cost] : edge) {
                if (dist[from] != inf && dist[to] > dist[from] + cost) {
                    dist[to] = dist[from] + cost;
                    is_updated = true;
                }
            }
            if (!is_updated) break;
        }
        if (is_updated) {
            for (auto [from, to, cost] : edge) {
                if (dist[from] == minf || (dist[from] != inf && dist[to] > dist[from] + cost)) {
                    dist[to] = minf;
                }
            }
        }
    }
    long long get_dist(size_type node) const
    {
        return dist[node];
    }
    bool is_unreachable(size_type node) const
    {
        return dist[node] == inf;
    }
    bool is_in_negative_loop(size_type node) const
    {
        return dist[node] == minf;
    }
};
