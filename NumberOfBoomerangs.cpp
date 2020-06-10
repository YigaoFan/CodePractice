// https://leetcode-cn.com/problems/number-of-boomerangs/description/
// LeetCode 报最后一个测试用例超时了，但没有显示最后一个测试用例是什么
class Solution
{
public:
    int numberOfBoomerangs(vector<vector<int>>& points)
    {
        auto n = 0;
        map<pair<int, int>, int> sqrDisCounter;
        for (auto i = 0; i < points.size(); ++i)
        {
            auto &p1 = points[i];
            for (auto j = i + 1; j < points.size(); ++j)
            {
                auto& p2 = points[j];
                auto dx = p1[0] - p2[0];
                auto dy = p1[1] - p2[1];
                auto sqrDis = dx * dx + dy * dy;
                ++sqrDisCounter[{ sqrDis, i }];
                ++sqrDisCounter[{ sqrDis, j }];
            }
        }

        n += countBoomerangsIn(sqrDisCounter);
        // 有一种 LeetCode 过了的写法是把 map 和上面这句移进 for 循环内，把 j = 0;
        // 这种写法比我的这种写法效率高很多吗？
        // 最后一个那个没过的测试用例显示不出来，我自己也测试不了。
        return n;
    }

    static int countBoomerangsIn(map<pair<int, int>, int> const& sqrDisCounter)
    {
        auto n = 0;
        for (auto& p : sqrDisCounter)
        {
            auto toPointCount = p.second;
            if (toPointCount > 1)
            {
                n += (toPointCount * (toPointCount - 1));
            }
        }

        return n;
    }
};