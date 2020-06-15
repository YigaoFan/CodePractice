// https://leetcode-cn.com/problems/trapping-rain-water/
enum Trend
{
    Rise,
    Fall,
    Unchange,
};

class Solution
{
public:
    int trap(vector<int>& height)
    {
        if (height.empty() && height.size() <= 2)
        {
            return 0;
        }

        // parting
        auto topPoints = statisticsTopIndexs(height);
        auto water = 0;
        // compute each part
        for (auto i = 0; i < topPoints.size() - 1; ++i)
        {
            auto left = topPoints[i];
            auto right = topPoints[i + 1];
            auto level = min(height[left], height[right]);
            for (auto k = left; k <= right; ++k)
            {
                auto h = height[k];
                if (auto w = level - h; water > 0)
                {
                    water += w;
                }
            }
        }

        return water;
    }

    static vector<int> statisticsTopIndexs(vector<int> const& heights)
    {
        vector<Trend> trends;
        for (auto i = 0; i < heights.size() - 1; ++i)
        {
            auto v1 = heights[i];
            auto v2 = heights[i + 1];
            if (v1 > v2)
            {
                trends.push_back(Trend::Fall);
            }
            else if (v1 < v2)
            {
                trends.push_back(Trend::Rise);
            }
            else
            {
                trends.push_back(Trend::Unchange);
            }
        }
        trends.push_back(Trend::Fall); // 最后一个的这个状态设置得对不对？暂时还不知道

        vector<pair<int, int>> topPoints;
        // 下面这些要不要处理 Unchange 的事情
        if (trends[0] == Trend::Fall)
        {
            topPoints.push_back({ heights[0], 0 });
        }

        for (auto i = 1; i < trends.size(); ++i)
        {
            auto last = trends[i - 1];
            auto current = trends[i];

            if (last == Trend::Rise && current == Trend::Fall)
            {
                topPoints.push_back({ heights[i], i });
            }
        }

        sort(topPoints.begin(), topPoints.end()); // sort by item.first

        // 消除无效的顶点
        vector<pair<int, int>> pillarPairs{ topPoints.back(), };
        topPoints.pop_back();

        while (!topPoints.empty())
        {
            auto p = topPoints.back();
            topPoints.pop_back();

            // if index is between first and last one, it need to exist, because higer pillar exists.
            if (auto i = p.second; i < pillarPairs[0].second) // index before the first one
            {
                pillarPairs.insert(pillarPairs.begin(), p);
            }
            else if (i > pillarPairs.back().second) // index after the last one
            {
                pillarPairs.push_back(p);
            }
        }
        
        vector<int> points;
        points.reserve(pillarPairs.size());
        for (auto& p : pillarPairs)
        {
            points.push_back(p.second);
        }

        return points;
    }
};