#ifndef _BREAK_SKETCH_IMPROVED_OLD_H
#define _BREAK_SKETCH_IMPROVED_OLD_H

#include "BOBHash.h"
#include "params.h"
#include "BloomFilter.h"
#include "Packet.h"
#include <string.h>
#include <iostream>
using namespace std;

class Break_Sketch_improved_old : public Break_Sketch
{
public:
    struct cell
    {
        uint32_t seq;
        int LRU_tag;
    };
    int w;          // w为bucket的个数，每个bucket里4个cell
    cell **buckets; // bucket的数组buckets
    BOBHash *hash;
    BloomFilter *BF;
    int index;

public:
    Break_Sketch_improved_old(int memory, int BFmemory, int hash_seed = 1000)
        : Break_Sketch(memory)
    {
        w = (memory - BFmemory) / 16;

        buckets = new cell *[w];
        for (int i = 0; i < w; i++)
        {
            buckets[i] = new cell[4];
            memset(buckets[i], 0, sizeof(cell) * 4);
        }

        hash = new BOBHash(hash_seed);
        BF = new BloomFilter(BFmemory);
    }

    void Insert(Packet packet)
    {
        index = (hash->run((const char *)&packet.id, KEY_LEN)) % w;
        int empty = 0;
        int min_i, min_LRUtag = 6; // LRUtag不会比5更大
        for (int i = 0; i < 4; i++)
        {
            if (buckets[index][i].seq == 0)
            { // 空的
                if (!empty)
                {
                    buckets[index][i].seq = packet.seq;
                    buckets[index][i].LRU_tag = 5;
                    empty = 1;
                }
            }
            else
            { // 非空
                buckets[index][i].LRU_tag--;
                if (buckets[index][i].LRU_tag < min_LRUtag)
                {
                    min_LRUtag = buckets[index][i].LRU_tag;
                    min_i = i;
                }
            }
        }
        if (!empty)
        {
            buckets[index][min_i].seq = packet.seq;
            buckets[index][min_i].LRU_tag = 5;
        }
    }

    bool Solution(Packet packet) // 返回是否发生断流
    {
        if (!BF->Query((const char *)&packet.id)) // 如果BF判断是新流
        {
            BF->Insert((const char *)&packet.id);
            this->Insert(packet);
            return false;
        }
        else // 如果是旧流
        {
            index = (hash->run((const char *)&packet.id, KEY_LEN)) % w;
            int min_i;
            uint32_t min_delta = 0xffffffff;
            for (int i = 0; i < 4; i++)
            {
                buckets[index][i].LRU_tag--;                                           // 无论是否为空，都把tag--（空的--无所谓）
                if (buckets[index][i].seq == 0 || buckets[index][i].seq >= packet.seq) // 空桶 或序列号大于等于seq 则跳过
                    continue;
                uint32_t delta = packet.seq - buckets[index][i].seq;
                if (delta < min_delta)
                {
                    min_delta = delta;
                    min_i = i;
                }
            }

            if (min_delta < 50) // 找到seq，更新对应seq，以及非空的tag
            {
                buckets[index][min_i].seq = packet.seq;
                buckets[index][min_i].LRU_tag = 5;
                if (min_delta <= 3)
                    return false;
                else
                    return true;
            }
            else // 发生断流，且丢失了原流
            {
                this->Insert(packet);
                return true;
            }
        }
    }

    ~Break_Sketch_improved_old()
    {
        for (int i = 0; i < w; i++)
        {
            delete[] buckets[i];
        }
        delete[] buckets;
        delete hash;
        delete BF;
    }
};

#endif