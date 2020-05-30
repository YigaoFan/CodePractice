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

    vector<int> current()
    {
        if (_subPermutation == nullptr)
        {
            return { };
        }
        else
        {
            auto sub = _subPermutation->current();
            sub.insert(sub.begin(), _options[_optionIndex]);
            return sub;
        }
    }
private:
    Permutation(vector<int> options, unsigned selectCount)
        : _options(move(options)), _optionIndex(0), _selectCount(selectCount)
    {
        if (_options.size() > 1)
        {
            _subPermutation = shared_ptr<Permutation>(genSubOpt(_options, 0), selectCount - 1);
        }
    }

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