/*
https://atcoder.jp/contests/abc291/submissions/39274543
*/

template<class ElemInputIterator, class RelInputIterator, class OutputIterator>
bool topological_sort(
    ElemInputIterator elem_begin, ElemInputIterator elem_end,
    RelInputIterator rel_begin, RelInputIterator rel_end,
    OutputIterator out)
{
    using T = typename ElemInputIterator::value_type;
    std::map<T, std::set<T>> dests_of, sources_of;
    for (auto itr = rel_begin; itr != rel_end; ++itr) {
        auto [s, d] = *itr;
        dests_of[s].insert(d);
        sources_of[d].insert(s);
    }
    std::set<T> sources;
    for (auto itr = elem_begin; itr != elem_end; ++itr) {
        if (sources_of[*itr].empty()) {
            sources.insert(*itr);
        }
    }
    bool is_unique = true;
    while (!sources.empty()) {
        is_unique &= (sources.size() == 1);
        auto source = *sources.begin();
        sources.erase(sources.begin());
        *out++ = source;
        while (!dests_of[source].empty()) {
            auto dest = *dests_of[source].begin();
            dests_of[source].erase(dests_of[source].begin());
            sources_of[dest].erase(sources_of[dest].find(source));
            if (sources_of[dest].empty()) {
                sources.insert(dest);
            }
        }
    }
    return is_unique;
}
