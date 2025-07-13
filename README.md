# Quine-McCluskey
## محمدمصطفی کارشکی  40325633
## محمد عباسی  40322653
### quine-McCluskey روشی است که برای ساده سازی توابع منطقی بولی به کار می رود.
### در این پروژه 3 الگوریتم اصلی به کار رفته است:
## 1. الگوریتم Quine-McCluskey
 * ساده‌سازی تابع بولی به کمک ترکیب مین‌ترم‌هایی که فقط در یک بیت تفاوت دارند.
 * یافتن Prime Implicants (مؤلفه‌های اولی که دیگر قابل ترکیب نیستند).
## خلاصه عملکرد:
 + گروه‌بندی مینترم‌ها بر اساس تعداد بیت‌های ۱.
 + ترکیب رشته‌های باینری مشابه (با تفاوت در یک بیت) و تولید رشته‌هایی با -.
 + تکرار ترکیب تا جایی که دیگر نتوان رشته‌ها را ساده‌تر کرد.
## توابع به کار رفته:
---c++

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

---
## 2. Essential Prime Implicants
 + تشخیص اینکه کدام Prime Implicant حتماً باید در تابع نهایی باشد، چون تنها آن می‌تواند یک مینترم خاص را پوشش دهد.
## خلاصه عملکرد:
 + ساخت یک جدول پوشش‌دهی.
 + بررسی اینکه هر ‌میتترم توسط چند ایمپلیکنت پوشش داده شده است.
 + اگر فقط یکی از ایمپلیکنت‌ها آن مینترم را پوشش دهد، آن ایمپلیکنت ضروری (Essential) است.
---c++
   
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

---
## 3. Petrick' s method
 + پوشش دادن مین‌ترم‌هایی که توسط ایمپلیکنت‌های ضروری پوشش داده نشده‌اند.
 + پیدا کردن ترکیب کمینه از ایمپلیکنت‌ها برای پوشش کامل باقی‌مانده‌ها.
## خلاصه عملکرد:
 + ساخت جدول عبارات منطقی برای باقی‌مانده مینترم ها.
 + ضرب جبری بین ایمپلیکنت‌های ممکن (از نظر پوشش‌دهی).
 + انتخاب ترکیبی که کمترین تعداد ایمپلیکنت را استفاده کند (بهینه‌ترین).
---c++
   
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
       
---




