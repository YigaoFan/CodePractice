// https://leetcode-cn.com/problems/jump-game-ii/
// Time out solution
enum PathState
{
    NoWayToGo,
    ReachDest,
    Continue,
};

class PathMover
{
private:
    vector<int> const& _stones;
    set<pair<int, size_t>> _history;
    // pair: step count, current stone index
    vector<pair<int, size_t>> _todoPaths;

public:
    PathMover(vector<int> const& stones)
        : _stones(stones)
    {
        _todoPaths.push_back({ 0, 0 });
    }

    pair<PathState, int> moveNext()
    {
        if (_todoPaths.empty())
        {
            return { PathState::NoWayToGo, 0 };
        }

        // 选择离终点最近的那个先开始
        auto fastestPath = _todoPaths.back();
        _todoPaths.pop_back();

        auto stepCount = fastestPath.first;
        auto posIndex = fastestPath.second;
        auto maxStep = _stones[posIndex];

        for (auto s = 1; s <= maxStep; ++s)
        {
            auto nextPoint = posIndex + s;
            if (nextPoint == (_stones.size() - 1))
            {
                _history.insert(fastestPath);
                return { PathState::ReachDest, stepCount + 1 };
                // 这里 return 会丢掉其他的 s，但对这题没有影响，因为其他的 s 的步数肯定比这个多
                // 而且这里应该把同一个循环里 push_back 的 newPos 给吐出来
            }
            else if (nextPoint < (_stones.size() - 1))
            {
                if (auto newPos = pair<int, size_t>(stepCount + 1, nextPoint); _history.find(newPos) == _history.end())
                {
                    _todoPaths.push_back(newPos);
                }
            }
        }

        _history.insert(fastestPath);
        return { PathState::Continue, 0 };
    }
};

class Solution
{
public:
    int jump(vector<int>& nums)
    {
        auto p = PathMover(nums);
        pair<PathState, int> s;
        optional<int> minStepCount;

        do
        {
            do
            {
                s = p.moveNext();
            } while (s.first == PathState::Continue);

            if (s.first == PathState::ReachDest)
            {
                minStepCount = minStepCount ? 
                    (minStepCount.value() < s.second ? minStepCount.value() : s.second)
                     :
                    s.second;
            }
            else
            {
                break;
            }
        } while (true);

        return minStepCount ? minStepCount.value() : 0;
    }
};