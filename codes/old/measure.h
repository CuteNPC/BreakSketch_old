#ifndef _MEASURE_H_
#define _MEASURE_H_

#include <bits/stdc++.h>
#include "includes/params.h"
#include "includes/Count_Break.h"
#include "includes/Break_Sketch.h"
#include "includes/Break_Sketch_straw.h"
#include "includes/Break_Sketch_basic.h"
#include "includes/Break_Sketch_improved_old.h"
#include "includes/Break_Sketch_improved.h"
#include "includes/Break_Sketch_Optimization_1.h"
#include "includes/Break_Sketch_Optimization_2.h"
#include <time.h>

static Break_Sketch_straw *break_sketch_straw;
static Break_Sketch_basic *break_sketch_basic;
static Break_Sketch_improved_old *break_sketch_improved_old;
static Break_Sketch_improved *break_sketch_improved;
static Break_Sketch_Optimization_1 *break_sketch_optimization_1;
static Break_Sketch_Optimization_2 *break_sketch_optimization_2;

static ofstream fout;

void create_sketch(int memory)
{
    break_sketch_straw = new Break_Sketch_straw(memory);
    break_sketch_basic = new Break_Sketch_basic(memory);
    break_sketch_improved_old = new Break_Sketch_improved_old(memory);
    break_sketch_improved = new Break_Sketch_improved(memory);
    break_sketch_optimization_1 = new Break_Sketch_Optimization_1(memory);
    break_sketch_optimization_2 = new Break_Sketch_Optimization_2(memory);
}

void free_Sketch()
{
    delete break_sketch_straw;
    delete break_sketch_basic;
    delete break_sketch_improved_old;
    delete break_sketch_improved;
    delete break_sketch_optimization_1;
    delete break_sketch_optimization_2;
}

// 最开始的简单测试，输出到终端
// 测试F1score，输出到终端
void measureF1(vector<Packet> &dataset, int read_flow)
{
    create_sketch(read_flow);

    Count_Break CB;
    uint32_t correct_report = 0, wrong_report = 0, loss_report = 0, normal = 0;

    for (Packet packet : dataset)
    {
        // bool Sketch_res = break_sketch_straw->Solution(packet);
        // bool Sketch_res = break_sketch_basic->Solution(packet);
        // bool Sketch_res = break_sketch_improved_old->Solution(packet);
        bool Sketch_res = break_sketch_improved->Solution(packet);
        // bool Sketch_res = break_sketch_optimization_1->Solution(packet);
        // bool Sketch_res = break_sketch_optimization_2->Solution(packet);
        bool C_res = CB.Solution(packet);
        if (Sketch_res && C_res) // 都报告断流，正确
            correct_report++;

        else if (Sketch_res && !C_res) // 错报
            wrong_report++;

        else if (!Sketch_res && C_res) // 漏报
            loss_report++;

        else
            normal++;
    }

    cout << "correct_report=" << correct_report << "; wrong_report=" << wrong_report << "; loss_report=" << loss_report << "; normal=" << normal << endl;
    double PR = (double)correct_report / ((double)wrong_report + correct_report);
    double RR = (double)correct_report / ((double)loss_report + correct_report);
    double F1score = (2 * PR * RR) / (PR + RR);
    cout << "F1score=" << F1score << endl;
    // CB.PrintInfo();

    free_Sketch();
}

