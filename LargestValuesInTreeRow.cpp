// https://leetcode-cn.com/problems/find-largest-value-in-each-tree-row/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

class Solution
{
public:
    vector<int> largestValues(TreeNode* root)
    {
        if (root == NULL)
        {
            return { };
        }

        vector<int> rowMaxValues;
        vector<TreeNode*> nodes{ root, };

        do
        {
            vector<int> currentRowValues;

            for (auto n : nodes)
            {
                currentRowValues.push_back(n->val);
            }

            rowMaxValues.push_back(maxValueIn(currentRowValues));

            nodes = moveDeep(nodes);
        } while (!nodes.empty());

        return rowMaxValues;
    }

private:
    static vector<TreeNode*> moveDeep(vector<TreeNode*> nodes)
    {
        vector<TreeNode*> subs;

        for (auto n : nodes)
        {
            if (auto l = n->left; l != NULL)
            {
                subs.push_back(l);
            }

            if (auto r = n->right; r != NULL)
            {
                subs.push_back(r);
            }
        }

        return subs;
    }

    static int maxValueIn(vector<int> const& v)
    {
        auto max = INT_MIN;
        for (auto i : v)
        {
            max = max > i ? max : i;
        }

        return max;
    }
};