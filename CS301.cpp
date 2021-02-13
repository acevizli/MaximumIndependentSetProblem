// CS301.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <random>
#include <ctime>
#include <time.h>
#include <chrono>
#include <iomanip>
#include <numeric>
#define NODE_NUMBER 200  // vertex node number
#define RUN_TIME 500 // number of different runs for analysis
#define GRAPH_TE 10
#define GRAPH_TW 20
#define GRAPH_TH 30
#define GRAPH_FO 40
#define GRAPH_EI 80 //edge densities of graphs
#define GRAPH_FI 50
#define GRAPH_SI 60
#define GRAPH_SE 70

std::vector<std::vector<int>> graph(NODE_NUMBER,std::vector<int>(NODE_NUMBER)); //adjacency matrix

int random_range(const int& min, const int& max) // return random number uniformly for given range
{
    static std::mt19937 generator(time(0));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void checkKvertex(int nodeIndex,int& minIndex, int & minValueM,int & minValueK,const std::vector<bool> & marked) // for uncovered vertices finds the m value and compare it with current minumum m value
{
    int k = 0;
    std::vector<int> neighboors;
    for (int i = 0; i < NODE_NUMBER; i++)
    {
        if (!marked[i] && graph[nodeIndex][i]) neighboors.push_back(i);
    }
    for (int i = 0; i < neighboors.size(); i++)
    {
        for (int j = i+1; j < neighboors.size(); j++) if (graph[neighboors[i]][neighboors[j]]) k++;
    }
    int n = (neighboors.size() * (neighboors.size() - 1)) / 2;
    int m = n - k;
    k = neighboors.size();
    if (m < minValueM)
    {
        minValueM = m;
        minValueK = k;
        minIndex = nodeIndex;
    }
    else if (m == minValueM && k > minValueK)
    {
        minValueK = k;
        minIndex = nodeIndex;
    }
}

void mark(int nodeIndex, std::vector<bool>& marked,int & vertexCount) // mark the vertex and its neighbors after including into set
{
    marked[nodeIndex] = true;
    vertexCount--;
    for (int i = 0; i < NODE_NUMBER; i++)
    {
        if (!marked[i] && graph[nodeIndex][i])
        {
            vertexCount--;
            marked[i] = true;
        }
    }
}
void MIS(std::vector<bool>& marked,std::vector<int> & MIS,int & EST) // heuristic algorithm of finding maximum independent set
{
    int vertexCount = NODE_NUMBER;
    while (vertexCount)
    {
        int minIndex, minValueM = INT_MAX, minValueK;
        for (int i = 0; i < NODE_NUMBER; i++)
        {
            if (!marked[i])
            {
                checkKvertex(i, minIndex, minValueM, minValueK,marked); //O(V^2E)
            }
        }
        MIS.push_back(minIndex);
        mark(minIndex, marked,vertexCount); //O(VE)
        //if(minValueM>EST) EST = minValueM;
        EST += minValueM;
    }
}
bool isIndependentSet(std::vector<int>& MIS) // checks for given set is independent set or not with respect to graph
{
    for (size_t i = 0; i < MIS.size(); i++)
    {
        for (size_t j = i+1; j < MIS.size(); j++)
        {
            if (graph[MIS[i]][MIS[j]])
            {
                return false;
            }
        }
    }
    return true;
}

/* Brute Force Algorithm functions Starts Here
//////////////////////////////////////////////*/
bool isSafeForIndependentSet(int index, std::set<int>& temp)
{
    for (auto i:temp)
    {
        if (graph[i][index]) return false;
    }
    return true;
}

std::set<std::set<int>> independentSets;
std::set<std::set<int>> maximalIndependentSets;

void findAllIndependentSets(
    int currV,
    int setSize,
    std::set<int> tempSolutionSet)
{
    for (int i = currV; i <= setSize; i++) {
        if (isSafeForIndependentSet(
            i-1,
            tempSolutionSet))
        {
            tempSolutionSet.insert(i-1);
            findAllIndependentSets(
                i + 1,
                setSize,
                tempSolutionSet);
            tempSolutionSet
                .erase(i-1);
        }
    }
    independentSets.insert(tempSolutionSet);
}
int sizeMaximalIndependentSets()
{
    int maxCount = 0;
    int localCount = 0;
    for (auto iter : independentSets) {

        localCount = 0;
        for (auto iter2 : iter) {
            localCount++;
        }
        if (localCount > maxCount)
            maxCount = localCount;
    }
    return maxCount;
}
void printMaximalIndependentSets()
{
    int maxCount = 0;
    int localCount = 0;
    for (auto iter : independentSets) {

        localCount = 0;
        for (auto iter2 : iter) {
            localCount++;
        }
        if (localCount > maxCount)
            maxCount = localCount;
    }
    for (auto iter : independentSets) {

        localCount = 0;
        std::set<int> tempMaximalSet;

        for (auto iter2 : iter) {
            localCount++;
            tempMaximalSet.insert(iter2);
        }
        if (localCount == maxCount)
            maximalIndependentSets
            .insert(tempMaximalSet);
    }
    for (auto iter : maximalIndependentSets) {
        std::cout << "{ ";
        for (auto iter2 : iter) {
            std::cout << iter2 << " ";
        }
        std::cout << "}";
    }
    std::cout << std::endl;
}
/*Brute force algorithms end here
/////////////////////////////////*/
void clear(std::vector<std::vector<int>>& graph) // clear graph after each run
{
    for ( int i = 0; i <NODE_NUMBER; i++)
    {
        graph[i].clear();
        graph[i] = std::vector<int>(NODE_NUMBER);
    }
}
int main()
{
    std::cout << "choose option 1 for runtime\n"
        "choose option 2 for correctness\n"
        "choose option 3 for quality\n"
        "choose option 4 for testing\n";
    std::string option;
    std::cin >> option;
    if (option == "1")
    {
        std::vector<double>runtimes;
        for (int i = 0; i < RUN_TIME; i++)
        {
            for (int j = 0; j < NODE_NUMBER; j++)
            {
                for (int k = j + 1; k < NODE_NUMBER; k++)         // random graph algorithm 
                {
                    int a = random_range(0, 100);
                    if (a < GRAPH_FI) graph[j][k] = graph[k][j] = 1;
                }
            }
            std::vector<int> MISet;
            int EST = 0;
            std::vector<bool> marked(NODE_NUMBER);
            auto timeStart = std::chrono::system_clock::now();
            MIS(marked, MISet, EST);
            auto timeEnd = std::chrono::system_clock::now();

            std::chrono::duration<double> diff = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart);
            runtimes.push_back(diff.count());
            clear(graph);
        }
        /*Performance analysis*/
        double sum = std::accumulate(runtimes.begin(), runtimes.end(), 0.0);
        double mean = sum / runtimes.size();
        double sq_sum = std::inner_product(runtimes.begin(), runtimes.end(), runtimes.begin(), 0.0,
            [](double const& x, double const& y) { return x + y; },
            [mean](double const& x, double const& y) { return (x - mean) * (y - mean); });
        double stdev = std::sqrt(sq_sum / runtimes.size());
        double sterr = stdev / sqrt(runtimes.size());

        double tval90 = 1.66;
        double tval95 = 1.984;
        double intervalUpperC90 = mean + sterr * tval90;
        double intervalLowerC90 = mean - sterr * tval90;

        double intervalUpperC95 = mean + sterr * tval95;
        double intervalLowerC95 = mean - sterr * tval95;
        std::ofstream out;
        out.open("data.txt", std::ios_base::app);
        out << NODE_NUMBER << " " << mean << " " << stdev << " " << sterr << " " << intervalLowerC90 << "-" << intervalUpperC90 << " " << intervalLowerC95 << "-" << intervalUpperC95 << std::endl;
        out.close();
    }
    else if (option == "2") /*Correctness analysis*/
    {
        std::vector<int> correctness;
        for (int i = 0; i < RUN_TIME; i++)
        {
            for (int j = 0; j < NODE_NUMBER; j++)
            {
                for (int k = j + 1; k < NODE_NUMBER; k++)
                {
                    int a = random_range(0, 100);
                    if (a < GRAPH_FI) graph[j][k] = graph[k][j] = 1;
                }
            }

            std::vector<int> MISet;
            int EST = 0;
            std::vector<bool> marked(NODE_NUMBER);
            MIS(marked, MISet, EST);
            correctness.push_back(isIndependentSet(MISet));
            clear(graph);
        }
        int sum = std::accumulate(correctness.begin(), correctness.end(), 0);
        double mean = sum * 1.0 / correctness.size();
        std::cout << "Correctness: %" << mean*100 << std::endl;
    }
    else if (option == "3") /*analysis of ratio bound - quality*/ // for this part keep NODE_NUMBER small around 10 30 get quality value
    {   
        std::ofstream output;
        output.open("data.txt", std::ios_base::app);
        std::vector<double> quality;
        for (int i = 0; i < RUN_TIME; i++)
        {
            for (int j = 0; j < NODE_NUMBER; j++)
            {
                for (int k = j + 1; k < NODE_NUMBER; k++)
                {
                    int a = random_range(0, 100);
                    if (a < GRAPH_FI) graph[j][k] = graph[k][j] = 1;
                }
            }

            std::vector<int> MISet;
            int EST = 0;
            std::vector<bool> marked(NODE_NUMBER);
            MIS(marked, MISet, EST);
            std::set<int> temp;
            findAllIndependentSets(1, NODE_NUMBER, temp);
            quality.push_back(MISet.size() * 1.0 / sizeMaximalIndependentSets());
            clear(graph);
            maximalIndependentSets.clear();
            independentSets.clear();
        }
        double sum = std::accumulate(quality.begin(), quality.end(), 0.0);
        double mean = sum / quality.size();
        output << mean << " ";
        output.close();

    }
    else if (option == "4") /*Testing of algorithm*/   //keep NODE_NUMBER small, get random graph with MIS and brute force for testing purposes 
    {
        for (int j = 0; j < NODE_NUMBER; j++)
        {
            for (int k = j + 1; k < NODE_NUMBER; k++)
            {
                int a = random_range(0, 100);
                if (a < GRAPH_FI) graph[j][k] = graph[k][j] = 1;
            }
        }
        std::vector<int> MISet;
        int EST = 0;
        std::vector<bool> marked(NODE_NUMBER);
        MIS(marked, MISet, EST);
        std::set<int> temp;
        findAllIndependentSets(1, NODE_NUMBER, temp);
        for (int i = 0; i < NODE_NUMBER; i++)
        {
            for (int j = 0; j < NODE_NUMBER; j++)
            {
                std::cout << graph[i][j] << " ";       // represent graph in adjacency matrix
            }
            std::cout << std::endl;
        }
        std::cout << "Heuristic Max indepedent set\n{ ";
        for (auto i : MISet) std::cout << i<<", ";
        std::cout << "}\nAll Max independent sets:\n";     // comparison part
        printMaximalIndependentSets();
        std::cout << "Estimation Value: " << EST*1.0/NODE_NUMBER << std::endl;
    }
}
