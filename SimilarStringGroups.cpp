// https://leetcode-cn.com/problems/similar-string-groups/
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

    T& operator()(unsigned rowIndex, unsigned colIndex)
    {
        return _data[rowIndex * _colCount + colIndex];
    }

    T const& operator()(unsigned rowIndex, unsigned colIndex) const
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
    int numSimilarGroups(vector<string>& A)
    {
        if (A.empty())
        {
            return 0;
        }

        Matrix<bool> matrix(A.size(), A.size());

        for (auto i = 0; i < A.size(); ++i)
        {
            matrix.SetAt(i, i, true);
        }

        for (auto i = 0; i < A.size(); ++i)
        {
            for (auto j = i + 1; j < A.size(); ++j)
            {
                if (isSimilar(A[i], A[j]))
                {
                    matrix.SetAt(i, j, true);
                }
                else
                {
                    matrix.SetAt(i, j, false);
                }                
            }
        }

        vector<set<int>> similarGroup;

        for (auto i = 0; i < A.size(); ++i)
        {
            auto idxs = queryAllSimilarIndex(i, matrix);
            auto groupIdxs = queryAllRelatedGroup(similarGroup, idxs);
            if (groupIdxs.empty())
            {
                similarGroup.push_back({ i, });
            }
            else 
            {
                auto firstGroupIdx = *groupIdxs.begin();
                similarGroup[firstGroupIdx].insert(i);
                if (groupIdxs.size() > 1)
                {
                    auto& merged = similarGroup[firstGroupIdx];

                    for (auto it = (++groupIdxs.begin()); it != groupIdxs.end(); ++it)
                    {
                        auto idx = *it;
                        merged.merge(move(similarGroup[idx]));
                    }

                    vector<int> toDelGroups(++groupIdxs.begin(), groupIdxs.end());
                    sort(toDelGroups.begin(), toDelGroups.end());
                    for (int j = toDelGroups.size() - 1; j >= 0; --j)
                    {
                        auto idx = toDelGroups[j];
                        similarGroup[idx] = move(similarGroup.back());
                        similarGroup.pop_back();
                    }
                }
            }
        }

        return similarGroup.size();
    }

    static bool isSimilar(string_view s1, string_view s2)
    {
        auto getFirstDiff = [](string_view s1, string_view s2)
        {
            for (int i = 0; i < s1.size(); ++i)
            {
                if (s1[i] != s2[i])
                {
                    return i;
                }
            }

            return -1;
        };

        auto getLastDiff = [](string_view s1, string_view s2)
        {
            for (int i = s1.size() - 1; i >= 0; --i)
            {
                if (s1[i] != s2[i])
                {
                    return i;
                }
            }

            return -1;
        };

        for (auto i = getFirstDiff(s1, s2) + 1; i < getLastDiff(s1, s2); ++i)
        {
            if (s1[i] != s2[i])
            {
                return false;
            }
        }

        return true;
    }

    vector<int> queryAllSimilarIndex(int i, Matrix<bool> const& matrix)
    {
        vector<int> similarIdxs;

        for (auto j = 0; j < i; ++j)
        {
            if (matrix(j, i))
            {
                similarIdxs.push_back(j);
            }
        }

        for (auto j = i + 1; j < matrix.colSize(); ++j)
        {
            if (matrix(i, j))
            {
                similarIdxs.push_back(j);
            }
        }

        return similarIdxs;
    }

    set<int> queryAllRelatedGroup(vector<set<int>> const& similarGroup, vector<int> const& idxs)
    {
        set<int> relatedGroupIdxs;

        for (auto i : idxs)
        {
            for (auto j = 0; j < similarGroup.size(); ++j)
            {
                auto& g = similarGroup[j];

                if (g.find(i) != g.end())
                {
                    relatedGroupIdxs.insert(j);
                }
            }
        }

        return relatedGroupIdxs;
    }
};