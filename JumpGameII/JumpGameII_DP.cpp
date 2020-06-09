// https://leetcode-cn.com/problems/jump-game-ii/
// Time out solution

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
    { }

    T& operator() (unsigned rowIndex, unsigned colIndex)
    {
        return _data[rowIndex * _colCount + colIndex];
    }

    void SetAt(unsigned rowIndex, unsigned colIndex, T value)
    {
        auto addr = _data + (rowIndex * _colCount + colIndex);
        _alloc.construct(addr, move(value));
        _constructedAddrs.push_back(addr);
    }

    unsigned rowSize() const
    {
        return _rowCount;
    }

    unsigned colSize() const
    {
        return _colCount;
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
    int jump(vector<int>& nums)
    {
        Matrix<int> matrix(nums.size(), nums.size());

        for (auto i = 0; i < nums.size(); ++i)
        {
            auto minStepToI = getMinStepTo(i, matrix);
            matrix.SetAt(i, i, minStepToI);

            auto n = nums[i];

            for (auto j = i + 1; j < nums.size(); ++j)
            {
                if (j <= (i + n))
                {
                    matrix.SetAt(i, j, minStepToI + 1);
                }
                else
                {
                    matrix.SetAt(i, j, INT_MAX);
                }
            }
        }

        return matrix(nums.size() - 1, nums.size() - 1);
    }

    static int getMinStepTo(int i, Matrix<int>& matrix)
    {
        if (i == 0)
        {
            return 0;
        }

        auto min = INT_MAX;
        // (*, i)
        for (auto r = 0; r < i; ++r)
        {
            auto stepCount = matrix(r, i);
            min = min < stepCount ? min : stepCount;
        }

        return min;
    }
};