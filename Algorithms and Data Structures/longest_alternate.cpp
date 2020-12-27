#include "vector"
#include <iostream>

using std::vector;

vector<int> LongestAlternateSequence(const vector<int> &input) {
    if (input.size() <= 1)
        return input;

    const size_t LOWING = 0, UPPING = 1;
    vector<vector<size_t>> subsequence_lengths = {
            vector<size_t>(input.size(), 1),
            vector<size_t>(input.size(), 1)};
    vector<vector<size_t>> next_element = {
            vector<size_t>(input.size(), 0),
            vector<size_t>(input.size(), 0)};
    vector<size_t> max_length = {1, 1};

    for (int i = static_cast<int>(input.size()) - 1; i >= 0; i -= 1) {
        for (size_t kind_of_sequence = LOWING; kind_of_sequence <= UPPING; kind_of_sequence += 1) {
            for (size_t j = i + 1; j < input.size(); j += 1) {
                if ((kind_of_sequence == LOWING) ? (input[i] > input[j])
                                                 : (input[i] < input[j])) {
                    if (subsequence_lengths[kind_of_sequence][i] <=
                        subsequence_lengths[!kind_of_sequence][j]) {

                        subsequence_lengths[kind_of_sequence][i] =
                        subsequence_lengths[!kind_of_sequence][j] + 1;
                        next_element[kind_of_sequence][i] = j;

                        if (subsequence_lengths[kind_of_sequence][i] >
                            max_length[kind_of_sequence]) {

                            max_length[kind_of_sequence] =
                            subsequence_lengths[kind_of_sequence][i];
                        }
                        if (subsequence_lengths[kind_of_sequence][i] ==
                            max_length[!kind_of_sequence] + 1) {
                            break;
                        }
                    }
                }
            }
        }
    }

    size_t kind_of_sequence;
    if (subsequence_lengths[LOWING][0] >
        subsequence_lengths[UPPING][0]) {
        kind_of_sequence = LOWING;
    } else {
        if (subsequence_lengths[UPPING][0] >
            subsequence_lengths[LOWING][0]) {
            kind_of_sequence = UPPING;
        } else {
            if ((next_element[LOWING][0] <
                 next_element[UPPING][0])) {
                kind_of_sequence = LOWING;
            } else {
                kind_of_sequence = UPPING;
            }
        }
    }
    size_t item = 0;
    vector<int> result;
    do {
        result.push_back(input[item]);
        item = next_element[kind_of_sequence][item];
        kind_of_sequence = !kind_of_sequence;
    } while (item != 0);
    return result;
}
