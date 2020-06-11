// https://leetcode-cn.com/problems/partition-equal-subset-sum/
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
    bool canPartition(vector<int>& nums)
    {
        if (nums.size() == 1)
        {
            return false;
        }

        auto sum = 0;
        for (auto n : nums)
        {
            sum += n;
        }

        if (sum % 2 != 0)
        {
            return false;
        }

        return canCollect(sum / 2, nums);
    }

    static bool canCollect(int total, vector<int> const& nums)
    {
        auto n = nums.size();
        Matrix<int> matrix(n, total + 1);

        for (int i = n - 1; i >= 0; --i)
        {
            for (auto j = 0; j <= total; ++j)
            {
                if (i == n - 1)
                {
                    matrix.SetAt(i, j, 0);
                }
                else
                {
                    matrix.SetAt(i, j, matrix(i + 1, j));
                }

                if (j >= nums[i])
                {
                    auto afterGet = 0;
                    if (i == n - 1)
                    {
                        afterGet = nums[i];
                    }
                    else
                    {
                        afterGet = matrix(i + 1, j - nums[i]) + nums[i];
                    }

                    if (afterGet > matrix(i, j))
                    {
                        matrix.SetAt(i, j, afterGet);
                    }
                }
            }
        }

        return matrix(0, total) == total;
    }
};