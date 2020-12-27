uint64_t KOrderStatistics(std::vector<uint64_t>& array, uint64_t kk) {
    uint64_t rr = array.size() - 1;
    uint64_t ll = 0;
    while (ll >= 0) {
        if (rr <= ll + 1) {
            if (rr == ll + 1 && array[rr] < array[ll]) {
                std::swap(array[rr], array[ll]);
            }
            return array[kk];
        }

        uint64_t mid = (ll + rr) / 2;
        std::swap(array[mid], array[ll + 1]);
        if (array[ll] > array[rr]) {
            std::swap(array[rr], array[ll]);
        }
        if (array[ll + 1] > array[rr]) {
            std::swap(array[ll + 1], array[rr]);
        }
        if (array[ll] > array[ll + 1]) {
            std::swap(array[ll], array[ll + 1]);
        }

        uint64_t l_index = ll + 1;
        uint64_t r_index = rr;
        uint64_t current = array[ll + 1];
        while (l_index > 0) {
            while (array[--r_index] > current) {
            }
            while (array[++l_index] < current) {
            }

            if (l_index > r_index) {
                break;
            }

            std::swap(array[l_index], array[r_index]);
        }

        array[ll + 1] = array[r_index];
        array[r_index] = current;

        if (r_index >= kk) {
            rr = r_index - 1;
        }
        if (r_index <= kk) {
            ll = l_index;
        }
    }
    return array[kk];
}