// 测试时间，输出到终端
void measureTime(vector<Packet> &dataset, int read_flow, int index = -1)
{
    create_sketch(read_flow);

    clock_t start, finish;
    double duration;
    start = clock();
    switch (index)
    {
    case 1:
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_straw->Solution(packet);
        break;
    case 2:
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_basic->Solution(packet);
        break;
    case 3:
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_improved_old->Solution(packet);
        break;
    case 4:
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_improved->Solution(packet);
        break;
    case 5:
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_optimization_1->Solution(packet);
        break;
    case 6:
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_optimization_2->Solution(packet);
        break;
    default:
        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_straw->Solution(packet);
        finish = clock();
        cout << "strawman time_cost=" << ((double)finish - start) / CLOCKS_PER_SEC << endl;
        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_basic->Solution(packet);
        finish = clock();
        cout << "basic time_cost=" << ((double)finish - start) / CLOCKS_PER_SEC << endl;
        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_improved_old->Solution(packet);
        finish = clock();
        cout << "improved_old time_cost=" << ((double)finish - start) / CLOCKS_PER_SEC << endl;
        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_improved->Solution(packet);
        finish = clock();
        cout << "improved time_cost=" << ((double)finish - start) / CLOCKS_PER_SEC << endl;
        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_optimization_1->Solution(packet);
        finish = clock();
        cout << "optimization_1 time_cost=" << ((double)finish - start) / CLOCKS_PER_SEC << endl;
        start = clock();
        for (Packet packet : dataset)
            bool Sketch_res = break_sketch_optimization_2->Solution(packet);
        finish = clock();
        cout << "optimization_2 time_cost=" << ((double)finish - start) / CLOCKS_PER_SEC << endl;
        break;
    }
    finish = clock();
    duration = ((double)finish - start) / CLOCKS_PER_SEC;
    cout << "time_cost=" << duration << "s" << endl;

    free_Sketch();
}

// 以下为 measureF1_toFile的辅助函数
void straw_F1(bool Sketch_res, bool C_res, int flag = 0)
{
    static uint32_t correct_report, wrong_report, loss_report;
    if (Sketch_res && C_res)
        correct_report++;
    else if (Sketch_res && !C_res)
        wrong_report++;
    else if (!Sketch_res && C_res)
        loss_report++;

    if (flag)
    {
        double PR = (double)correct_report / ((double)wrong_report + correct_report);
        double RR = (double)correct_report / ((double)loss_report + correct_report);
        double F1score = (2 * PR * RR) / (PR + RR);
        fout << "strawman: " << F1score << endl;
    }
}

void basic_F1(bool Sketch_res, bool C_res, int flag = 0)
{
    static uint32_t correct_report, wrong_report, loss_report;
    if (Sketch_res && C_res)
        correct_report++;
    else if (Sketch_res && !C_res)
        wrong_report++;
    else if (!Sketch_res && C_res)
        loss_report++;

    if (flag)
    {
        double PR = (double)correct_report / ((double)wrong_report + correct_report);
        double RR = (double)correct_report / ((double)loss_report + correct_report);
        double F1score = (2 * PR * RR) / (PR + RR);
        fout << "basic: " << F1score << endl;
    }
}

void improved_old_F1(bool Sketch_res, bool C_res, int flag = 0)
{
    static uint32_t correct_report, wrong_report, loss_report;
    if (Sketch_res && C_res)
        correct_report++;
    else if (Sketch_res && !C_res)
        wrong_report++;
    else if (!Sketch_res && C_res)
        loss_report++;

    if (flag)
    {
        double PR = (double)correct_report / ((double)wrong_report + correct_report);
        double RR = (double)correct_report / ((double)loss_report + correct_report);
        double F1score = (2 * PR * RR) / (PR + RR);
        fout << "improved_old: " << F1score << endl;
    }
}

void improved_F1(bool Sketch_res, bool C_res, int flag = 0)
{
    static uint32_t correct_report, wrong_report, loss_report;
    if (Sketch_res && C_res)
        correct_report++;
    else if (Sketch_res && !C_res)
        wrong_report++;
    else if (!Sketch_res && C_res)
        loss_report++;

    if (flag)
    {
        double PR = (double)correct_report / ((double)wrong_report + correct_report);
        double RR = (double)correct_report / ((double)loss_report + correct_report);
        double F1score = (2 * PR * RR) / (PR + RR);
        fout << "improved: " << F1score << endl;
    }
}

