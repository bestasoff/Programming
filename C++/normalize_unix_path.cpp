#include <iostream>
#include <stack>
#include <string>

std::string NormalizePath(std::string path) {
    std::stack<std::string> st;
    std::string dirs;
    std::string result;

    result.push_back('/');
    for (int i = 0; i < path.size(); ++i) {
        dirs.clear();
        while (path[i] == '/') {
            ++i;
        }
        while (i < path.size() && path[i] != '/') {
            dirs.push_back(path[i]);
            ++i;
        }
        if (dirs.compare("..") == 0) {
            if (!st.empty()) {
                st.pop();
            }
        } else if (dirs.compare(".") == 0) {
            continue;
        } else if (!dirs.empty()) {
            st.push(dirs);
        }
    }
    std::stack<std::string> st1;
    while (!st.empty()) {
        st1.push(st.top());
        st.pop();
    }
    while (!st1.empty()) {
        std::string temp = st1.top();
        if (st1.size() != 1) {
            result.append(temp + "/");
        } else {
            result.append(temp);
        }
        st1.pop();
    }
    return result;
}

int main() {
    std::cout << NormalizePath("/////documents/root/.././../etc");
    return 0;
}
