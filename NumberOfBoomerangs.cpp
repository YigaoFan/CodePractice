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