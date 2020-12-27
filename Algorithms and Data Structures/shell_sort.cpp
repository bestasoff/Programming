void ShellSort(std::vector<int>& a) {
    int d = n / 2;
    while (d != 0) {
        for (int k = 0; k < d; k++) {
            for (int i = k; i < n; i = i + d) {
                int j = i;
                while ((j != k) && (a[j] < a[j - d])) {
                    a[j] = a[j] ^ a[j - d];
                    a[j - d] = a[j] ^ a[j - d];
                    a[j] = a[j] ^ a[j - d];
                    j -= d;
                }
            }
        }
        d = d / 2;
    }
}
