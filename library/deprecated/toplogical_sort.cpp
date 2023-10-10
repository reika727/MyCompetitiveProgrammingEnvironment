/*
https://atcoder.jp/contests/abc223/submissions/34859751
https://atcoder.jp/contests/abc216/submissions/34859767
*/

template<
    class Integer,
    class OutputIterator,
    std::enable_if_t<std::is_integral_v<Integer>, bool> = bool()
>
bool smallest_topological_sort(const std::vector<std::vector<Integer>> &g, OutputIterator oi)
{
    std::vector<Integer> indegree(g.size());
    int remaining_edges_num = 0;
    for (const auto &dests : g) {
        for (Integer dest : dests) {
            ++indegree[dest];
            ++remaining_edges_num;
        }
    }
    std::priority_queue<Integer, std::vector<Integer>, std::greater<Integer>> sources;
    for (std::size_t i = 0; i < indegree.size(); ++i) {
        if (indegree[i] == 0) {
            sources.push(i);
        }
    }
    while (!sources.empty()) {
        Integer source = sources.top(); sources.pop();
        for (Integer dest : g[source]) {
            if (--indegree[dest] == 0) {
                sources.push(dest);
            }
            --remaining_edges_num;
        }
        *oi++ = source;
    }
    return remaining_edges_num == 0;
}

struct null_output_iterator : std::iterator<std::output_iterator_tag, null_output_iterator> {
    template<class T>
    null_output_iterator &operator=(const T&)
    {
        return *this;
    }

    null_output_iterator operator++(int)
    {
        return *this;
    }

    null_output_iterator &operator*()
    {
        return *this;
    }
};

template<
    class Integer,
    std::enable_if_t<std::is_integral_v<Integer>, bool> = bool()
>
bool is_topological_sortable(std::vector<std::vector<Integer>> &g)
{
    return smallest_topological_sort(g, null_output_iterator());
}
