// https://leetcode-cn.com/problems/burst-balloons/
// Matrix 没用到
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
class Combination
{
private:
    bool _firstMove = false;
    shared_ptr<vector<int>> _options;
    unsigned _optionIndex;
    unsigned _selectCount;
    shared_ptr<Combination> _subCombination;

public:
    static Combination MakeCombination(unsigned optionsCount)
    {
        auto opts = make_shared<vector<int>>();
        opts->reserve(optionsCount);
        for (auto i = 0; i < optionsCount; ++i)
        {
            opts->push_back(i);
        }

        return Combination(move(opts));
    }

    void setSelectCount(unsigned selectCount)
    {
        _firstMove = true;
        _selectCount = selectCount;
        _optionIndex = 0;
        setupSub();
    }

    bool moveNext()
    {
        if (_options->empty())
        {
            return false;
        }

        if (_firstMove)
        {
            _firstMove = false;
            return true;
        }

        if (_subCombination != nullptr && _subCombination->moveNext())
        {
            return true;
        }

        if (((++_optionIndex) + _selectCount) <= _options->size())
        {
            if (_subCombination != nullptr)
            {
                _subCombination = make_shared<Combination>(_options, _selectCount - 1, _optionIndex + 1);
            }
            
            return true;
        }

        return false;
    }

    vector<int> current(vector<int> previousCombination = {})
    {
        previousCombination.push_back((*_options)[_optionIndex]);

        if (_subCombination != nullptr)
        {
            return _subCombination->current(move(previousCombination));
        }

        return previousCombination;
    }

    Combination(shared_ptr<vector<int>> options) : _options(options)
    { }

    Combination(shared_ptr<vector<int>> options, unsigned selectCount, unsigned startIndex)
        : _options(options), _selectCount(selectCount), _optionIndex(startIndex)
    {
        setupSub();
    }

private:
    void setupSub()
    {
        if (auto subSelectCount = _selectCount - 1; subSelectCount > 0)
        {
            // 面对不同的需求，这里 selectCount 要动一动
            _subCombination = make_shared<Combination>(_options, subSelectCount, _optionIndex + 1);
        }
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

        map<vector<int>, int> table;
        for (auto n : nums)
        {
            table.insert({ { n, }, n });
        }

        // 这个 c 的所有枚举个数加起来有 2^n
        auto c = Combination::MakeCombination(nums.size());
        for (auto len = 2; len <= nums.size(); ++len)
        {
            c.setSelectCount(len);
            while (c.moveNext())
            {
                auto selects = c.current();
                auto items = collectItemByIndices(selects, nums);

                auto coinCount = 0;
                if (table.find(items) != table.end())
                {
                    coinCount = table[items];
                }
                else
                {
                    for (auto firstRemove = 0; firstRemove < selects.size(); ++firstRemove)
                    {
                        auto pos = selects[firstRemove];
                        auto middle = nums[pos];

                        auto left = 1;
                        if (firstRemove != 0)
                        {
                            left = nums[selects[firstRemove - 1]];
                        }

                        auto right = 1;
                        if (firstRemove != (selects.size() - 1))
                        {
                            right = nums[selects[firstRemove + 1]];
                        }

                        auto current = left * middle * right;
                        auto removePos = selects.begin() + firstRemove;
                        auto itemRemovePos = items.begin() + firstRemove;
                        selects.erase(removePos);
                        items.erase(itemRemovePos);
                        current += table[items];
                        selects.insert(removePos, pos);
                        items.insert(itemRemovePos, middle);
                        coinCount = coinCount > current ? coinCount : current;
                    }
                }

                table.insert({ move(items), coinCount});
            }
        }

        return table[nums];
    }

    vector<int> collectItemByIndices(vector<int> const& indices, vector<int> const& nums) const
    {
        vector<int> items;
        items.reserve(indices.size());

        for (auto i : indices)
        {
            items.push_back(nums[i]);
        }

        return items;
    }
};