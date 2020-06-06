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
    vector<pair<unsigned, size_t>> _paths;
public:
    PathMover(vector<int> const &stones)
        : _stones(stones)
    {
        _paths.push_back({ 0, 0 });
    }

    PathState loopMoveNext()
    {
        for (auto i = 0; i < _paths.size(); ++i)
        {
            auto step = _paths[i].first;
            auto posIndex = _paths[i].second;

            vector<unsigned> stepChoices;
            stepChoices.reserve(1);
            if (step == 0)
            {
                stepChoices.push_back(1);
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
                            // replace with new next path
                            _paths.push_back({ s, j });
                        }
                    }
                    else if (nextPoint < p)
                    {
                        break;
                    }
                }
            }
            
            // remove this old path
            _paths[i] = _paths.back(); // if these are some position, will go wrong?
            _paths.pop_back();
            --i;
        }

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
            s = p.loopMoveNext();
        } while (s == PathState::Continue);

        return s == PathState::ReachDest;
    }
};