// https://leetcode-cn.com/problems/snakes-and-ladders/
class Position
{
private:
    shared_ptr<vector<int>> _board;
    shared_ptr<vector<bool>> _visitedMark;
    int _i;
    int _step;
    bool _lastJumped;
public:
    static Position MakePosition(vector<vector<int>> const& board, int start)
    {
        auto posNum = board.size() * board.size();
        auto flatBoard = make_shared<vector<int>>();
        flatBoard->reserve(posNum);
        auto fromHead = true;
        for (int i = board.size() - 1; i >= 0; --i, fromHead = !fromHead)
        {
            auto& v = board[i];
            auto end = flatBoard->end();
            if (fromHead)
            {
                flatBoard->insert(end, v.begin(), v.end());
            }
            else
            {
                flatBoard->insert(end, v.rbegin(), v.rend());
            }
        }

        auto visitedMark = make_shared<vector<bool>>(posNum, false);
        return Position(flatBoard, visitedMark, start, 0, false);
    }

    Position(shared_ptr<vector<int>> flatBoard, shared_ptr<vector<bool>> visitedMark, int i, int step, bool lastJumped)
        : _board(flatBoard), _visitedMark(visitedMark), _i(i), _step(step), _lastJumped(lastJumped)
    { 
        (*_visitedMark)[_i] = true;
    }

    vector<Position> nextNewPoses()
    {
        vector<int> nextIndexs;
        vector<int> jumpIndexs;

        auto max = _i + 6;
        int rightBound = max < _board->size() ? max : _board->size() - 1;

        for (auto i = _i + 1; i <= rightBound; ++i)
        {
            if (auto nextV = (*_board)[i]; nextV != -1)
            {
                jumpIndexs.push_back(nextV - 1);
            }
            else
            {
                nextIndexs.push_back(i);
            }
        }

        // 题目中没有提“上一步（靠蛇或者梯子）跳了，下一步就不能直接通过所在的位置跳”，只说了“一步移动中只能跳一次”。
        // 我是提交的测试过不了的时候，想不通，本地调试结果觉得没有问题，只是有一点特殊，两步都是跳。
        // 看评论区，才知道这是题目中未描述清楚的地方。
        // 所以在下面这行进行了这样的控制。
        if (auto v = (*_board)[_i]; !_lastJumped && v != -1)
        {
            jumpIndexs.push_back(v - 1);
        }

        vector<Position> nextPoses;
        for (auto i : nextIndexs)
        {
            if (!(*_visitedMark)[i])
            {
                nextPoses.push_back({ _board, _visitedMark, i, _step + 1, false });
            }
        }

        for (auto i : jumpIndexs)
        {
            if (!(*_visitedMark)[i])
            {
                nextPoses.push_back({ _board, _visitedMark, i, _step + 1, true });
            }
        }


        return nextPoses;
    }

    bool isEnd() const
    {
        return _i == (_board->size() - 1);
    }

    int step() const
    {
        return _step;
    }
};

class Solution
{
public:
    int snakesAndLadders(vector<vector<int>>& board)
    {
        if (board.empty() || board.size() == 1)
        {
            return 0;
        }

        vector<Position> steps
        {
            Position::MakePosition(board, 0),
        };

        while (!steps.empty())
        {
            auto s = steps[0];
            steps.erase(steps.begin());
            auto nextSteps = s.nextNewPoses();
            for (auto& s : nextSteps)
            {
                if (s.isEnd())
                {
                    return s.step();
                }
            }

            steps.insert(steps.end(), nextSteps.begin(), nextSteps.end());
        }

        return -1;
    }
};