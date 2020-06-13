// https://leetcode-cn.com/problems/symmetric-tree/
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
    bool isSymmetric(TreeNode* root)
    {
        if (root == NULL)
        {
            return true;
        }

        vector<TreeNode*> nodes{ root, };

        while (true)
        {
            nodes = moveDeep(nodes);

            if (isAllNull(nodes))
            {
                return true;
            }
            else if (!isSymmetric(nodes))
            {
                return false;
            }
        }
    }

    static vector<TreeNode*> moveDeep(vector<TreeNode*> const& nodes)
    {
        vector<TreeNode*> subs;

        for (auto n : nodes)
        {
            if (n == NULL)
            {
                continue;
            }

            subs.push_back(n->left);
            subs.push_back(n->right);
        }

        return subs;
    }

    static bool isSymmetric(vector<TreeNode*> const& nodes)
    {
        if (nodes.size() % 2 != 0)
        {
            return false;
        }

        for (auto i = 0; i < (nodes.size() / 2); ++i)
        {
            auto left = nodes[i];
            auto right = nodes[nodes.size() - 1 - i];

            auto leftNull = false;
            if (left == NULL)
            {
                leftNull = true;
            }

            auto rightNull = false;
            if (right == NULL)
            {
                rightNull = true;
            }

            if (leftNull != rightNull)
            {
                return false;
            }
            else if (!leftNull) // because equal, rightNull is not NULL, too
            {
                auto leftValue = left->val;
                auto rightValue = right->val;
                if (leftValue != rightValue)
                {
                    return false;
                }
            }
        }

        return true;
    }

    static bool isAllNull(vector<TreeNode*> const& nodes)
    {
        for (auto n : nodes)
        {
            if (n != NULL)
            {
                return false;
            }
        }

        return true;
    }
};