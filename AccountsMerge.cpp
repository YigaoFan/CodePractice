// https : //leetcode-cn.com/problems/accounts-merge/submissions/
class Dimension
{
private:
    vector<int> _data; // Item: 1 means exist, 0 means not exist
public:
    Dimension(size_t size) : _data(size, 0)
    { }

    void setExistAt(size_t i)
    {
        if (i < _data.size())
        {
            _data[i] = 1;
        }
        else
        {
            _data.push_back(0);
            setExistAt(i);
        }
    }

    int operator[] (int accountIndex) const
    {
        if (accountIndex < _data.size())
        {
            return _data[accountIndex];
        }

        return 0;
    }

    operator vector<int>() const
    {
        return _data;
    }

    auto size() { reutrn _data.size(); }
};

class AddrQuery
{
private:
    vector<pair<int, string>> _sortedIndexedAddrs;

public:
    AddrQuery(vector<string> addrs)
        : _sortedIndexedAddrs(sort(packageWithIndex(move(addrs))))
    { }

    vector<string> queryAddrsThenSort(vector<int> indexs)
    {
        vector<int> buckets(indexs.size(), 0);

        for (auto i : indexs)
        {
           buckets[find(i)] = 1;
        }

        vector<string> addrs;
        for (auto j = 0; j < buckets.size(); ++j)
        {
            auto b = buckets[j];
            if (b == 1)
            {
                addrs.push_back(_sortedIndexedAddrs[j]);
            }
        }

        return addrs;
    }

private:
    static vector<pair<int, string>> packageWithIndex(vector<string> addrs)
    {
        vector<pair<int, string>> packaged;

        for (auto i = 0; i < addrs.size(); ++i)
        {
            packaged.push_back({ i, addrs[i] });
        }

        return packaged;
    }

    static vector<pair<int, string>> sort(vector<pair<int, string>> indexedAddr)
    {
        sort(indexedAddr.begin(), indexedAddr.end(), [](auto& a, auto& b)
        {
            return a.second < b.second;
        });
        return indexedAddr;
    }

    int find(int rawIndex)
    {
        for (auto i = 0; i < _sortedIndexedAddrs.size(); ++i)
        {
            auto& item = _sortedIndexedAddrs[i];
            if (item.first == rawIndex)
            {
                return i;
            }
        }

        throw runtime_error("Not found");
    }
};

class Solution
{
private:
    enum class DataKind
    {
        Mail,
        User,
    };
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts)
    {
        vector<string> addrs;
        vector<pair<string, Dimension>> statisticTable;

        // Statistic
        for (auto& a : accounts)
        {
            auto dim = Dimension(addrs.size());

            for (auto i = 1; i < a.size(); ++i)
            {
                auto& addr = a[i];
                if (auto pos = findIn(addrs, addr))
                {
                    dim.setExistAt(pos.value());
                }
                else
                {
                    addrs.push_back(addr);
                    dim.setExistAt(addrs.size() - 1);
                }
            }

            statisticTable.push_back({ a[0], move(dim) });
        }

        return mergetIn(move(statisticTable), move(addrs));
    }

    optional<int> findIn(vector<string> const& addrList, string const& addr)
    {
        for (auto i = 0; i < addrList.size(); ++i)
        {
            auto& a = addrList[i];
            if (addr == a)
            {
                return i;
            }
        }

        return {};
    }

    vector<vector<string>> mergetIn(vector<pair<string, Dimension>> statisticTable, vector<string> addrs)
    {
        vector<vector<string>> mergedAccounts;
        auto query = AddrQuery(move(addrs));

        while (!statisticTable.empty())
        {
            vector<string> account{ statisticTable[0].first, };
            auto idxs = catchAllRelatedAddrIdx(statisticTable, 0);
            // remove duplicate
            sort(idxs.begin(), idxs.end());
            auto last = unique(idxs.begin(), idxs.end());
            idxs.earse(last, idxs.end());

            account.reserve(idxs.size());

            auto addrs = query.queryAddrsThenSort(idxs);
            account.insert(account.end(),
                make_move_iterator(addrs.begin()), 
                make_move_iterator(addrs.end()));
            mergedAccounts.push_back(move(account));
        }

        return mergedAccounts;
    }

    // catch related addr idxs and delete corresponding item in statisticTable
    vector<string> catchAllRelatedAddrIdx(vector<pair<string, Dimension>>& statisticTable, int accountIndex)
    {
        vector<int> addrs;
        
        auto addrStates = queryData<DataKind::Mail>(statisticTable, accountIndex);
        statisticTable.erase(statisticTable.begin() + accountIndex);

        for (auto i = 0; i < addrStates.size(); ++i)
        {
            auto state = addrStates[i];
            if (state == 1)
            {
                auto commonUsedIdxs = queryData<DataKind::User>(statisticTable, i);
                for (auto idx : commonUsedIdxs)
                {
                    auto relatedAddrs = catchAllRelatedAddrIdx(statisticTable, idx);
                    addrs.insert(addrs.end(), 
                        make_move_iterator(relatedAddrs.begin()),
                        make_move_iterator(relatedAddrs.end()));
                }
            }
        }

        return addrs;
    }

    template <DataKind Kind>
    vector<int> queryData(vector<pair<string, Dimension>> const& statisticTable, int accountIndex)
    {
        switch (Kind)
        {
        case DataKind::Mail:
            return statisticTable[accountIndex].second;

        case DataKind::User:
            {
                vector<int> commonUseIdxs;

                for (auto i = 0; i < statisticTable.size(); ++i)
                {
                    auto item = statisticTable[i];
                    if (item.second[accountIndex] == 1)
                    {
                        commonUseIdxs.push_back(i);
                    }
                }

                return commonUseIdxs;          
            }
    }
};