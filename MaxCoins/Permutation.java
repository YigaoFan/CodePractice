package com.company;

import java.util.ArrayList;

/**
 * Created by LaoFan on 5/30/20.
 */
public class Permutation {
    private boolean m_firstMove = false;
    private int m_optionIndex;
    private ArrayList<Integer> m_options;
    private Permutation m_subPermutation;

    public static Permutation MakePermutation(int num) {
        ArrayList options = new ArrayList<Boolean>(num);
        for (int i = 1; i <= num; ++i) {
            options.add(i);
        }

        Permutation p = new Permutation(options);
        p.m_firstMove = true;
        return p;
    }

    private Permutation(ArrayList<Integer> options) {
        m_options = options;
        for (int i = 0; i < options.size(); ++i) {
            m_optionIndex = i;

            if (options.size() > 1) {
                m_subPermutation = new Permutation(regenSubOpt(m_optionIndex));
                return;
            }
        }
    }

    public boolean moveNext() {
        if (m_options.isEmpty()) {
            return false;
        }

        if (m_firstMove) {
            m_firstMove = false;
            return true;
        }

        if (m_subPermutation == null) {
            return false;
        }

        if (m_subPermutation.moveNext()) {
            return true;
        }

        if ((++m_optionIndex) < m_options.size()) {
            m_subPermutation = new Permutation(regenSubOpt(m_optionIndex));
            return true;
        }

        return false;

    }

    public ArrayList<Integer> current() {
        ArrayList a;

        if (m_subPermutation == null) {
            a = new ArrayList<>();
            a.add(m_options.get(m_optionIndex));
        } else {
            a = new ArrayList<>(m_subPermutation.current());
            a.add(0, m_options.get(m_optionIndex));
        }

        return a;
    }

    private ArrayList<Integer> regenSubOpt(int newMyOptIndex) {
        ArrayList<Integer> subOpt = (ArrayList<Integer>) m_options.clone();
        subOpt.remove(newMyOptIndex);
        return subOpt;
    }
}
