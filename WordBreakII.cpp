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
                        matrix.SetAt(p, j, s.size());
                        foundPos = p;
                    }
                    else
                    {
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
                    for (auto& s : optionsContent[remainIndex])
                    {
                        optionContent.push_back(wordDict[idx] + " " + s);
                    }
                }
            }
            
            optionsContent[i] = move(optionContent);
        }

        return optionsContent[0];
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
};