// https://leetcode-cn.com/problems/permutations-ii/
class Permutation
{
private:
    bool _firstMove = false;
    vector<int> const& _options;
    shared_ptr<map<int, bool>> _indexStates; // true means using, false means not
    unsigned _currentIndex;
    unsigned _selectCount;
    shared_ptr<Permutation> _subPermutation;

public:
    static Permutation MakePermutation(vector<int> const& nums)
    {
        auto indexStates = make_shared<map<int, bool>>();
        return Permutation(nums, indexStates);
    }

    void setSelectCount(unsigned selectCount)
    {
        cancelAllUsedIndex();
        _firstMove = true;
        _selectCount = selectCount;
        _currentIndex = 0;
        registerIndexInUse(_currentIndex);
        setupSub();
    }

    bool moveNext()
    {
        if (_firstMove)
        {
            _firstMove = false;
            return true;
        }

        if (_subPermutation != nullptr && _subPermutation->moveNext())
        {
            return true;
        }

        cancelUsingIndex(_currentIndex);
        auto hasSub = true;
        if (_subPermutation == nullptr)
        {
            hasSub = false;
        }
        else
        {
            _subPermutation.reset();
        }
        
        if (auto r = getNextIndex(_currentIndex); r)
        {
            _currentIndex = r.value();
            if (hasSub)
            {
                setupSub();
            }

            return true;
        }

        return false;
    }

    vector<int> current(vector<int> previousPermutation = {})
    {
        previousPermutation.push_back(_options[_currentIndex]);

        if (_subPermutation != nullptr)
        {
            return _subPermutation->current(move(previousPermutation));
        }

        return previousPermutation;
    }

    Permutation(vector<int> const& options, shared_ptr<map<int, bool>> selectedIndexs)
        : _options(options), _indexStates(selectedIndexs)
    { }

    Permutation(vector<int> const& options, shared_ptr<map<int, bool>> selectedIndexs, unsigned selectCount, unsigned currentIndex)
        : _options(options), _indexStates(selectedIndexs), _selectCount(selectCount), _currentIndex(currentIndex)
    {
        setupSub();
    }

    ~Permutation()
    {
        cancelUsingIndex(_currentIndex);
    }

private:
    void setupSub()
    {
        if (auto subSelectCount = _selectCount - 1; subSelectCount > 0)
        {
            // 这里为什么 get 不到 free index 啊？
            _subPermutation = make_shared<Permutation>(_options, _indexStates, subSelectCount, getFreeIndex());
        }
    }

    optional<int> getNextIndex(int optionIndex)
    {
        while ((++optionIndex) < _options.size())
        {
            if (isFreeIndex(optionIndex))
            {
                registerIndexInUse(optionIndex);
                return optionIndex;
            }
        }

        return { };
    }

    int getFreeIndex()
    {
        for (auto it = _indexStates->begin(); it != _indexStates->end(); ++it)
        {
            if (!it->second)
            {
                it->second = true;
                return it->first;
            }
        }

        throw runtime_error("Free index not found");
    }

    void registerIndexInUse(int i)
    {
        (*_indexStates)[i] = true;
    }

    void cancelUsingIndex(int i)
    {
        (*_indexStates)[i] = false;
    }

    bool isFreeIndex(int i)
    {
        return !(*_indexStates)[i];
    }

    void cancelAllUsedIndex()
    {
        if (_indexStates->empty())
        {
            for (auto i = 0; i < _options.size(); ++i)
            {
                _indexStates->insert({ i, false });
            }

            return;
        }

        for (auto it = _indexStates->begin(); it != _indexStates->end(); ++it)
        {
            if (it->second)
            {
                it->second = false;
            }
        }
    }
};

class Solution
{
public:
    vector<vector<int>> permuteUnique(vector<int>& nums)
    {
        vector<vector<int>> allP;
        auto n = nums.size();
        auto p = Permutation::MakePermutation(nums);
        p.setSelectCount(n);

        while (p.moveNext())
        {
            allP.push_back(p.current());
        }

        // 这里使用的是全枚举过后排序去重，
        // 也可以枚举的时候加一些代码，防止重复枚举
        sort(allP.begin(), allP.end());
        auto last = unique(allP.begin(), allP.end());
        allP.erase(last, allP.end());
        return allP;
    }
};