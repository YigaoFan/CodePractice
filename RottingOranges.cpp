// https://leetcode-cn.com/problems/rotting-oranges/
class Solution
{
public:
    int orangesRotting(vector<vector<int>>& grid)
    {
        auto total = 0;
        auto freshOneNum = 0;
        map<pair<int, int>, int> rottenPath;

        // 感染后的橘子才有扩散能力，位置上有橘子才有扩散能力
        function<void(int, int, int)> spread = [&rottenPath, &grid, &spread](int i, int j, int step)
        {
            // set up self
            auto pos = pair<int, int>(i, j);
            if (rottenPath.find(pos) == rottenPath.end())
            {
                rottenPath.insert({ { i, j }, step });
            }
            else if (rottenPath[pos] > step)
            {
                rottenPath[pos] = step;
            }
            else
            {
                return;
            }
            
            // set up nearby
            auto next = step + 1;
            auto onTop = false;
            auto onBottom = false;
            if (i == 0)
            {
                onTop = true;
            }
            else if (i == grid.size() - 1)
            {
                onBottom = true;
            }

            if (!onTop)
            {
                spread(i - 1, j, next);
            }

            if (!onBottom)
            {
                spread(i + 1, j, next);
            }

            auto onLeft = false;
            auto onRight = false;
            if (j == 0)
            {
                onLeft = true;
            }
            else if (j == grid[0].size() - 1)
            {
                onRight = true;
            }

            if (!onLeft)
            {
                spread(i, j - 1, next);
            }

            if (!onRight)
            {
                spread(i, j + 1, next);
            }
        };

        for (auto i = 0; i < grid.size(); ++i)
        {
            for (auto j = 0; j < grid[0].size(); ++j)
            {
                auto item = grid[i][j];
                auto pos = pair<int, int>(i, j);

                if (item == 1)
                {
                    if (rottenPath.find(pos) == rottenPath.end())
                    {
                        rottenPath.insert({ { i, j }, INT_MAX });
                    }
                    else
                    {
                        spread(i, j, rottenPath[pos]);
                    }
                    
                    ++freshOneNum;
                    ++total;
                }
                else if (item == 2)
                {
                    if (rottenPath.find(pos) == rottenPath.end())
                    {
                        rottenPath.insert({ { i, j }, INT_MAX });
                    }
                    else
                    {
                        rottenPath[pos] = 0;
                        spread(i, j, rottenPath[pos]);
                    }

                    spread(i, j, 0);
                    ++total;
                }
            }
        }

        if (total == 0 || freshOneNum == 0)
        {
            return 0;
        }
        else
        {
            auto maxStep = 0;
            for (auto& p : rottenPath)
            {
                maxStep = max(maxStep, p.second);
            }

            return maxStep == INT_MAX ? -1 : maxStep;
        }
    }
};