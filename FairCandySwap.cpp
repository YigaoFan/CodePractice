// https://leetcode-cn.com/problems/fair-candy-swap/comments/
class Enumerator
{
private:
    vector<int> const& _vec;
    size_t _i = 0;
    bool _firstMove = true;
public:
    Enumerator(vector<int> const& vec) : _vec(vec)
    { }

    int current()
    {
        return _vec[_i];
    }

    bool moveNext()
    {
        if (_firstMove)
        {
            _firstMove = false;
            return true;
        }

        if ((++_i) < _vec.size())
        {
            return true;
        }

        return false;
    }
};

class Solution
{
public:
    // 看了下评论，发觉可以用将 A 和 B 的内容分别放到 set、查找 set 的思路来做
    // 那样应该快一些
    vector<int> fairCandySwap(vector<int>& A, vector<int>& B)
    {
        sort(A.begin(), A.end());
        sort(B.begin(), B.end());

        auto sumA = sumItemIn(A);
        auto sumB = sumItemIn(B);
        auto aver = (sumA + sumB) / 2;

        auto dis = sumA - aver;
        auto eA = Enumerator(A);
        auto eB = Enumerator(B);

        eA.moveNext();
        eB.moveNext();
        while (true)
        {
            auto itemA = eA.current();
            auto itemB = eB.current();

            if (auto r = itemA - dis; r > itemB)
            {
                eB.moveNext();
            }
            else if (r < itemB)
            {
                eA.moveNext();
            }
            else
            {
                return 
                {
                    itemA,
                    itemB,
                };
            }
        }
    }

    static int sumItemIn(vector<int> const& v)
    {
        auto sum = 0;

        for (auto i = 0; i < v.size(); ++i)
        {
            sum += v[i];
        }

        return sum;
    }
};