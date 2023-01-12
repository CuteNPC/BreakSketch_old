#include <bits/stdc++.h>
#include "includes/TestAcc.h"
#include "includes/DataProcess.h"
#include "includes/Packet.h"
#include "includes/Break_Sketch_straw.h"
#include "includes/Break_Sketch_basic.h"
#include "includes/Break_Sketch_improved.h"
#include "includes/Break_Sketch_Optimization_1.h"
#include "includes/Break_Sketch_Optimization_2.h"
using namespace std;
#define TESTSIZE 10
int memarray[] = {96, 192, 384, 768, 1536, 3 * 1024, 6 * 1024, 12 * 1024, 24 * 1024, 48 * 1024};

vector<Packet> dataset;
int main()
{
    Load(dataset, "../data/loss_data.txt");
    ofstream F1out("res/F1.csv", ios::trunc | ios::out);
    char filename_str[128];
    F1out << "X,Break_Sketch_straw,Break_Sketch_basic,Break_Sketch_improved,Break_Sketch_Optimization_1,Break_Sketch_Optimization_2" << endl;
    for (int j = 0; j < TESTSIZE; j++)
    {
        int mem = memarray[j];
        int BFmem = mem / 16;
        mem += BFmem;
        F1out << j;
        Break_Sketch *break_sketch[6];
        break_sketch[1] = new Break_Sketch_straw(mem);
        break_sketch[2] = new Break_Sketch_basic(mem, BFmem);
        break_sketch[3] = new Break_Sketch_improved(mem, BFmem);
        break_sketch[4] = new Break_Sketch_Optimization_1(mem, BFmem);
        break_sketch[5] = new Break_Sketch_Optimization_2(mem, BFmem);

        for (int i = 1; i <= 5; i++)
        {
            sprintf(filename_str, "%s-%d", break_sketch[i]->Name().c_str(), mem);
            GetOutput(break_sketch[i], dataset, filename_str);
            delete break_sketch[i];
            Acc acc = CompareOutput(string("output/") + filename_str + string(".txt"), "output/standard_output.txt");
            F1out << ',' << acc.F1;
        }
        F1out << endl;
    }

    return 0;
}