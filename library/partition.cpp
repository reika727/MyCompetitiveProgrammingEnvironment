/*
THIS LIBRARY HAS NOT BEEN TESTED!
*/

class partition_generator final {
private:
    std::vector<std::size_t> indices;
    std::vector<std::size_t> max_indices;

public:
    partition_generator(const std::size_t n)
        : indices(n, 0), max_indices(n + 1, 0)
    {
    }

    bool next_partition()
    {
        if (indices.size() == 0) {
            return false;
        }
        for (auto i = indices.size() - 1; i > 0; --i) {
            if (indices[i] <= max_indices[i]) {
                ++indices[i];
                break;
            }
            indices[i] = 0;
        }
        for (auto i = 0; i < indices.size(); ++i) {
            max_indices[i + 1] = std::max(max_indices[i], indices[i]);
        }
        return max_indices.back() != 0;
    }

    const std::span<const std::size_t> get_indices()
    {
        return indices;
    }
};
