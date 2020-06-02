template <typename T>
class Matrix
{
private:
    unsigned _rowCount;
    unsigned _colCount;
    allocator<T> _alloc;
    T *_data;
    vector<T *> _constructedAddrs;

public:
    Matrix(unsigned rowCount, unsigned colCount)
        : _rowCount(rowCount), _colCount(colCount),
          _alloc(), _data(_alloc.allocate(rowCount * colCount))
    {
    }

    T &operator()(unsigned rowIndex, unsigned colIndex)
    {
        return _data[rowIndex * _colCount + colIndex];
    }

    void SetAt(unsigned rowIndex, unsigned colIndex, T value)
    {
        auto addr = _data + (rowIndex * _colCount + colIndex);
        _alloc.construct(addr, move(value));
        _constructedAddrs.push_back(addr);
    }

    ~Matrix()
    {
        for (auto addr : _constructedAddrs)
        {
            _alloc.destroy(addr);
        }

        _alloc.deallocate(_data, _rowCount * _colCount);
    }
};

class Solution
{
public:
    // 以下牵涉《算导》15.2 节
    // 这个问题其实和矩阵链相乘的问题是一样的。
    // 将矩阵用 i * j 表示。
    // 矩阵链相乘中，矩阵链的第一项的 i 是不会被消去的，最后一项的 j 也是不会消去的
    // 所以以下 nums 的输入中第一位插入 1，让原来的第一项 i 被消去
    // 在最后一位放入，让原来的最后一项 j 被消去
    // 否则直接把 nums 当作矩阵链的输入来算，这第一项和最后一项不会被消去，就不符合题意了。
    // 所以要加两项，让原来这两项加入到消去的运算过程中来。
    // 为什么会想到还原成矩阵链来做呢？
    // 因为我看到题目的评论里有人的代码有加了这两项，又说了状态转移方程的 i，j 不会消去
    // 我看了一点不太明白，因为我原来在 MaxCoins.cpp 中按枚举出所有组合的方式那样来做的，猜想这个的方式会不会漏掉什么情况。
    // 后来我想到：这里其实就是最终结果是 1 * 1 的矩阵链相乘嘛，最外层两边都是 1，中间随便你怎么乘
    // 我把它补成 1 * 1 的样子就行了
    // 然后比较麻烦就是 Matrix 偏移位置了，因为《算导》上这个偏移是从 1 开始的，就有些浑。
    // 后来知道，后来知道第一项不会消去的缘故，正好在最前面多了一项，所以 i，j，k 的偏移相对书上来说，是不用变的
    // 要减一的地方，是所有与 matrix 相关的地方。
    int maxCoins(vector<int> &nums)
    {
        if (nums.empty())
        {
            return 0;
        }

        if (nums.size() == 1)
        {
            return nums[0];
        }

        nums.insert(nums.begin(), 1);
        nums.push_back(1);

        auto n = nums.size() - 1;
        Matrix<int> matrix(n, n);
        for (unsigned i = 0; i < n; ++i)
        {
            matrix.SetAt(i, i, 0);
        }

        for (unsigned len = 2; len <= n; ++len)
        {
            for (unsigned i = 1; i <= n - len + 1; ++i)
            {
                auto j = i + len - 1;
                auto coinCount = 0;

                for (unsigned k = i; k < j; ++k)
                {
                    auto value = nums[i - 1] * nums[k] * nums[j];
                    value += matrix(i - 1, k - 1) + matrix(k, j - 1);
                    coinCount = coinCount > value ? coinCount : value;
                }

                matrix.SetAt(i - 1, j - 1, coinCount);
            }
        }

        return matrix(0, n - 1);
    }
};
