// https://leetcode-cn.com/problems/word-break-ii/
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

    T& operator() (unsigned rowIndex, unsigned colIndex)
    {
        return _data[rowIndex * _colCount + colIndex];
    }

    void SetAt(unsigned rowIndex, unsigned colIndex, T value)
    {
        // bug: 如果一个地方已经被 set 过了，这里又被 set，前一个 set 的值就没有被析构
        auto addr = _data + (rowIndex * _colCount + colIndex);
        _alloc.construct(addr, move(value));
        _constructedAddrs.push_back(addr);
    }

    unsigned rowSize() const
    {
        return _rowCount;
    }

    unsigned colSize() const
    {
        return _colCount;
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
    vector<string> wordBreak(string s, vector<string>& wordDict)
    {
        auto chars = statisticAppearedChar(s);
        auto dicChars = statisticAppearedChar(wordDict);
        if (chars.size() > dicChars.size())
        {
            return { };
        }

        Matrix<int> matrix(s.size(), wordDict.size());
        
        for (auto j = 0; j < wordDict.size(); ++j)
        {
            auto& w = wordDict[j];

            for (auto i = 0, foundPos = -1; i < s.size(); ++i)
            {
                if (i > foundPos)
                {
                    if (auto p = s.find(w, i); p != string::npos)
                    {
                        if (i != p)
                        {
                            matrix.SetAt(i, j, 0);
                        }
                        matrix.SetAt(p, j, w.size());

                        foundPos = p;
                    }
                    else
                    {
                        matrix.SetAt(i, j, 0);
                        foundPos = s.size();
                    }
                }
                else if (i < foundPos)
                {
                    matrix.SetAt(i, j, 0);
                }
            }
        }

        vector<vector<string>> optionsContent(s.size() + 1, vector<string>());
        optionsContent[s.size()].push_back("");

        for (int i = s.size() - 1; i >= 0; --i)
        {
            auto words = queryWordStartFrom(i, matrix);
            vector<string> optionContent;
            
            for (auto& p : words)
            {
                auto idx = p.first;
                auto remainIndex = i + p.second;
                if (optionsContent[remainIndex].size() > 0)
                {
                    for (auto& c : optionsContent[remainIndex])
                    {
                        if (c.empty())
                        {
                            optionContent.push_back(wordDict[idx]);
                        }
                        else
                        {
                            optionContent.push_back(wordDict[idx] + " " + c);
                        }
                    }
                }
            }
            
            optionsContent[i] = move(optionContent);
        }

        auto& b = optionsContent[0];
        // 按字符串从末尾开始比较排下结果的顺序
        sort(b.begin(), b.end(), [](auto& s1, auto& s2)
        {
            for (auto it1 = s1.rbegin(), it2 = s2.rbegin(); it1 != s1.rend() && it2 != s2.rend(); ++it1, ++it2)
            {
                auto c1 = *it1;
                auto c2 = *it2;
                if (c1 != c2)
                {
                    return c1 < c2;
                }
            }

            return s1.size() < s2.size();
        });
        return b;
    }

    // pair: word index in WordDict, word len
    static vector<pair<int, int>> queryWordStartFrom(int i, Matrix<int>& wordMatrix)
    {
        vector<pair<int, int>> words;
        for (auto j = 0; j < wordMatrix.colSize(); ++j)
        {
            if (auto len = wordMatrix(i, j); len != 0)
            {
                words.push_back({ j, len });
            }
        }

        return words;
    }

    static set<char> statisticAppearedChar(string const& s)
    {
        set<char> appearedChar;
        for (auto c : s)
        {
            appearedChar.insert(c);
        }

        return appearedChar;
    }

    static set<char> statisticAppearedChar(vector<string> const& v)
    {
        set<char> appearedChar;
        for (auto& s : v)
        {
            appearedChar.merge(statisticAppearedChar(s));
        }

        return appearedChar;
    }
};