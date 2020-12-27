void heapify(std::vector<int>& a, int x, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < x && a[left] > a[largest])
        largest = left;

    if (right < x && a[right] > a[largest])
        largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        heapify(x, largest);
    }
}

void heapSort(std::vector<int>& a, int x) {
    // Build the heap
    for (int i = x / 2 - 1; i >= 0; i--)
        heapify(a, x, i);

    for (int i = x - 1; i >= 0; i--) {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}
