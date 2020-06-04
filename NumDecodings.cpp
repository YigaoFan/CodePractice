// https://leetcode-cn.com/problems/decode-ways/
class Solution
{
public:
    int numDecodings(string s)
    {
        // 题目中没有说明下面这个，是通过提交测试出来的
        if (s[0] == '0')
        {
            return 0;
        }

        vector<int> waysTable;
        waysTable.push_back(1);
        waysTable.push_back(1);

        for (auto i = 2; i <= s.size(); ++i)
        {
            auto ways = 0;
            if (oneBitConvertible(s, i))
            {
                ways += waysTable[i - 1];
            }

            if (twoBitConvertible(s, i))
            {
                ways += waysTable[i - 2];
            }

            waysTable.push_back(ways);
        }

        return waysTable[s.size()];
    }

    bool twoBitConvertible(string const& s, int i)
    {
        switch (s[i - 2])
        {
        case '1':
            return true;
        
        case '2':
            if (auto second = s[i - 1]; second >= '0' && second <= '6')
            {
                return true;
            }

        default:
            return false;
        }
    }

    bool oneBitConvertible(string const &s, int i)
    {
        if (auto c = s[i - 1]; c >= '1' && c <= '9')
        {
            return true;
        }

        return false;
    }
};