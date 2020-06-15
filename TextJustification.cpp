// https://leetcode-cn.com/problems/text-justification/
class TextUnit
{
private:
    vector<string> const& _wordsSrc;
    int _i;
    int _tailSpaceCount = 1;

public:
    TextUnit(int i, vector<string> const& wordsSrc) : _wordsSrc(wordsSrc), _i(i)
    { }

    int size() const
    {
        return _tailSpaceCount + _wordsSrc[_i].size();
    }

    void addSpace(int n)
    {
        _tailSpaceCount += n;
    }

    void removeSpace()
    {
        _tailSpaceCount = 0;
    }

    string toString() const
    {
        auto s = _wordsSrc[_i];
        auto tail = string(_tailSpaceCount, ' ');
        s.insert(s.end(), tail.begin(), tail.end());

        return s;
    }
};

class WordsCombinator
{
private:
    vector<string> const& _wordsSrc;
    int _unitMaxWidth;
    vector<TextUnit> _selecteds;
public:
    WordsCombinator(vector<string> const& wordsSrc, int unitMaxWidth)
        : _wordsSrc(wordsSrc), _unitMaxWidth(unitMaxWidth)
    { }

    bool tryAdd(int i)
    {
        if (auto size = countSize(); size + _wordsSrc[i].size() > _unitMaxWidth)
        {
            return false;
        }
        else
        {
            _selecteds.push_back({ i, _wordsSrc });
            return true;
        }
    }

    string genThenClearRecord()
    {
        _selecteds.back().removeSpace();
        auto s = countSize();
        auto lackSize = _unitMaxWidth - s;
        if (lackSize > 0)
        {
            auto n = _selecteds.size();
            auto spaceNums = distributeSpaces(lackSize, n);

            for (auto i = 0; i < n; ++i)
            {
                auto& unit = _selecteds[i];
                auto n = spaceNums[i];
                unit.addSpace(n);
            }
        }
        
        return toStringThenClear();
    }

    string genLastLineThenClear()
    {
        _selecteds.back().removeSpace();
        auto s = countSize();
        if (auto lackSize = _unitMaxWidth - s; lackSize > 0)
        {
            _selecteds.back().addSpace(lackSize);
        }

        return toStringThenClear();
    }
 
    bool empty() const
    {
        return _selecteds.empty();
    }

private:
    int countSize() const
    {
        auto n = 0;
        for (auto& i : _selecteds)
        {
            n += i.size();
        }

        return n;
    }

    string toStringThenClear()
    {
        string str;
        for (auto &i : _selecteds)
        {
            auto sub = i.toString();
            str.insert(str.end(), sub.begin(), sub.end());
        }

        _selecteds.clear();
        return str;
    }

    static vector<int> distributeSpaces(int spaceNum, int partNum)
    {
        if (partNum == 1)
        {
            return { spaceNum, };
        }

        auto involvedN = partNum - 1;
        auto aver = spaceNum / involvedN;
        vector<int> parts(partNum, aver);
        parts.back() = 0;

        if (auto remain = spaceNum % involvedN; remain != 0)
        {
            for (auto i = 0; i < remain; ++i)
            {
                ++parts[i];
            }
        }

        return parts;
    }
};

class Solution
{
public:
    vector<string> fullJustify(vector<string>& words, int maxWidth)
    {
        vector<string> justified;
        WordsCombinator c(words, maxWidth);

        for (auto i = 0; i < words.size(); ++i)
        {
            if (!c.tryAdd(i))
            {
                justified.push_back(c.genThenClearRecord());
                c.tryAdd(i);
            }

            if (i == words.size() - 1)
            {
                if (!c.empty())
                {
                    justified.push_back(c.genLastLineThenClear());
                }
            }
        }

        return justified;
    }
};