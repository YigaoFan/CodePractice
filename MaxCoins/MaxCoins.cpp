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

// 类似这个的东西很常用
class Permutation
{
private:
    bool _firstMove = false;
    vector<int> _options;
    unsigned _optionIndex;
    unsigned _selectCount;
    shared_ptr<Permutation> _subPermutation;

public:
    static Permutation MakePermutation(unsigned optionsCount, unsigned selectCount)
    {
        auto opts = vector<int>();
        opts.reserve(optionsCount);
        for (auto i = 0; i < optionsCount; ++i)
        {
            opts.push_back(i);
        }

        auto p = Permutation(move(opts), selectCount);
        p._firstMove = true;
        return p;
    }

    bool moveNext()
    {
        if (_options.empty())
        {
            return false;
        }

        if (_firstMove)
        {
            _firstMove = false;
            return true;
        }

        if (_subPermutation == nullptr)
        {
            return false;
        }

        if (_subPermutation->moveNext())
        {
            return true;
        }

        if ((++_optionIndex) < _options.size())
        {
            _subPermutation = shared_ptr<Permutation>(genSubOpt(_options, _optionIndex), _selectCount - 1);
        }
    }

    vector<int> current(vector<int> previousPermutation = {})
    {
        previousPermutation.push_back(_options[_optionIndex]);

        if (_subPermutation != nullptr)
        {
            return _subPermutation->current(move(previousPermutation));
        }
    }
private:
    Permutation(vector<int> options, unsigned selectCount)
        : _options(move(options)), _optionIndex(0), _selectCount(selectCount)
    {
        if (_options.size() > 1)
        {
            // 面对不同的需求，这里 selectCount 要动一动
            _subPermutation = shared_ptr<Permutation>(genSubOpt(_options, 0), selectCount - 1);
        }
    }

    // 面对不同的需求，这里要动一动
    vector<int> genSubOpt(vector<int> options, unsigned currentOptIndex)
    {
        auto b = options.begin();
        options.erase(b, b + currentOptIndex + 1);
        return options;
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
            matrix.SetAt(i, i, nums[i]);
        }

        for (auto len = 1; len <= nums.size(); ++len)
        {
            auto p = Permutation::MakePermutation(nums.size(), len);

            while (p.moveNext())
            {
                auto selects = p.current();
                auto coinCount = 0;
                for (auto firstRemove = 0; firstRemove < selects.size(); ++firstRemove)
                {
                    auto pos = selects[firstRemove];

                    auto middle = nums[pos];
                    auto left = 1;
                    if (pos != 0)
                    {
                        left = nums[pos - 1];
                    }

                    int right = 1;
                    if (pos != nums.size() - 1)
                    {
                        right = nums[pos + 1];
                    }

                    auto current = left * middle * right;
                    // 这里哪里要 max
                    current += coinCount + matrix(/*这里要重定下 cache 的 key*/);
                    coinCount = coinCount > current ? coinCount : current;
                    // 虽然这里的选择是不连续的，但是大也是包含小的选择的
                }

                // how to set coinCount into matrix
            }
        }
    }
};