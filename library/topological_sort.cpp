/*
https://atcoder.jp/contests/abc291/submissions/51001333
https://atcoder.jp/contests/abc223/submissions/51001574
https://atcoder.jp/contests/abc216/submissions/51001640
*/

template <
    std::ranges::input_range Elements,
    std::ranges::input_range Relations,
    std::weakly_incrementable Out
>
    requires std::indirectly_writable<Out, std::ranges::range_value_t<Elements>>
std::optional<bool> topological_sort(Elements&& elements, Relations&& relations, Out out)
{
    using T = std::ranges::range_value_t<Elements>;
    std::map<T, std::set<T>> dests_of, sources_of;
    for (const auto& [s, d] : relations) {
        dests_of[s].insert(d);
        sources_of[d].insert(s);
    }
    std::set<T> sources;
    for (const auto& e : elements) {
        if (!sources_of.contains(e)) {
            sources.insert(e);
        }
    }
    bool is_unique = true;
    while (!sources.empty()) {
        is_unique &= (sources.size() == 1);
        const auto source = *sources.begin();
        sources.erase(sources.begin());
        *out++ = source;
        while (dests_of.contains(source)) {
            const auto dest = *dests_of[source].begin();
            dests_of[source].erase(dests_of[source].begin());
            if (dests_of[source].empty()) {
                dests_of.erase(source);
            }
            sources_of[dest].erase(sources_of[dest].find(source));
            if (sources_of[dest].empty()) {
                sources_of.erase(dest);
                sources.insert(dest);
            }
        }
    }
    if (sources_of.empty() && dests_of.empty()) {
        return is_unique;
    } else {
        return std::nullopt;
    }
}
