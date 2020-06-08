// https://leetcode-cn.com/problems/frog-jump/
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
    // pair: last step size, current stone index
    set<pair<unsigned, size_t>> _history;
    vector<pair<unsigned, size_t>> _todoPaths;
public:
    PathMover(vector<int> const &stones)
        : _stones(stones)
    {
        _todoPaths.push_back({ 0, 0 });
    }

    PathState moveNext()
    {
        // 选择离终点最近的那个先开始
        auto fastestPath = _todoPaths.back();
        _todoPaths.pop_back();
        auto step = fastestPath.first;
        auto posIndex = fastestPath.second;

        vector<unsigned> stepChoices;
        stepChoices.reserve(1);
        if (step == 0)
        {
            stepChoices.push_back(1);
        }
        else if (step == 1)
        {
            stepChoices = vector<unsigned>
            {
                step,
                step + 1,
            };
        }
        else
        {
            stepChoices = vector<unsigned>
            {
                step - 1,
                step,
                step + 1,
            };
        }

        for (auto s : stepChoices)
        {
            auto nextPoint = _stones[posIndex] + s;
            for (auto j = posIndex + 1; j < _stones.size(); ++j)
            {
                if (auto p = _stones[j]; p == nextPoint)
                {
                    if (j == _stones.size() - 1)
                    {
                        return PathState::ReachDest;
                    }
                    else
                    {
                        if (auto p = pair<unsigned, size_t>(s, j); _history.find(p) == _history.end())
                        {
                            _todoPaths.push_back(p);
                        }
                    }
                }
                else if (nextPoint < p)
                {
                    break;
                }
            }
        }

        _history.insert(fastestPath);
        return _todoPaths.empty() ? PathState::NoWayToGo : PathState::Continue;
    }
};

class Solution
{
public:
    bool canCross(vector<int>& stones)
    {
        auto p = PathMover(stones);
        PathState s;

        do
        {
            s = p.moveNext();
        } while (s == PathState::Continue);

        return s == PathState::ReachDest;
    }
};