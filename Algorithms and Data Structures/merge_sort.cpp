template<class RandomAccessIterator, class Compare>
void Merge(RandomAccessIterator first, RandomAccessIterator middle,
           RandomAccessIterator last, Compare comp) {
    std::vector<typename RandomAccessIterator::value_type> sorted;
    sorted.reserve(std::distance(first, last));

    auto left = first;
    auto right = middle;
    while (left != middle && right != last) {
        if (comp(*right, *left)) {
            sorted.emplace_back(*right++);
        } else {
            sorted.emplace_back(*left++);
        }
    }
    sorted.insert(sorted.end(), left, middle);
    sorted.insert(sorted.end(), right, last);

    std::move(sorted.begin(), sorted.end(), first);
}

template<class RandomAccessIterator, class Compare>
void MergeSort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    auto distance = std::distance(first, last);
    if (distance < 2) {
        return;
    }
    RandomAccessIterator middle = std::next(first, distance / 2);

    MergeSort(first, middle, comp);
    MergeSort(middle, last, comp);

    Merge(first, middle, last, comp);
}
