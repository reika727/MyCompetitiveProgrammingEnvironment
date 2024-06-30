/*
https://atcoder.jp/contests/abc338/submissions/55104140
https://yukicoder.me/submissions/992204
*/

template <std::default_initializable T, size_t Dimension>
class imos final {
    static_assert(Dimension != 0, "Dimension must not be 0.");
    friend imos<T, Dimension + 1>;

private:
    using lower_dimension_t = std::conditional_t<
        Dimension == 1,
        T,
        imos<T, Dimension - 1>
    >;

    std::vector<lower_dimension_t> lower_dimensions;

    std::size_t cast_index(const std::ptrdiff_t index) const noexcept
    {
        const std::ptrdiff_t elem_count = lower_dimensions.size() - 1;
        return (index % elem_count + elem_count) % elem_count;
    }

    std::ptrdiff_t floor_index(const std::ptrdiff_t index) const noexcept
    {
        const std::ptrdiff_t elem_count = lower_dimensions.size() - 1;
        return index >= 0 ? index / elem_count : (index + 1) / elem_count - 1;
    }

    imos<T, Dimension> &operator+=(const imos<T, Dimension> &im)
    {
        for (auto i = 0uz; i < lower_dimensions.size(); ++i) {
            lower_dimensions[i] += im.lower_dimensions[i];
        }
        return *this;
    }

public:
    imos(const std::size_t size, const auto... sizes)
        : lower_dimensions(size + 1, {sizes...})
    {
        static_assert(
            1 + sizeof...(sizes) == Dimension,
            "Number of sizes must be equal to Dimension."
        );
    }

    const T& at(const std::ptrdiff_t index, const auto... indices) const
    {
        static_assert(
            1 + sizeof...(indices) == Dimension,
            "Number of indices must be equal to Dimension."
        );
        if constexpr (sizeof...(indices) == 0) {
            return lower_dimensions[cast_index(index)];
        } else {
            return lower_dimensions[cast_index(index)].at(indices...);
        }
    }

    void set(
        const std::span<const std::ptrdiff_t, Dimension> begin,
        const std::span<const std::ptrdiff_t, Dimension> end,
        const T weight
    )
    {
        if (weight == 0) {
            return;
        }
        const auto cross_count = floor_index(end.front()) - floor_index(begin.front());
        if constexpr (Dimension > 1) {
            lower_dimensions[cast_index(begin.front())].set(
                begin.template subspan<1>(),
                end.template subspan<1>(),
                weight
            );
            lower_dimensions[cast_index(end.front())].set(
                begin.template subspan<1>(),
                end.template subspan<1>(),
                -weight
            );
            lower_dimensions.front().set(
                begin.template subspan<1>(),
                end.template subspan<1>(),
                weight * cross_count
            );
            lower_dimensions.back().set(
                begin.template subspan<1>(),
                end.template subspan<1>(),
                -weight * cross_count
            );
        } else {
            lower_dimensions[cast_index(begin.front())] += weight;
            lower_dimensions[cast_index(end.front())] -= weight;
            lower_dimensions.front() += weight * cross_count;
            lower_dimensions.back() -= weight * cross_count;
        }
    }

    void set(
        const std::array<const std::ptrdiff_t, Dimension> &begin,
        const std::array<const std::ptrdiff_t, Dimension> &end,
        const T weight
    )
    {
        set(std::span{begin}, std::span{end}, weight);
    }

    void set(const std::ptrdiff_t begin, const std::ptrdiff_t end, const T weight)
    {
        static_assert(
            Dimension == 1,
            "set(ptrdiff_t, ptrdiff_t, T) can be called only when Dimension == 1."
        );
        return set(std::array{begin}, std::array{end}, weight);
    }

    void accumulate()
    {
        for (auto i = 0uz; i < lower_dimensions.size() - 1; ++i) {
            lower_dimensions[i + 1] += lower_dimensions[i];
        }
        if constexpr (Dimension > 1) {
            std::ranges::for_each(lower_dimensions, &lower_dimension_t::accumulate);
        }
    }
};

template <class T>
auto make_imos(auto... sizes)
{
    return imos<T, sizeof...(sizes)>(sizes...);
}
