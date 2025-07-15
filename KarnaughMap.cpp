#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <set>
#include <algorithm>
#include <climits>
using namespace std;

int num_vars;
char var_names[] = {'A', 'B', 'C', 'D', 'E', 'F'};

// تبدیل عدد به باینری با طول مشخص
string to_binary(int num, int bits) {
    return bitset<6>(num).to_string().substr(6 - bits);
}

// شمارش تعداد بیت‌های 1 در رشته باینری
int count_ones(const string& s) {
    return count(s.begin(), s.end(), '1');
}

// بررسی تفاوت در فقط یک بیت بین دو رشته
bool one_bit_diff(const string& a, const string& b, string& res) {
    int diff = 0;
    res = a;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            diff++;
            res[i] = '-';
        }
        if (diff > 1) return false;
    }
    return diff == 1;
}

// بررسی اینکه آیا یک ایمپلیکنت یک مینیم‌ ترم را پوشش می‌دهد
bool covers(const string& implicant, const string& m) {
    for (int i = 0; i < m.size(); i++)
        if (implicant[i] != '-' && implicant[i] != m[i])
            return false;
    return true;
}

// تبدیل رشته باینری به عبارت بولی
string bin_to_expr(const string& s) {
    string expr = "";
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '1') expr += var_names[i];
        else if (s[i] == '0') expr += var_names[i] + string("'");
    }
    return expr.empty() ? "1" : expr;
}

// پیاده‌سازی الگوریتم Quine-McCluskey برای یافتن Prime Implicants
vector<string> quine_mccluskey(vector<string>& minterms_bin) {
    map<int, vector<string>> groups;
    for (auto& s : minterms_bin)
        groups[count_ones(s)].push_back(s);

    vector<string> primes;
    bool changed = true;

    while (changed) {
        changed = false;
        map<int, vector<string>> next;
        set<string> used, combined;

        vector<int> keys;
        for (auto& g : groups)
            keys.push_back(g.first);

        for (size_t i = 0; i + 1 < keys.size(); i++) {
            int k1 = keys[i], k2 = keys[i + 1];
            for (auto& a : groups[k1]) {
                for (auto& b : groups[k2]) {
                    string r;
                    if (one_bit_diff(a, b, r)) {
                        next[count_ones(r)].push_back(r);
                        used.insert(a);
                        used.insert(b);
                        combined.insert(r);
                        changed = true;
                    }
                }
            }
        }

        for (auto& g : groups)
            for (auto& term : g.second)
                if (!used.count(term))
                    primes.push_back(term);

        groups.clear();
        for (const auto& c : combined)
            groups[count_ones(c)].push_back(c);
    }

    sort(primes.begin(), primes.end());
    primes.erase(unique(primes.begin(), primes.end()), primes.end());
    return primes;
}

// یافتن Essential Prime Implicants
set<string> find_essential(const vector<string>& primes, const vector<string>& terms) {
    map<string, vector<string>> table;
    for (auto& t : terms)
        for (auto& p : primes)
            if (covers(p, t))
                table[t].push_back(p);

    set<string> essentials;
    for (auto& row : table)
        if (row.second.size() == 1)
            essentials.insert(row.second[0]);

    return essentials;
}

// اعمال روش Petrick برای یافتن پوشش مینیمال باقی‌مانده
set<string> petrick(const vector<string>& primes, const vector<string>& terms, set<string>& essentials) {
    vector<string> remaining;
    for (auto& t : terms) {
        bool covered = false;
        for (auto& e : essentials)
            if (covers(e, t)) {
                covered = true;
                break;
            }
        if (!covered) remaining.push_back(t);
    }

    if (remaining.empty()) return essentials;

    // تولید عبارات جایگزینی برای هر مینیم‌ترم
    vector<vector<string>> P;
    for (auto& t : remaining) {
        vector<string> row;
        for (auto& p : primes)
            if (covers(p, t))
                row.push_back(p);
        P.push_back(row);
    }

    // ضرب جبری ساده بین انتخاب‌ها
    vector<set<string>> combos;
    combos.push_back(set<string>());

    for (auto& row : P) {
        vector<set<string>> new_combos;
        for (auto& combo : combos)
            for (auto& opt : row) {
                auto new_combo = combo;
                new_combo.insert(opt);
                new_combos.push_back(new_combo);
            }
        combos = new_combos;
    }

    // انتخاب ترکیب با کمترین تعداد ایمپلیکنت
    int best_size = INT_MAX;
    set<string> best_combo;
    for (auto& c : combos)
        if (c.size() < best_size) {
            best_size = c.size();
            best_combo = c;
        }

    for (auto& e : essentials)
        best_combo.insert(e);

    return best_combo;
}

// تابع اصلی برنامه
int main() {
    cout << "The number of variables: ";
    cin >> num_vars;
    if (num_vars < 2 || num_vars > 6) {
        cout << "The number is out of the range.\n";
        return 1;
    }

    int n;
    cout << "The number of minterms: ";
    cin >> n;

    vector<string> minterms_bin;
    cout << "minterms: \n";
    for (int i = 0; i < n; i++) {
        int m;
        cin >> m;
        if (m >= 0 && m < (1 << num_vars))
            minterms_bin.push_back(to_binary(m, num_vars));
        else
            cout << "wrong minterm: " << m << endl;
    }

    auto primes = quine_mccluskey(minterms_bin);
    auto essentials = find_essential(primes, minterms_bin);
    auto final_terms = petrick(primes, minterms_bin, essentials);

    // نمایش نتیجه
    cout << "\nSOP: F = ";
    vector<string> terms;
    for (auto& term : final_terms)
        terms.push_back(bin_to_expr(term));

    for (size_t i = 0; i < terms.size(); i++) {
        cout << terms[i];
        if (i != terms.size() - 1) cout << " + ";
    }
    cout << "\n";

    return 0;
}
