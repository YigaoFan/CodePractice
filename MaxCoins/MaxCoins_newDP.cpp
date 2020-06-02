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
    int maxCoins(vector<int>& nums)
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

        Matrix<int> matrix(nums.size(), nums.size());

        for (unsigned i = 0; i < nums.size(); ++i)
        {
            matrix.SetAt(i, i, nums[i]);
        }

        for (unsigned len = 2; len <= nums.size(); ++len)
        {
            for (unsigned i = 1; i <= nums.size() - len + 1; ++i)
            {
                auto j = i + len - 1;
                auto coinCount = 0;

                for (unsigned k = i; k < j; ++k)
                {
                    auto value = nums[i - 1] * nums[k] * nums[j];
                    value += matrix(i, k) + matrix(k + 1, j);
                    coinCount = coinCount > value ? coinCount : value;
                }
                
                matrix.SetAt(i, j , coinCount);
            }
        }

        return matrix(0, nums.size() - 1);
    }
};
