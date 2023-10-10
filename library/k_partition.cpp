/*
https://atcoder.jp/contests/abc310/submissions/43675220
*/

class k_partition_generator final {
private:
    std::vector<std::size_t> indices;
    const std::size_t partition_size;
    std::vector<std::size_t> max_indices;
public:
    k_partition_generator(std::size_t n, std::size_t k)
        : indices(n), partition_size(k), max_indices(n + 1)
    {
        if (n < k) {
            throw std::runtime_error("n must be greater or equal to k");
        }
        if (n != 0 && k == 0) {
            throw std::runtime_error("k can be 0 only when n is 0");
        }
        std::iota(indices.begin() + (n - k), indices.end(), static_cast<std::size_t>(0));
        std::iota(max_indices.begin() + (n - k + 1), max_indices.end(), static_cast<std::size_t>(0));
    }
    bool next_partition()
    {
        if (indices.size() == 0) {
            return false;
        }
        for (std::size_t i = indices.size() - 1; i > 0; --i) {
            if (indices[i] <= max_indices[i] && indices[i] != partition_size - 1) {
                const std::size_t new_max = std::max(max_indices[i], ++indices[i]);
                for (std::size_t j = i + 1; j < indices.size(); ++j) {
                    if (indices[j] > new_max) {
                        break;
                    }
                    indices[j] = 0;
                }
                break;
            }
            indices[i] =
                partition_size - 1 >= indices.size() - 1 - i
                ? (partition_size - 1) - (indices.size() - 1 - i)
                : 0;
        }
        for (std::size_t i = 0; i < indices.size(); ++i) {
            max_indices[i + 1] = std::max(max_indices[i], indices[i]);
        }
        return max_indices[indices.size() - partition_size + 1] != 0;
    }
    const std::vector<std::size_t> &get_indices()
    {
        return indices;
    }
};
