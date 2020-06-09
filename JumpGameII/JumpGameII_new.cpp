// https://leetcode-cn.com/problems/jump-game-ii/
// Time out solution
enum PathState
{
    ReachDest,
    Continue,
};

class PathMover
{
private:
    vector<int> const& _stones;
    set<int> _history;
    vector<int> _todoPaths;

public:
    PathMover(vector<int> const& stones)
        : _stones(stones)
    {
        _todoPaths.push_back(0);
    }

    PathState moveNext()
    {
        vector<int> nextGenerationPaths;
        
        for (auto p : _todoPaths)
        {
            auto maxStep = _stones[p];

            for (auto s = 1; s <= maxStep; ++s)
            {
                auto nextPoint = p + s;
                if (nextPoint == (_stones.size() - 1))
                {
                    return PathState::ReachDest;
                }
                else if (nextPoint < (_stones.size() - 1))
                {
                    if (auto newPos = nextPoint; _history.find(newPos) == _history.end())
                    {
                        nextGenerationPaths.push_back(newPos);
                    }
                }
            }
        }

        _todoPaths = move(nextGenerationPaths);
        return PathState::Continue;
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

        auto p = PathMover(nums);
        PathState s;
        auto minStepCount = 0;

        do
        {
            s = p.moveNext();
            ++minStepCount;
        } while (s == PathState::Continue);

        return minStepCount;
    }
};