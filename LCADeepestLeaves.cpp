// https://leetcode-cn.com/problems/lowest-common-ancestor-of-deepest-leaves/
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
private:
    map<TreeNode*, TreeNode*> _nodeToParent;
public:
    TreeNode* lcaDeepestLeaves(TreeNode* root)
    {
        if (root == NULL)
        {
            return NULL;
        }

        vector<TreeNode*> nodes
        {
            root,
        };
        vector<TreeNode*> last;

        do
        {
            last = nodes;
            nodes = moveDeep(nodes);

            if (nodes.empty())
            {
                set<TreeNode*> parents(last.begin(), last.end());

                while (parents.size() > 1)
                {
                    parents = moveUp(parents);
                }

                return *parents.begin();
            } 
        } while (true);
    }

private:
    vector<TreeNode*> moveDeep(vector<TreeNode*> nodes)
    {
        vector<TreeNode*> subs;

        for (auto n : nodes)
        {
            if (auto l = n->left; l != NULL)
            {
                _nodeToParent.insert({ l, n });
                subs.push_back(l);
            }

            if (auto r = n->right; r != NULL)
            {
                _nodeToParent.insert({ r, n });
                subs.push_back(r);
            }
        }

        return subs;
    }

    set<TreeNode*> moveUp(set<TreeNode*> const& nodes)
    {
        set<TreeNode*> parents;

        for (auto& n : nodes)
        {
            parents.insert(_nodeToParent[n]);
        }

        return parents;
    }
};