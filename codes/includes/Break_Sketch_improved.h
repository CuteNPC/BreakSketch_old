#ifndef _BREAK_SKETCH_IMPROVED_H
#define _BREAK_SKETCH_IMPROVED_H

#include <bits/stdc++.h>
#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include "simd.h"
#include "Packet.h"
#include "Break_Sketch.h"
#define REPORT_TIME_BREAK_SKETCH_IMPROVED 0

using namespace std;
class Break_Sketch_improved : public Break_Sketch
{
public:
    SIMD_Bucket_4_32 *bucket;
    int size;
    BOBHash *hash;
    BloomFilter *BF;

public:
    Break_Sketch_improved(int memory, int BFmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        size = (memory - BFmemory) / 16;
        bucket = new SIMD_Bucket_4_32[size];
        hash = new BOBHash(hash_seed);
        BF = new BloomFilter(BFmemory);
    }
#if REPORT_TIME_BREAK_SKETCH_IMPROVED
    bool Solution(const Packet &packet)
    {
        static long long tm0 = 0, tm1 = 0, tm2 = 0, tm3 = 0;
        static int cnt = 0;
        clock_t first, second;

        first = clock();
        second = clock();
        tm0 += second - first;

        first = clock();
        int index = hash->run((char *)&packet.id, sizeof(packet.id)) % size;
        second = clock();
        tm1 += second - first;

        first = clock();
        int bucket_res = bucket[index].Insert(packet.seq);
        second = clock();
        tm2 += second - first;

        first = clock();
        if (!BF->Query((char *)&packet.id))
        {
            BF->Insert((char *)&packet.id);
            bucket_res = false;
        }
        second = clock();
        tm3 += second - first;

        if ((++cnt) % 100000 == 0)
        {
            double sum = tm1 - tm0 + tm2 - tm0 + tm3 - tm0;
            cout << "improve: " << fixed << setprecision(4) << (tm1 - tm0) / sum << ' ' << (tm2 - tm0) / sum << ' ' << (tm3 - tm0) / sum << endl;
            cout << "improve: " << fixed << setprecision(4) << tm1 - tm0 << ' ' << tm2 - tm0 << ' ' << tm3 - tm0 << endl;
        }
        return bucket_res == 1;
    }
#else
    bool Solution(const Packet &packet) // 返回是否发生断流
    {
        int index = hash->run((char *)&packet.id, sizeof(packet.id)) % size;
        int bucket_res = bucket[index].Insert(packet.seq);

        if (!BF->Query((char *)&packet.id))
        {
            BF->Insert((char *)&packet.id);
            return false;
        }

        return bucket_res == 1;
    }
#endif
    string Name()
    {
        return string("Break_Sketch_improved");
    }
    ~Break_Sketch_improved()
    {
        delete hash;
        delete BF;
        delete[] bucket;
    }
};

#endif