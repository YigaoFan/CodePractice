// Recursive way to solve
// https://leetcode-cn.com/problems/burst-balloons/
package com.company;

import java.util.ArrayList;

/**
 * Created by LaoFan on 5/28/20.
 */
public class MaxCoins {
    public static int maxCoins(int[] nums) {
        Permutation p = Permutation.MakePermutation(nums.length);

        int coins = 0;
        while (p.moveNext()) {
            ArrayList<Integer> numList = new ArrayList<Integer>(nums.length);
            for (int i : nums) {
                numList.add(i);
            }

            int current = computeCoin(numList, p.current(), 1, 0);
            coins = coins > current ? coins : current;
        }

        return coins;
    }

    private static int computeCoin(ArrayList<Integer> nums, ArrayList<Integer> mulOrder, int currentOrder, int coinCount) {
        if (nums.isEmpty()) {
            return coinCount;
        }

        int pos = mulOrder.indexOf(currentOrder);

        int middle = nums.get(pos);
        int left = 1;
        if (pos != 0) {
            left = nums.get(pos - 1);
        }

        int right = 1;
        if (pos != nums.size() - 1) {
            right = nums.get(pos + 1);
        }

        coinCount += left * middle * right;
        mulOrder.remove(pos);
        nums.remove(pos);

        return computeCoin(nums, mulOrder, currentOrder + 1, coinCount);
    }
}
