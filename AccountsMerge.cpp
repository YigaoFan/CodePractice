// https : //leetcode-cn.com/problems/accounts-merge/submissions/
// 看一下别人的实现代码
class Dimension
{
private:
    vector<int> _data; // Item: 1 means exist, 0 means not exist
public:
    Dimension(size_t size) : _data(size, 0)
    {
    }

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

    int operator[](int accountIndex) const
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

    auto size() { return _data.size(); }
};

class AddrQuery
{
private:
    map<int, string> _addrMap;

public:
    AddrQuery(map<string, int> addrMap)
        : _addrMap(reverseMap(move(addrMap)))
    {
    }

    vector<string> queryAddrsThenSort(vector<int> indexs)
    {
        vector<string> addrs;

        for (auto i : indexs)
        {
            if (auto search = _addrMap.find(i); search != _addrMap.end())
            {
                addrs.push_back(search->second);
            }
            else
            {
                throw runtime_error("Not found");
            }
        }

        std::sort(addrs.begin(), addrs.end());
        return addrs;
    }

private:
    map<int, string> reverseMap(map<string, int> originalMap)
    {
        map<int, string> reversedMap;
        for (auto it = originalMap.begin(); it != originalMap.end(); ++it)
        {
            reversedMap[move(it->second)] = move(it->first);
        }

        return reversedMap;
    }
};

class Solution
{
public:
    vector<vector<string>> accountsMerge(vector<vector<string>> &accounts)
    {
        map<string, int> addrMap;
        vector<pair<string, Dimension>> statisticTable;

        // Statistic
        for (auto &a : accounts)
        {
            auto dim = Dimension(addrMap.size());

            for (auto i = 1; i < a.size(); ++i)
            {
                auto &addr = a[i];
                if (auto pos = findIn(addrMap, addr))
                {
                    dim.setExistAt(pos.value());
                }
                else
                {
                    addrMap.insert({addr, addrMap.size()});
                    dim.setExistAt(addrMap.size() - 1);
                }
            }

            statisticTable.push_back({a[0], move(dim)});
        }

        auto query = AddrQuery(move(addrMap));
        return mergetIn(move(statisticTable), move(query));
    }

    optional<int> findIn(map<string, int> const &addrMap, string const &addr)
    {
        if (auto search = addrMap.find(addr); search != addrMap.end())
        {
            return search->second;
        }

        return {};
    }

    vector<vector<string>> mergetIn(vector<pair<string, Dimension>> statisticTable, AddrQuery query)
    {
        vector<vector<string>> mergedAccounts;

        while (!statisticTable.empty())
        {
            vector<string> account{
                move(statisticTable[0].first),
            };
            auto addrStates = move(statisticTable[0].second);
            statisticTable[0] = statisticTable.back();
            statisticTable.pop_back();

            auto idxs = catchAllRelatedAddrIdx(statisticTable, move(addrStates));
            // remove duplicate
            sort(idxs.begin(), idxs.end());
            auto last = unique(idxs.begin(), idxs.end());
            idxs.erase(last, idxs.end());

            account.reserve(idxs.size() + 1);

            auto addrs = query.queryAddrsThenSort(idxs);
            account.insert(account.end(),
                           make_move_iterator(addrs.begin()),
                           make_move_iterator(addrs.end()));
            mergedAccounts.push_back(move(account));
        }

        return mergedAccounts;
    }

    // catch related addr idxs and delete corresponding item in statisticTable
    vector<int> catchAllRelatedAddrIdx(vector<pair<string, Dimension>> &statisticTable, vector<int> addrStates)
    {
        vector<int> allAddrs;

        for (auto i = 0; i < addrStates.size(); ++i)
        {
            auto state = addrStates[i];
            if (state == 1)
            {
                allAddrs.push_back(i);
                auto otherAccountsAddrs = queryData(statisticTable, i);
                for (auto &addrs : otherAccountsAddrs)
                {
                    addrs[i] = 0;
                    // 下面的 catchAllRelatedAddrIdx 里 addrs 可能包含别的 addrs 已经查过的 addr
                    // 这样就重复查了，尽管别的 addrs 查过后，将相关的 account 已经删掉了
                    // reply: 我在这里尝试合并 addrs 和各个 i 下的 addrs，结果 LeetCode 上显示却没有这版代码跑得快。内存占用差不多，小一点点。
                    // 我还发现 LeetCode 上注意引用传值，这样可以优化内存
                    auto relatedAddrs = catchAllRelatedAddrIdx(statisticTable, move(addrs));
                    allAddrs.insert(allAddrs.end(),
                                    make_move_iterator(relatedAddrs.begin()),
                                    make_move_iterator(relatedAddrs.end()));
                }
            }
        }

        return allAddrs;
    }

    vector<vector<int>> queryData(vector<pair<string, Dimension>> &statisticTable, int addrIndex, vector<vector<int>> lastQueryResult = {})
    {
        for (auto i = 0; i < statisticTable.size(); ++i)
        {
            auto &item = statisticTable[i];

            if (item.second[addrIndex] == 1)
            {
                lastQueryResult.push_back(move(item.second));
                statisticTable[i] = statisticTable.back();
                statisticTable.pop_back();
                return queryData(statisticTable, addrIndex, move(lastQueryResult));
            }
        }

        return lastQueryResult;
    }
};