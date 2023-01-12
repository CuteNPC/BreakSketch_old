#include "trace.h"
#include <bits/stdc++.h>
#include "includes/Dataprocess.h"
#include "includes/params.h"
#include "includes/params.h"
// #include "measure.h"
#include "paint.h"

using namespace std;

static int readnum = -1;  // 读取包总数
static int read_flow = 0; // 读取流总数，cell的总数是流总数的2-3倍(取3倍)，所以该变量用于控制各Sketch的构造，作为参数传递！

vector<pair<uint32_t, int>> input;
vector<pair<int, uint32_t>> gt; // 包个数，流id
vector<Packet> dataset;

int main(int argc, char *argv[])
{
    // 运行多次，在不同包数上测试
    /*for (readnum = 3000000; readnum < TOTAL_PACKETS; readnum += 3000000)
    {
        srand(RANDOM_SEED);
        input = loadCAIDA18(readnum);
        gt = groundtruth(input, read_flow, readnum);
        dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);

        measureF1_toCSV(dataset, read_flow, readnum); // 将结果输出到 break/codes/res 文件夹下的各个txt文件中，便于画图
        measureTime_toCSV(dataset, read_flow, readnum);

        // measureF1_toFile(dataset, read_flow, readnum); 将结果输出到 break/codes 即当前文件夹下的txt文件中，在头文件measure.h中定义(与paint.h冲突，请注释掉头文件中的paint.h)
        // measureTime_toFile(dataset, read_flow, readnum);
    }*/

    input = loadCAIDA18(readnum);
    gt = groundtruth(input, read_flow, readnum);
    dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB, readnum);
    readnum = dataset.size();
    // 运行多次，在不同内存上测试

    for (uint64_t i = 10, totmemory = 3 * 1024; totmemory <= 48 * 1024 * 1024; i += 2, totmemory *= 4)
    {
        srand(RANDOM_SEED);
        measureF1_toCSV_(dataset, totmemory, totmemory / 4, i); // 将结果输出到 break/codes/res 文件夹下的各个txt文件中，便于画图
        // measureTime_toCSV(dataset, testmemory[i], i + 1);

        // measureF1_toFile(dataset, read_flow, readnum); 将结果输出到 break/codes 即当前文件夹下的txt文件中，在头文件measure.h中定义(与paint.h冲突，请注释掉头文件中的paint.h)
        // measureTime_toFile(dataset, read_flow, readnum);
    }
    
    for (int i = 1; i <= 15; i++)
    {
        srand(RANDOM_SEED);
        measureBFF1_toCSV_(dataset, 3 * 1024 * 64, 1024 * 64 * 3 * i / 16, i); // 将结果输出到 break/codes/res 文件夹下的各个txt文件中，便于画图
        // measureTime_toCSV(dataset, testmemory[i], i + 1);

        // measureF1_toFile(dataset, read_flow, readnum); 将结果输出到 break/codes 即当前文件夹下的txt文件中，在头文件measure.h中定义(与paint.h冲突，请注释掉头文件中的paint.h)
        // measureTime_toFile(dataset, read_flow, readnum);
    }
    
    // 只运行一次
    /*
    srand(RANDOM_SEED);
    input = loadCAIDA18(readnum);
    gt = groundtruth(input, read_flow, readnum);
    dataset = Addseq_and_LossPacket(input, getFlowLargerThan256(gt), LOSS_PROB , readnum);

    measureF1(dataset, read_flow); //测试单个Sketch的F1score，详见函数定义（在头文件measure.h中定义(与paint.h冲突，请注释掉paint.h)
    measureTime(dataset, read_flow, 4); //测试单个或所有Sketch的时间，详见函数定义
    */

    return 0;
}
