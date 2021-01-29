#include <iostream>
#include <vector>

using namespace std;

bool BinarySearch(std::vector<int>& a, int begin, int end, int val) {
    while (begin <= end) {
        int middle = (begin + end) / 2;
        if (a[middle] == val) {
            return true;
        } else if (a[middle] < val) {
            begin = middle + 1;
        } else {
            end = middle - 1;
        }
    }
    return false;
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
