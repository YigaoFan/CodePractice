// https://leetcode-cn.com/problems/jump-game-ii/
// Time out solution
class Ranges
{
private:
    map<int, int> _rightToMostLeft;
public:
    Ranges(vector<int> const& nums)
    {
        auto tail = nums.size() - 1;
        for (auto i = 0; i <= tail; ++i)
        {
            auto n = nums[i];
            if (auto max = i + n; tail <= max)
            {
                _rightToMostLeft.insert({ tail, i });
                break;
            }
            else
            {
                for (auto des = i; des <= max; ++des)
                {
                    if (_rightToMostLeft.find(des) == _rightToMostLeft.end())
                    {
                        _rightToMostLeft.insert({ des, i });
                    }
                }
            }
        }
    }

    int findMostLeftTo(int i)
    {
        return _rightToMostLeft[i];
    }
};

class Solution
{
public:
    int jump(vector<int>& nums)
    {
        if (nums.size() == 1)
        {
            return 0;
        }

        Ranges ranges(nums);

        auto step = 0;
        for (auto right = nums.size() - 1; right != 0; right = ranges.findMostLeftTo(right), ++step)
        { }

        return step;
    }
};