// https : //leetcode-cn.com/problems/accounts-merge/submissions/
class Solution
{
public:
    vector<vector<string>> accountsMerge(vector<vector<string>> &accounts)
    {
        // 这种方法合并的时候有点问题，可能还是要换用类似二维数组那种方法
        vector<vector<int>> twoDim;
        map<int, vector<string>> idAccountMap;
        auto id = 0;
        for (auto &a : accounts)
        {
            if (!tryAppendIfHasSameMails(a, idAccountMap))
            {
                auto p = idAccountMap.insert({id++, {a[0]}});
                combineTwoAccount(&p.first->second, a);
            }
        }

        vector<vector<string>> combinedAccounts;
        for (auto &p : idAccountMap)
        {
            auto &a = p.second;
            sort(++a.begin(), a.end());
            combinedAccounts.push_back(a);
        }

        return combinedAccounts;
    }

    bool tryAppendIfHasSameMails(vector<string> &account, map<int, vector<string>> &idAccountMap)
    {
        for (auto i = 1; i < account.size(); ++i)
        {
            auto &mail = account[i];
            if (auto pos = findSameMail(mail, idAccountMap))
            {
                combineTwoAccount(pos.value(), account);
                return true;
            }
        }

        return false;
    }

    optional<vector<string> *> findSameMail(string const &mail, map<int, vector<string>> &idAccountMap)
    {
        for (auto &item : idAccountMap)
        {
            auto &accounts = item.second;
            for (auto i = 1; i < accounts.size(); ++i)
            {
                auto &m = accounts[i];
                if (m == mail)
                {
                    return &accounts;
                }
            }
        }

        return {};
    }

    void combineTwoAccount(vector<string> *pos, vector<string> &account)
    {
        auto has = [pos = pos](auto &a) {
            for (auto j = 1; j < pos->size(); ++j)
            {
                auto &existedAccount = (*pos)[j];
                if (existedAccount == a)
                {
                    return true;
                }
            }

            return false;
        };

        for (auto i = 1; i < account.size(); ++i)
        {
            auto &a = account[i];
            if (!has(a))
            {
                pos->push_back(a);
            }
        }
    }
};