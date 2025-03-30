#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// 生成反向互补序列
string reverse_complement(const string &s) {
    unordered_map<char, char> complement = {{'A', 'T'}, {'T', 'A'}, {'C', 'G'}, {'G', 'C'}};
    string rc;
    for (auto it = s.rbegin(); it != s.rend(); ++ it) {
        rc += complement[*it];
    }
    return rc;
}

// 预处理参考序列，记录所有子串的位置
unordered_map<string, vector<int>> build_substr_dict(const string &reference) {
    unordered_map<string, vector<int>> substr_dict;
    int n = reference.size();
    for (int i = 0; i < n; ++ i) {
        for (int j = i + 1; j <= n; ++ j) {
            string s = reference.substr(i, j - i);
            substr_dict[s].push_back(i);
        }
    }
    return substr_dict;
}

void find_repeats(const string &query, const string &reference) {
    auto forward_dict = build_substr_dict(reference);
    int n = query.size();
    vector<bool> covered(n, false);

    // 从长到短遍历子串长度
    for (int L = n; L >= 1; --L) {
        int i = 0;
        while (i <= n - L) {
            if (covered[i]) {
                ++ i;
                continue;
            }

            string s = query.substr(i, L);
            int j = i + L;
            int repeats = 1;

            // 检测连续重复次数
            while (j <= n - L && query.substr(j, L) == s && !covered[j]) {
                ++ repeats;
                j += L;
            }

            if (repeats >= 2) {
                int extra = repeats - 1;
                bool found = false;

                // 正向重复检查
                if (forward_dict.count(s) && forward_dict[s].size() == 1) {
                    int pos = forward_dict[s][0] + L;
                    cout << "POS: " << pos << ", Repeat size: " << L << ", Repeat count: " << repeats << ", Inverse: NO" << endl;
                    found = true;
                }else {
                    // 反向重复检查
                    string s_rev = reverse_complement(s);
                    if (forward_dict.count(s_rev) && forward_dict[s_rev].size() == 1) {
                        int pos = forward_dict[s_rev][0] + L;
                        cout << "POS: " << pos << ", Repeat size: " << L << ", Repeat count: " << repeats << ", Inverse: YES" << endl;
                        found = true;
                    }
                }

                // 标记覆盖区域
                if (found) {
                    for (int k = i; k < j; ++ k) covered[k] = true;
                    i = j;
                } else {
                    ++ i;
                }
            } else {
                ++ i;
            }
        }
    }
}

int main() {
    string reference = "CTGCAACGTTCGTGGTTCATGTTTGAGCGATAGGCCGAAACTAACCGTGCATGCAACGTTAGTGGATCATTGTGGAACTATAGACTCAAACTAAGCGAGCTTGCAACGTTAGTGGACCCTTTTTGAGCTATAGACGAAAACGGACCGAGGCTGCAAGGTTAGTGGATCATTTTTCAGTTTTAGACACAAACAAACCGAGCCATCAACGTTAGTCGATCATTTTTGTGCTATTGACCATATCTCAGCGAGCCTGCAACGTGAGTGGATCATTCTTGAGCTCTGGACCAAATCTAACCGTGCCAGCAACGCTAGTGGATAATTTTGTTGCTATAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTTACCATCGGACCTCCACGAATCTGAAAAGTTTTAATTTCCGAGCGATACTTACGACCGGACCTCCACGAATCAGAAAGGGTTCACTATCCGCTCGATACATACGATCGGACCTCCACGACTCTGTAAGGTTTCAAAATCCGCACGATAGTTACGACCGTACCTCTACGAATCTATAAGGTTTCAATTTCCGCTGGATCCTTACGATCGGACCTCCTCGAATCTGCAAGGTTTCAATATCCGCTCAATGGTTACGGACGGACCTCCACGCATCTTAAAGGTTAAAATAGGCGCTCGGTACTTACGATCGGACCTCTCCGAATCTCAAAGGTTTCAATATCCGCTTGATACTTACGATCGCAACACCACGGATCTGAAAGGTTTCAATATCCACTCTATA"; // 示例参考序列
    string query = "CTGCAACGTTCGTGGTTCATGTTTGAGCGATAGGCCGAAACTAACCGTGCATGCAACGTTAGTGGATCATTGTGGAACTATAGACTCAAACTAAGCGAGCTTGCAACGTTAGTGGACCCTTTTTGAGCTATAGACGAAAACGGACCGAGGCTGCAAGGTTAGTGGATCATTTTTCAGTTTTAGACACAAACAAACCGAGCCATCAACGTTAGTCGATCATTTTTGTGCTATTGACCATATCTCAGCGAGCCTGCAACGTGAGTGGATCATTCTTGAGCTCTGGACCAAATCTAACCGTGCCAGCAACGCTAGTGGATAATTTTGTTGCTATAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCCTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCTAGACCAACACTAATCGAGACTGCCTCGTTAGTGCATCATTTTTGCGCCATAGACCATAGCTAAGCGAGCGCTCGCTTAGCTATGGTCTATGGCGCAAAAATGATGCACTAACGAGGCAGTCTCGATTAGTGTTGGTCTATAGCAACAAAATTATCCACTAGCGTTGCTGGCTCGCTTAGCTATGGTCTATGGCGCAAAAATGATGCACTAACGAGGCAGTCTCGATTAGTGTTGGTCTATAGCAACAAAATTATCCACTAGCGTTGCTGCTTACCATCGGACCTCCACGAATCTGAAAAGTTTTAATTTCCGAGCGATACTTACGACCGGACCTCCACGAATCAGAAAGGGTTCACTATCCGCTCGATACATACGATCGGACCTCCACGACTCTGTAAGGTTTCAAAATCCGCACGATAGTTACGACCGTACCTCTACGAATCTATAAGGTTTCAATTTCCGCTGGATCCTTACGATCGGACCTCCTCGAATCTGCAAGGTTTCAATATCCGCTCAATGGTTACGGACGGACCTCCACGCATCTTAAAGGTTAAAATAGGCGCTCGGTACTTACGATCGGACCTCTCCGAATCTCAAAGGTTTCAATATCCGCTTGATACTTACGATCGCAACACCACGGATCTGAAAGGTTTCAATATCCACTCTATA"; // 示例查询序列
    find_repeats(query, reference);
    return 0;
}