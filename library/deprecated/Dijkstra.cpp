/*
https://atcoder.jp/contests/abc035/submissions/13190503
*/
class Dijkstra final {
private:
    using typename std::vector<unsigned long long>::size_type;
    static inline constexpr auto inf = std::numeric_limits<unsigned long long>::max();
    std::vector<std::vector<std::pair<unsigned long long, size_type>>> edge;
    std::vector<unsigned long long> dist;

public:
    Dijkstra(size_type node_cnt)
        : edge(node_cnt), dist(node_cnt)
    {
    }
    void add_edge(size_type from, size_type to, unsigned long long cost)
    {
        edge[from].emplace_back(cost, to);
    }
    void update(size_type start = 0)
    {
        std::fill(dist.begin(), dist.end(), inf);
        std::priority_queue<
            std::pair<unsigned long long, size_type>,
            std::vector<std::pair<unsigned long long, size_type>>,
            std::greater<std::pair<unsigned long long, size_type>>>
            node_queue;
        node_queue.emplace(0, start);
        while (!node_queue.empty()) {
            auto [tpe_cost, tpe_to] = node_queue.top();
            node_queue.pop();
            if (dist[tpe_to] > tpe_cost) {
                dist[tpe_to] = tpe_cost;
                for (auto [dist_cost, dist_index] : edge[tpe_to]) {
                    if (dist[dist_index] > tpe_cost + dist_cost) {
                        node_queue.emplace(tpe_cost + dist_cost, dist_index);
                    }
                }
            }
        }
    }
    unsigned long long get_dist(size_type node) const
    {
        return dist[node];
    }
    bool is_unreachable(size_type node) const
    {
        return dist[node] == inf;
    }
};
