/*
https://atcoder.jp/contests/abc338/submissions/49981374
https://yukicoder.me/submissions/947830
*/

template <std::default_initializable T, size_t Dimension>
class imos
    : protected std::conditional_t<
        Dimension == 1,
        std::vector<T>,
        std::vector<imos<T, Dimension - 1>>
      >
{
    static_assert(Dimension != 0, "Dimension must not be 0.");
    friend imos<T, Dimension + 1>;

private:
    using lower_dimension = std::conditional_t<
        Dimension == 1,
        T,
        imos<T, Dimension - 1>
    >;

    imos<T, Dimension> &operator+=(const imos<T, Dimension> &im)
    {
        for (auto i = 0uz; i < this->size(); ++i) {
            (*this)[i] += im[i];
        }
        return *this;
    }

public:
    imos(const std::size_t size, const auto... sizes)
        : std::vector<lower_dimension>(
              size + 1,
              lower_dimension(sizes...)
          )
    {
        static_assert(
            1 + sizeof...(sizes) == Dimension,
            "Number of sizes must be equal to Dimension."
        );
    }

    const T& at(const std::size_t index, const auto... indices) const
    {
        static_assert(
            1 + sizeof...(indices) == Dimension,
            "Number of indices must be equal to Dimension."
        );
        if constexpr (sizeof...(indices) == 0) {
            return (*this)[index];
        } else {
            return (*this)[index].at(indices...);
        }
    }

    void set(
        const std::span<const std::size_t, Dimension> begin,
        const std::span<const std::size_t, Dimension> end,
        const T weight
    )
    {
        const auto begin0 = std::min(begin.front(), this->size() - 1);
        const auto end0 = std::min(end.front(), this->size() - 1);
        if constexpr (Dimension > 1) {
            (*this)[begin0].set(
                begin.template last<Dimension - 1>(),
                end.template last<Dimension - 1>(),
                weight
            );
            (*this)[end0].set(
                begin.template last<Dimension - 1>(),
                end.template last<Dimension - 1>(),
                -weight
            );
        } else {
            (*this)[begin0] += weight;
            (*this)[end0] -= weight;
        }
    }

    void set(
        const std::array<std::size_t, Dimension> &begin,
        const std::array<std::size_t, Dimension> &end,
        const T weight
    )
    {
        set(std::span{begin}, std::span{end}, weight);
    }

    void set(std::size_t begin, std::size_t end, const T weight)
    {
        static_assert(
            Dimension == 1,
            "set(size_t, size_t, T) can be called only when Dimension == 1."
        );
        return set(std::array{begin}, std::array{end}, weight);
    }

    void accumulate()
    {
        for (auto i = 0uz; i < this->size() - 1; ++i) {
            (*this)[i + 1] += (*this)[i];
        }
        if constexpr (Dimension > 1) {
            for (auto &child : *this) {
                child.accumulate();
            }
        }
    }
};

template <class T, size_t Dimension>
class cyclic_imos final : public imos<T, Dimension> {
public:
    using imos<T, Dimension>::imos;

    void set_cyclic(
        const std::span<const std::size_t, Dimension> begin,
        const std::span<const std::size_t, Dimension> end,
        const T weight
    )
    {
        this->set(begin, end, weight);
        if constexpr (Dimension > 1) {
            for (unsigned i = 1; i < (1 << Dimension); ++i) {
                std::array<std::size_t, Dimension> correction_begin, correction_end;
                for (auto j = 0uz; j < Dimension; ++j) {
                    if (i & (1 << j)) {
                        if (begin[j] <= end[j]) {
                            goto CONTINUE;
                        }
                        correction_begin[j] = 0;
                        correction_end[j] = this->size() - 1;
                    } else {
                        correction_begin[j] = begin[j];
                        correction_end[j] = end[j];
                    }
                }
                this->set(correction_begin, correction_end, weight);
            CONTINUE:
                ;
            }
        } else {
            if (begin.front() > end.front()) {
                (*this).front() += weight;
                (*this).back() -= weight;
            }
        }
    }

    void set_cyclic(
        const std::array<std::size_t, Dimension> &begin,
        const std::array<std::size_t, Dimension> &end,
        const T weight
    )
    {
        set_cyclic(std::span{begin}, std::span{end}, weight);
    }

    void set_cyclic(std::size_t begin, std::size_t end, const T weight)
    {
        static_assert(
            Dimension == 1,
            "set_cyclic(size_t, size_t, T) can be called only when Dimension == 1."
        );
        return set_cyclic(std::array{begin}, std::array{end}, weight);
    }
};

template <class T>
auto make_imos(auto... sizes)
{
    return imos<T, sizeof...(sizes)>(sizes...);
}

template <class T>
auto make_cyclic_imos(auto... sizes)
{
    return cyclic_imos<T, sizeof...(sizes)>(sizes...);
}
