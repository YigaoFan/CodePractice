// https://leetcode-cn.com/problems/burst-balloons/
template <typename T>
class Matrix
{
private:
    unsigned _rowCount;
    unsigned _colCount;
    allocator<T> _alloc;
    T* _data;
    vector<T*> _constructedAddrs;

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
    int maxCoins(vector<int>& nums) 
    {
        Matrix<int> matrix(nums.size(), nums.size());
        for (unsigned i = 0; i < nums.size(); ++i)
        {
            matrix.SetAt(i, i, 0);
        }

        for (auto len = 1; len <= nums.size(); ++len)
        {
            for (auto i = 0; i < nums.size() - len; ++i)
            {
                auto j = i + len - 1;
                auto value = 0;
                for (auto k = i; k <= j; ++k)
                {
                    auto total = matrix(i, k) + matrix(k + 1, j) + nums[k];
                    value = max(value, total);
                }

                matrix.SetAt(i, j, value);
            }
        }

        return matrix(nums.size() - 1, nums.size() - 1);
    }
};