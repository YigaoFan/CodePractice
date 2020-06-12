// https://leetcode-cn.com/problems/rotting-oranges/
class Solution
{
public:
    int orangesRotting(vector<vector<int>>& grid)
    {
        auto total = 0;
        set<pair<int, int>> freshCoords;
        set<pair<int, int>> rottedCoords;
 
        for (auto i = 0; i < grid.size(); ++i)
        {
            for (auto j = 0; j < grid[0].size(); ++j)
            {
                auto item = grid[i][j];
                auto pos = pair<int, int>(i, j);

                if (item == 1)
                {
                    freshCoords.insert(pos);
                    ++total;
                }
                else if (item == 2)
                {
                    rottedCoords.insert(pos);
                    ++total;
                }
            }
        }

        if (total == 0 || freshCoords.empty())
        {
            return 0;
        }
        else
        {
            auto getNearbyCoords = [&grid](set<pair<int, int>> const& coords)
            {
                set<pair<int, int>> nearbyCoords;
                
                for (auto& c : coords)
                {
                    auto i = c.first;
                    auto j = c.second;

                    auto onTop = false;
                    auto onBottom = false;
                    if (i == 0)
                    {
                        onTop = true;
                    }
                    if (i == (grid.size() - 1))
                    {
                        onBottom = true;
                    }

                    if (!onTop)
                    {
                        nearbyCoords.insert({ i - 1, j });
                    }
                    if (!onBottom)
                    {
                        nearbyCoords.insert({ i + 1, j });
                    }

                    auto onLeft = false;
                    auto onRight = false;
                    if (j == 0)
                    {
                        onLeft = true;
                    }
                    if (j == (grid[0].size() - 1))
                    {
                        onRight = true;
                    }

                    if (!onLeft)
                    {
                        nearbyCoords.insert({ i, j - 1 });
                    }
                    if (!onRight)
                    {
                        nearbyCoords.insert({ i, j + 1 });
                    }
                }

                return nearbyCoords;
            };

            auto newRotted = move(rottedCoords);
            for (auto min = 1; ; ++min)
            {
                newRotted = getNearbyCoords(newRotted);
                removeRotted(freshCoords, newRotted);

                if (freshCoords.empty())
                {
                    return min;
                }

                if (newRotted.empty())
                {
                    return -1;
                }
            }
        }
    }

    static void removeRotted(set<pair<int, int>>& freshCoords, set<pair<int, int>>& newPossibleRotted)
    {
        vector<pair<int, int>> toDel;

        for (auto& c : newPossibleRotted)
        {
            if (freshCoords.find(c) != freshCoords.end())
            {
                freshCoords.erase(c);
            }
            else
            {
                toDel.push_back(c);
            }
        }

        for (auto& c : toDel)
        {
            newPossibleRotted.erase(c);
        }
    }
};