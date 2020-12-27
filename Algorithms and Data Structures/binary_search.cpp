#include <iostream>
#include <vector>

using namespace std;

int Binary_Search(const vector<int>& input, int l, int r, int x, int n) {
    if(r >= l)
    {
        int mid = (l + r) / 2;
        if( ( mid == 0 || x > input[mid-1]) && input[mid] == x)
            return mid;
        else if(x > input[mid])
            return Binary_Search(input, (mid + 1), r, x, n);
        else
            return Binary_Search(input, l, (mid -1), x, n);
    }
    return -1;
}

int Upper_Bound(const vector<int>& a, int x) {
    int l = 0;
    int h = static_cast<int>(a.size());
    while (l < h) {
        int mid = (l + h) / 2;
        if (x >= a[mid]) {
            l = mid + 1;
        } else {
            h = mid;
        }
    }
    return l;
}

int Lower_Bound(const vector<int>& a, int x) {
    int l = 0;
    int h = static_cast<int>(a.size());
    while (l < h) {
        int mid = (l + h) / 2;
        if (x <= a[mid]) {
            h = mid;
        } else {
            l = mid + 1;
        }
    }
    return l;
}
