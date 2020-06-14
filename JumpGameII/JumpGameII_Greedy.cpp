// https://leetcode-cn.com/problems/jump-game-ii/
// 题目里说：你总是可以到达数组的最后一个位置，意思是你无论走到哪个位置最终都能到终点。
// 思路很棒，来自：https://leetcode-cn.com/problems/jump-game-ii/comments/6248
// 关于贪心为什么可以：
// 每次走最大的，你每次选择的范围就大，每次选择的范围都最大，那最后算出来的答案就是最少的步数。
// 来自：https://leetcode-cn.com/problems/jump-game-ii/solution/xiang-xi-tong-su-de-si-lu-fen-xi-duo-jie-fa-by-10/380574
class Solution
{
public:
    int jump(vector<int>& nums)
    {
        if (nums.size() == 1)
        {
            return 0;
        }

        int currentPos = 0;
        int nextPos = 0;
        int step = 0;
        for (auto i = 0; i < nums.size(); ++i)
        {
            // 下面这行和下面的第二个 if 表示，把当前这个位置到可以到的最远的位置
            // 这之间的所有的位置，所能到达的最远位置统计出来
            // 这个区间内无论选择在哪里停下，其实都是算一步
            // 很巧妙的思路。
            nextPos = max(nextPos, i + nums[i]);
            if (nextPos >= nums.size() - 1)
            {
                ++step;
                break;
            }

            if (i == currentPos)
            {
                currentPos = nextPos;
                ++step;
            }
        }

        return step;
    }
};