# Quine-McCluskey
## محمدمصطفی کارشکی  40325633
## محمد عباسی  40322653
### quine-McCluskey روشی است که برای ساده سازی توابع منطقی بولی به کار می رود.
### در ابتدا تعداد متغیرها به ورودی داده می شود. سپس تعداد مینترم ها رو از ورودی دریافت می کند و پس از آن، مینترم ها را از ورودی میگیرد.
### در خروجی نیز عبارت بولی ساده شده نمایش داده می شود.
### مثال:
--- C++

    The number of variables: 3
    The number of minterms: 4
    minterms:
    1 3 5 7
    F = C
    
---
### در این پروژه 3 الگوریتم اصلی به کار رفته است:
## 1. الگوریتم Quine-McCluskey
 * ساده‌سازی تابع بولی به کمک ترکیب مین‌ترم‌هایی که فقط در یک بیت تفاوت دارند.
 * یافتن Prime Implicants (مؤلفه‌های اولی که دیگر قابل ترکیب نیستند).
## خلاصه عملکرد:
 + گروه‌بندی مینترم‌ها بر اساس تعداد بیت‌های ۱.
 + ترکیب رشته‌های باینری مشابه (با تفاوت در یک بیت) و تولید رشته‌هایی با -.
 + تکرار ترکیب تا جایی که دیگر نتوان رشته‌ها را ساده‌تر کرد.
   
--- C++

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

   
--- C++
   
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

## توابعی که در کد به کار رفته است:

## 1. to_binary
### کاربرد:
 + تبدیل یک عدد دهدهی به یک رشته ی باینری با تعداد بیت مشخص

--- C++

        string to_binary(int num, int bits) {
           return bitset<6>(num).to_string().substr(6 - bits);
           }
---



## 2. count_ones
### کاربرد:
 + شمارش تعداد بیت های 1 در رشته باینری

--- C++

    int count_ones(const string& s) {
       return count(s.begin(), s.end(), '1');
    }
---
## 3. one_bit_diff
### کاربرد:
 + بررسی اینکه دو رشته باینری فقط در یک بیت با هم تفاوت دارند

--- C++

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
---
## 4. covers
### کاربرد:
 + بررسی اینکه آیا یک ایمپلیکنت خاص می‌تواند یک مین‌ترم را پوشش دهد یا نه
--- C++

       bool covers(const string& implicant, const string& m) {
        for (int i = 0; i < m.size(); i++)
            if (implicant[i] != '-' && implicant[i] != m[i])
                return false;
          return true;
       }
---
## 5. bin_to_expr
### کاربرد:
 + تبدیل رشته باینری با عبارت بولی با متغیرهایی مانند AB و غیره
--- C++

        string bin_to_expr(const string& s) {
        string expr = "";
        for (int i = 0; i < s.size(); i++) {
        if (s[i] == '1') expr += var_names[i];
        else if (s[i] == '0') expr += var_names[i] + string("'");
        }
        return expr.empty() ? "1" : expr;
---

## کتابخانه های به کار رفته در کد:
### 1. کتابحانه bitset:
 + کلاسی قالبی است که مجموعه ای از n بیت ( 0 , 1) را در خود نگه می دارد.
 + می توان عملیاتی مانند (or , and , set , reset , flip , ...) را روی آنها انجام داد.

### 2. کتابخانه map:
 + یک ساختار داده است که که برای ذخیره یا باریابی جفت های کلید_مقدار به کار می رود.
 + هر کلید به یک مقدار (value) وصل می شود.
 + عناصر به صورت مرتب ذخیره می شوند.
