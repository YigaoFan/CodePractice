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
    vector<pair<unsigned, size_t>> _paths;
public:
    PathMover(vector<int> const &stones)
        : _stones(stones)
    {
        _paths.push_back({ 0, 0 });
    }

    PathState moveNext()
    {
        vector<pair<unsigned, size_t>> nextGenerationPaths;

        for (auto& p : _paths)
        {
            auto step = p.first;
            auto posIndex = p.second;

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
                            // 选择离终点最近最快的那个先开始
                            if (auto p = pair<unsigned, size_t>(s, j); _history.find(p) == _history.end())
                            {
                                nextGenerationPaths.push_back(p);
                            }
                        }
                    }
                    else if (nextPoint < p)
                    {
                        break;
                    }
                }
            }
        }

        _history.insert(_paths.begin(), _paths.end());
        _paths = move(nextGenerationPaths);
        return _paths.empty() ? PathState::NoWayToGo : PathState::Continue;
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