// https://leetcode-cn.com/problems/decode-ways-ii/
class Solution
{
private:
    enum HeadBitInTwo
    {
        One,
        Two,
    };
    using WaysType = long;

public:
    int numDecodings(string s)
    {
        assert(!s.empty());

        // 题目中没有说明下面这个，是通过提交测试出来的
        if (s[0] == '0')
        {
            return 0;
        }

        vector<int> waysTable;
        waysTable.push_back(1);
        if (s[0] == '*')
        {
            waysTable.push_back(9);
        }
        else
        {
            waysTable.push_back(1);
        }

        int m = pow(10, 9) + 7;
        for (auto i = 2; i <= s.size(); ++i)
        {
            WaysType ways = 0;

            auto r1 = detectOneBitConvert(s, i);
            ways += promotedMul(waysTable[i - 1], r1);

            auto r2 = detectTwoBitConvert(s, i);
            ways += promotedMul(waysTable[i - 2], r2);

            waysTable.push_back(static_cast<int>(ways % m));
        }

        auto r = waysTable[s.size()];
        return r;
    }

    int detectTwoBitConvert(string const &s, int i)
    {
        switch (s[i - 2])
        {
        case '1':
            return 1 * detect2ndBitConvertInTwoBit<HeadBitInTwo::One>(s, i);

        case '2':
            return 1 * detect2ndBitConvertInTwoBit<HeadBitInTwo::Two>(s, i);

        case '*':
            return detect2ndBitConvertInTwoBit<HeadBitInTwo::One>(s, i) 
                + detect2ndBitConvertInTwoBit<HeadBitInTwo::Two>(s, i);

        default:
            return 0;
        }
    }

    template <HeadBitInTwo HeadKind>
    int detect2ndBitConvertInTwoBit(string const &s, int i)
    {
        if (auto second = s[i - 1]; second != '*')
        {
            if constexpr (HeadKind == HeadBitInTwo::One)
            {
                return 1;
            }
            else if (second >= '0' && second <= '6')
            {
                return 1;
            }
            
            return 0;
        }
        else
        {
            if constexpr (HeadKind == HeadBitInTwo::One)
            {
                return 9;
            }
            else
            {
                return 6; // second >= '1' && second <= '6'
            }
        }
    }

    int detectOneBitConvert(string const &s, int i)
    {
        if (auto c = s[i - 1]; c >= '1' && c <= '9')
        {
            return 1;
        }
        else if (c == '*')
        {
            return 9;
        }

        return 0;
    }

    static WaysType promotedMul(WaysType n1, WaysType n2)
    {
        return n1 * n2;
    }
};