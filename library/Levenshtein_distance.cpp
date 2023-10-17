/*
https://atcoder.jp/contests/pastbook2022/submissions/46681190
*/

template <
    std::ranges::input_range R1,
    std::ranges::input_range R2,
    class Pred = std::ranges::equal_to,
    class Proj1 = std::identity,
    class Proj2 = std::identity
>
requires std::indirectly_comparable<
    std::ranges::iterator_t<R1>,
    std::ranges::iterator_t<R2>,
    Pred,
    Proj1,
    Proj2
>
std::size_t Levenshtein_distance(
    R1 &&r1,
    R2 &&r2,
    Pred pred = {},
    Proj1 proj1 = {},
    Proj2 proj2 = {}
)
{
    const auto m = std::ranges::size(r1), n = std::ranges::size(r2);
    if (m > n) {
        return Levenshtein_distance(std::forward<R2>(r2), std::forward<R1>(r1), pred, proj2, proj1);
    }
    const auto distance = std::make_unique<std::size_t[]>(m + 1);
    std::iota(distance.get(), distance.get() + (m + 1), 0uz);
    for (auto j = 0uz; j < n; ++j) {
        auto prev = std::exchange(distance[0], j + 1);
        for (auto i = 0uz; i < m; ++i){
            prev = std::exchange(
                distance[i + 1],
                std::min({
                    distance[i + 1] + 1,
                    distance[i] + 1,
                    prev + (pred(proj1(r1[i]), proj2(r2[j])) ? 0 : 1)
                })
            );
        }
    }
    return distance[m];
}
