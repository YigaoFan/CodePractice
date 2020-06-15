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
        Matrix<bool> matrix(s.size(), wordDict.size());

        for (auto j = 0; j < wordDict.size(); ++j)
        {
            auto& w = wordDict[j];

            for (auto i = 0; i < s.size();) // set other position as false
            {
                auto p = s.find(w, i);
                if (p != string::npos)
                {
                    matrix.SetAt(p, j, true);
                    i = p + 1;
                }
                else
                {
                    break;
                }
            }
        }
        // 动态规划，背包问题？
    }
};