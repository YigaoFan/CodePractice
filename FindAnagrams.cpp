// https://leetcode-cn.com/problems/find-all-anagrams-in-a-string/
class Solution
{
public:
    vector<int> findAnagrams(string s, string p)
    {
        if (s.size() < p.size())
        {
            return { };
        }

        vector<int> matchIdxs;
        auto viewP = string_view(p);

        auto desCharFreq = statisticCharFrequency(viewP);
        auto i = 0;
        auto firstFreq = statisticCharFrequency(string_view(&s[i], p.size()));

        do
        {
            if (firstFreq == desCharFreq)
            {
                matchIdxs.push_back(i);
            }

            // update
            --firstFreq[s[i]];
            
            ++i;
            if (i < (s.size() - p.size() + 1))
            {
                ++firstFreq[s[i + p.size() - 1]];
            }
            else
            {
                break;
            }
        } while (true);

        return matchIdxs;
    }

private:
    static map<char, unsigned> statisticCharFrequency(string_view s)
    {
        map<char, unsigned> charCounter;
        // 由于这里的字符串只有字母，所以可以像下面这样将所有可能的元素都插入
        // 这样就保证了所有统计元素出现的时候，map 中项的数目一致性，
        // 不用当某个字母的数目为零时，erase 掉相应的项
        // 不 erase 掉依旧保留的话，会干扰 map 之间的比较，因为一个有这个项，另一个没有这个项，但有的这个项的 value 值为 0，代表这个字符其实没有
        for (auto c = 'a'; c <= 'z'; ++c)
        {
            charCounter.insert({ c, 0 });
        }

        for (auto c : s)
        {
            ++charCounter[c];
        }

        return charCounter;
    }
};