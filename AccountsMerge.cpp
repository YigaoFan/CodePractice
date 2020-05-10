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

    int operator[] (int index) const
    {
        if (index < _data.size())
        {
            return _data[index];
        }

        return 0;
    }

    auto size() { reutrn _data.size(); }
};

class Solution
{
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts)
    {
        vector<string> allMailAddr;
        vector<pair<string, Dimension>> statisticTable;

        // Statistic
        for (auto& a : accounts)
        {
            auto dim = Dimension(allMailAddr.size());

            for (auto i = 1; i < a.size(); ++i)
            {
                auto& addr = a[i];
                if (auto pos = findIn(allMailAddr, addr))
                {
                    dim.setExistAt(pos.value());
                }
                else
                {
                    allMailAddr.push_back(addr);
                    dim.setExistAt(allMailAddr.size() - 1);
                }
            }

            statisticTable.push_back({ a[0], move(dim) });
        }

        return mergetIn(move(statisticTable), move(allMailAddr));
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

    vector<vector<string>> mergetIn(vector<pair<string, Dimension>> statisticTable, vector<string> allMailAddr)
    {
        // Find in statisticTable to merge
        vector<vector<string>> mergedAccounts;
        
        auto name = statisticTable[0].first;
        vector<string> account{ name, };
        auto dim = statisticTable[0].second;
        for (auto i = 0; i < dim.size(); ++i)
        {
            auto exist = dim[i];
            if (exist)
            {
                auto& addr = allMailAddr[i];
                account.push_back(addr);
                auto dimIdxs = searchOtherDimUse(statisticTable, i, name);
                // TODO
                account.push_back();
            }
        }
    }

    vector<int> searchOtherDimUse(vector<pair<string, Dimension>> statisticTable, int existIndex, string const& name)
    {
        vector<int> commonUseDimIdxs;

        for (auto i = 0; i < statisticTable.size(); ++i)
        {
            auto item = statisticTable[i];
            if (item.first == name)
            {
                if (item.second[existIndex] == 1)
                {
                    commonUseDimIdxs.push_back(i);
                }
            }
        }

        return commonUseDimIdxs;
    }
};