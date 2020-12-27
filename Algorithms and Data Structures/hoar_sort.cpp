void HoarS(std::vector<int>& a, int first, int last) {
    int op = a[(first + last) / 2];
    int f = first, l = last;
    do {
        while (a[f] < op)
            f++;
        while (a[l] > op)
            l--;
        if (f <= l) {
            if (f < l) {
                int temp = a[f];
                a[f] = a[l];
                a[l] = temp;
            }
            f++;
            l--;
        }
    } while (f <= l);
    if (f < last)
        HoarS(a, f, last);
    if (l > first)
        HoarS(a, first, l);
}
