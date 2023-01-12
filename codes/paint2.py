#!/usr/bin/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt


# 作 相同输入、不同内存大小 下的F1图
def paint():
    F1csv = pd.read_csv(u'./codes/res/F1.csv')

    plt.title("F1score of Sketch")
    plt.grid(axis='y')
    plt.xlabel("log_2(memory/102)", loc='center')
    plt.ylabel("F1", loc='top')

    plt.plot(F1csv['X'], F1csv['Break_Sketch_straw'], label='strawman',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_basic'], label='basic',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_improved'], label='improved',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_1'], label='Op1',
             linestyle='-', marker='*', markersize=10)
    plt.plot(F1csv['X'], F1csv['Break_Sketch_Optimization_2'], label='Op2',
             linestyle='-', marker='*', markersize=10)

    plt.legend()

    plt.savefig('./codes/res/F1score.png')
    plt.show()
    plt.cla()


def main():
    paint()


main()