void Optimization1_F1(bool Sketch_res, bool C_res, int flag = 0)
{
    static uint32_t correct_report, wrong_report, loss_report;
    if (Sketch_res && C_res)
        correct_report++;
    else if (Sketch_res && !C_res)
        wrong_report++;
    else if (!Sketch_res && C_res)
        loss_report++;

    if (flag)
    {
        double PR = (double)correct_report / ((double)wrong_report + correct_report);
        double RR = (double)correct_report / ((double)loss_report + correct_report);
        double F1score = (2 * PR * RR) / (PR + RR);
        fout << "Optimization1: " << F1score << endl;
    }
}

void Optimization2_F1(bool Sketch_res, bool C_res, int flag = 0)
{
    static uint32_t correct_report, wrong_report, loss_report;
    if (Sketch_res && C_res)
        correct_report++;
    else if (Sketch_res && !C_res)
        wrong_report++;
    else if (!Sketch_res && C_res)
        loss_report++;

    if (flag)
    {
        double PR = (double)correct_report / ((double)wrong_report + correct_report);
        double RR = (double)correct_report / ((double)loss_report + correct_report);
        double F1score = (2 * PR * RR) / (PR + RR);
        fout << "Optimization2: " << F1score << endl;
    }
}

// 以上为 measureF1_toFile的辅助函数

// 测试F1，输出到文件F1score.txt
void measureF1_toFile(vector<Packet> &dataset, int read_flow, int read_packet)
{
    create_sketch(read_flow);

    fout.open("F1score.txt", ios_base::out | ios_base::app);
    fout << read_packet << ":" << endl;

    Count_Break CB;

    for (Packet packet : dataset)
    {
        bool C_res = CB.Solution(packet);
        straw_F1(break_sketch_straw->Solution(packet), C_res);
        basic_F1(break_sketch_basic->Solution(packet), C_res);
        improved_old_F1(break_sketch_improved_old->Solution(packet), C_res);
        improved_F1(break_sketch_improved->Solution(packet), C_res);
        Optimization1_F1(break_sketch_optimization_1->Solution(packet), C_res);
        Optimization2_F1(break_sketch_optimization_2->Solution(packet), C_res);
    }

    straw_F1(0, 0, 1);
    basic_F1(0, 0, 1);
    improved_old_F1(0, 0, 1);
    improved_F1(0, 0, 1);
    Optimization1_F1(0, 0, 1);
    Optimization2_F1(0, 0, 1);
    fout << endl;
    fout.close();
    free_Sketch();
}

// 测试时间，输出到文件Time.txt
void measureTime_toFile(vector<Packet> &dataset, int read_flow, int read_packet)
{
    create_sketch(read_flow);

    fout.open("Time.txt", ios_base::out | ios_base::app);
    fout << read_packet << ":" << endl;

    clock_t start, finish;
    double duration;
    start = clock();
    for (Packet packet : dataset)
        bool Sketch_res = break_sketch_straw->Solution(packet);
    finish = clock();
    fout << "strawman: " << ((double)finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (Packet packet : dataset)
        bool Sketch_res = break_sketch_basic->Solution(packet);
    finish = clock();
    fout << "basic: " << ((double)finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (Packet packet : dataset)
        bool Sketch_res = break_sketch_improved_old->Solution(packet);
    finish = clock();
    fout << "improved_old: " << ((double)finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (Packet packet : dataset)
        bool Sketch_res = break_sketch_improved->Solution(packet);
    finish = clock();
    fout << "improved: " << ((double)finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (Packet packet : dataset)
        bool Sketch_res = break_sketch_optimization_1->Solution(packet);
    finish = clock();
    fout << "optimization_1: " << ((double)finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (Packet packet : dataset)
        bool Sketch_res = break_sketch_optimization_2->Solution(packet);
    finish = clock();
    fout << "optimization_2: " << ((double)finish - start) / CLOCKS_PER_SEC << endl;

    fout << endl;
    fout.close();

    free_Sketch();
}

#endif