/*
https://atcoder.jp/contests/abc338/submissions/49850308
*/

template <class T, size_t Dimension>
class cyclic_imos final : private std::vector<cyclic_imos<T, Dimension - 1>> {
    friend cyclic_imos<T, Dimension + 1>;

public:
    using coordinate = std::array<std::size_t, Dimension>;

    struct segment {
        coordinate begin, end;
    };

    constexpr inline static std::size_t dimension = Dimension;

    using std::vector<cyclic_imos<T, Dimension - 1>>::begin;
    using std::vector<cyclic_imos<T, Dimension - 1>>::end;
    using std::vector<cyclic_imos<T, Dimension - 1>>::operator[];

private:
    using std::vector<cyclic_imos<T, Dimension - 1>>::size;

    T& get(const std::span<const std::size_t, Dimension> idxs)
    {
        return (*this)[idxs.front()].get(idxs.template last<Dimension - 1>());
    }

    void set(
        const std::span<const std::size_t, Dimension> begin,
        const std::span<const std::size_t, Dimension> end,
        const T weight
    )
    {
        (*this)[begin.front()].set(
            begin.template last<Dimension - 1>(),
            end.template last<Dimension - 1>(),
            weight
        );
        (*this)[end.front()].set(
            begin.template last<Dimension - 1>(),
            end.template last<Dimension - 1>(),
            -weight
        );
    }

    cyclic_imos<T, Dimension> &operator+=(const cyclic_imos<T, Dimension> &ci)
    {
        for (std::size_t i = 0; i < size(); ++i) {
            (*this)[i] += ci[i];
        }
        return *this;
    }

public:
    template <class... Sizes>
    cyclic_imos(const std::size_t size, const Sizes... sizes)
        : std::vector<cyclic_imos<T, Dimension - 1>>(
              size + 1,
              cyclic_imos<T, sizeof...(Sizes)>(sizes...)
          )
    {
    }

    T& get(const coordinate &c)
    {
        return get(std::span{c});
    }

    void set(const segment &s, const T weight)
    {
        set(std::span{s.begin}, std::span{s.end}, weight);
    }

    void set_cyclic(const segment &s, const T weight)
    {
        set(s, weight);
        for (unsigned i = 1; i < (1 << Dimension); ++i) {
            segment correction;
            for (unsigned j = 0; j < Dimension; ++j) {
                if (i & (1 << j)) {
                    if (s.begin[j] <= s.end[j]) {
                        goto CONTINUE;
                    }
                    correction.begin[j] = 0;
                    correction.end[j] = size() - 1;
                } else {
                    correction.begin[j] = s.begin[j];
                    correction.end[j] = s.end[j];
                }
            }
            set(correction, weight);
        CONTINUE:
            ;
        }
    }

    void accumulate()
    {
        for (std::size_t i = 0; i < size() - 1; ++i) {
            (*this)[i + 1] += (*this)[i];
        }
        for (auto &child : *this) {
            child.accumulate();
        }
    }
};

template <class T>
class cyclic_imos<T, 1> final : private std::vector<T> {
    friend cyclic_imos<T, 2>;

public:
    using coordinate = std::array<std::size_t, 1>;

    struct segment {
        coordinate begin, end;
    };

    constexpr inline static std::size_t dimension = 1;

    using std::vector<T>::begin;
    using std::vector<T>::end;
    using std::vector<T>::operator[];

private:
    using std::vector<T>::size;
    T& get(const std::span<const std::size_t, 1> idxs)
    {
        return (*this)[idxs.front()];
    }

    void set(
        const std::span<const std::size_t, 1> begin,
        const std::span<const std::size_t, 1> end,
        const T weight
    )
    {
        (*this)[begin.front()] += weight;
        (*this)[end.front()] -= weight;
    }

    cyclic_imos<T, 1> &operator+=(const cyclic_imos<T, 1> &ci)
    {
        for (std::size_t i = 0; i < size(); ++i) {
            (*this)[i] += ci[i];
        }
        return *this;
    }

public:
    cyclic_imos(const std::size_t size)
        : std::vector<T>(size + 1)
    {
    }

    T& get(const coordinate &c)
    {
        return (*this)[c.front()];
    }

    T& get(const std::size_t idx)
    {
        return (*this)[idx];
    }

    void set(const segment &s, const T weight)
    {
        set(std::span{s.begin}, std::span{s.end}, weight);
    }

    void set(const size_t begin, const size_t end, const T weight)
    {
        set({.begin = begin, .end = end}, weight);
    }

    void set_cyclic(const segment &s, const T weight)
    {
        set(s, weight);
        if (s.begin.front() > s.end.front()) {
            (*this).front() += weight;
            (*this).back() -= weight;
        }
    }

    void set_cyclic(const std::size_t begin, const std::size_t end, const T weight)
    {
        set_cyclic({.begin = begin, .end = end}, weight);
    }

    void accumulate()
    {
        for (std::size_t i = 0; i < size() - 1; ++i) {
            (*this)[i + 1] += (*this)[i];
        }
    }
};

template <class T>
auto make_cyclic_imos(auto... sizes)
{
    return cyclic_imos<T, sizeof...(sizes)>(sizes...);
}
