#include <vector>
#include <algorithm>
#include <utility>

template <typename T>
class diff
{

public:
    int change_pen = 1, remove_pen = 1, add_pen = 1;

    std::vector<int> make_diff(std::vector<T> &a, std::vector<T> &b)
    {
        std::vector<int> ans;

        pr_make_diff(word(a, b, 0, a.size(), 0, b.size()), ans);
        return ans;
    }

private:

    struct word
    {

        std::vector<T> &a, &b;
        int start_a, start_b, end_a, end_b;
        bool reversed;

        void reverse()
        {
            std::swap(a, b);
            std::swap(start_a, start_b);
            std::swap(end_a, end_b);
            reversed ^= 1;
        }

        int alen()
        {
            return end_a - start_a;
        }

        int blen()
        {
            return end_b - start_b;
        }

        word(std::vector<T> a, std::vector<T> b, int st_a, int en_a, int st_b, int en_b, bool rev = false):
            a(a), b(b), start_a(st_a), end_a(en_a), start_b(st_b), end_b(en_b), reversed(rev)
        {
            if (alen() < blen())
                reverse();
        }

        void norm()
        {
            if (alen() < blen())
                reverse();
        }
    };

    /// 0 - do nothing
    /// 1 - change
    /// 2 - add
    /// 3 - remove

    int pr_make_diff(word w, std::vector<int> &ans)
    {
        if (w.alen() <= 0 && w.blen() <= 0)
            return 0;
        if (w.alen() <= 0 && w.blen() == 1)
        {
            ans.push_back(w.reversed ? 2 : 3);
            return 0;
        }
        if (w.alen() == 1 && w.blen() <= 0)
        {
            ans.push_back(w.reversed ? 3 : 2);
            return 0;
        }
        if (w.alen() == 1 && w.blen() == 1)
        {
            ans.push_back(w.a[w.start_a] == w.b[w.start_b] ? 0 : 1);
            return 0;
        }
        int height = w.blen() + 1;
        std::vector<int> a1(height), a2(height);

        a2[0] = 0;
        for (int i = 1; i < height; ++i)
            a2[i] = (w.reversed ? add_pen : remove_pen) * i;
        int mid = (w.start_a + w.end_a + 1) / 2;
        for (int j = w.start_a; j < mid; ++j)
        {
            a2.swap(a1);
            a2[0] = (w.reversed ? remove_pen : add_pen) + a1[0];
            for (int i = 1; i < height; ++i)
            {
                a2[i] = a1[i - 1] + (w.a[j] == w.b[w.start_b + i - 1] ? 0 : change_pen);
                a2[i] = std::min(a2[i], a2[i - 1] + (w.reversed ? add_pen : remove_pen));
                a2[i] = std::min(a2[i], a1[i] + (w.reversed ? remove_pen : add_pen));
            }
        }
        std::vector<int> b1(height), b2(height);
        b2[0] = 0;
        for (int i = 1; i < height; ++i)
            b2[i] = i * (w.reversed ? add_pen : remove_pen);
        for (int j = w.end_a - 1; j >= mid; --j)
        {
            b2.swap(b1);
            b2[0] = b1[0] + (w.reversed ? remove_pen : add_pen);
            for (int i = 1; i < height; ++i)
            {
                b2[i] = b1[i - 1] + (w.a[j] == w.b[w.end_b - i] ? 0 : change_pen);
                b2[i] = std::min(b2[i], b2[i - 1] + (w.reversed ? add_pen : remove_pen));
                b2[i] = std::min(b2[i], b1[i] + (w.reversed ? remove_pen : add_pen));
            }
        }
        int t = 0;

        for (int i = 1; i < height; ++i)
            if (a2[i] + b2[height - 1 - i] < a2[t] + b2[height - 1 - t])
                t = i;
        if (t > 0 && a2[t] == a1[t - 1] && w.a[mid - 1] == w.b[w.start_b + t - 1])
        {
            pr_make_diff(word(w.a, w.b, w.start_a, mid - 1, w.start_b, w.start_b + t - 1, w.reversed), ans);
            ans.push_back(0);
        }
        else if (t > 0 && a2[t] == a1[t - 1] + change_pen)
        {
            pr_make_diff(word(w.a, w.b, w.start_a, mid - 1, w.start_b, w.start_b + t - 1, w.reversed), ans);
            ans.push_back(1);
        }
        else if (t > 0 && a2[t] == a2[t - 1] + (w.reversed ? add_pen : remove_pen))
        {
            pr_make_diff(word(w.a, w.b, w.start_a, mid, w.start_b, w.start_b + t - 1, w.reversed), ans);
            ans.push_back(w.reversed ? 2 : 3);
        }
        else if (a2[t] == a1[t] + (w.reversed ? remove_pen : add_pen))
        {
            pr_make_diff(word(w.a, w.b, w.start_a, mid - 1, w.start_b, w.start_b + t, w.reversed), ans);
            ans.push_back(w.reversed ? 3 : 2);
        }
        else
            throw 1;

        //pr_make_diff(word(w.a, w.b, w.start_a, mid, w.start_b, w.start_b + t, w.reversed), ans);
        pr_make_diff(word(w.a, w.b, mid, w.end_a, w.start_b + t, w.end_b, w.reversed), ans);
        return 0;
    }


};
