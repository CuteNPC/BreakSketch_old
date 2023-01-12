#ifndef _COUNT_BREAK_H
#define _COUNT_BREAK_H

#include <bits/stdc++.h>
#include "params.h"
#include "Packet.h"
using namespace std;

// 精确计数断流，用来计算正确率 F1score
class Count_Break
{
public:
    int t;
    int total_break;
    int total_packet;
    map<uint32_t, Seq> cnt;

    Count_Break()
    {
        t = 0;
        total_break = 0;
        total_packet = 0;
    }

    int Solution(Packet elem)
    {
        int res = 0;
        ++total_packet;
        if (cnt.count(elem.id) && (elem.seq - cnt[elem.id] > (uint32_t)3))
        {
            ++total_break;
            res = 1;
        }
        cnt[elem.id] = elem.seq;
        return res;
    }

    void PrintInfo()
    {
        cout << "total_packets=" << total_packet << endl;
        cout << "total_break=" << total_break << endl;
    }
};

#endif