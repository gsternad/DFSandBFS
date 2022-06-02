#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <chrono>
    using std::chrono::duration_cast;
    using std::chrono::microseconds;
    using std::chrono::steady_clock;

void menu() {
    unsigned char rightVertical = 185;  // -|
    unsigned char leftVertical = 204;   // |-
    unsigned char vertical = 186;       // |
    unsigned char edgeRightTop = 187;   // itd.
    unsigned char edgeRightBot = 188;
    unsigned char edgeLeftBot = 200;    // L
    unsigned char edgeLeftTop = 201;
    unsigned char horizontal = 205;     // -

    // STRUCTURE OF MENU
    std::cout << edgeLeftTop;
    for(int i = 0; i < 50; i++)
    {
        std::cout << horizontal;
    }
    std::cout << edgeRightTop << "\n";
    std::cout << vertical;
    for(int i = 0; i < 23; i++)
    {
        std::cout << " ";
    }
    std::cout << "MENU";
    for(int i = 0; i < 23; i++)
    {
        std::cout << " ";
    }
    std::cout << vertical << "\n" << leftVertical;
    for(int i = 0; i < 50; i++)
    {
        std::cout << horizontal;
    }
    std::cout << rightVertical << "\n";

    std::cout << vertical << "  1 ... READ GRAPH                                " << vertical << std::endl;
    std::cout << vertical << "  2 ... DFS FROM s TO f                           " << vertical << std::endl;
    std::cout << vertical << "  3 ... BFS FROM s TO f                           " << vertical << std::endl;
    std::cout << vertical << "  0 ... EXIT                                      " << vertical << std::endl;

    std::cout << edgeLeftBot;
    for(int i = 0; i < 50; i++)
    {
        std::cout << horizontal;
    }
    std::cout << edgeRightBot << "\nPlease select:\n";
}

struct Node
{
    int pre;
    int len;
    int index;
    int status;
    int name;
};

void defaultEdge(int u,
             int v,
             int**& adjMatrix)
{
    adjMatrix[u][v] = 0;
}

void addEdge(int u,
             int v,
             int**& adjMatrix)
{
    adjMatrix[u][v] = 1;
}

void readGraph(const std::string& fileName,
               std::vector<Node*>& nodeVec,
               int**& adjMatrix)
{
    std::ifstream myFile;
    unsigned int numOfNodes = 0;
    unsigned int numOfEdges = 0;
    myFile.open(fileName);
    myFile >> numOfNodes;
    myFile >> numOfEdges;
    adjMatrix = new int*[numOfNodes];
    for(int i = 0; i < numOfNodes; i++)
    {
        adjMatrix[i] = new int[numOfNodes];
        for(int j = 0; j < numOfNodes; j++)
        {
            defaultEdge(i, j, adjMatrix);
        }
    }
    for(int i = 0; i < numOfNodes; i++)
    {
        Node* newNode = new Node;
        newNode->name = i+1;
        newNode->index = i;
        nodeVec.push_back(newNode);
    }

    int v1 = 0;
    int v2 = 0;
    int weight = 0;
    for(int i = 0; i < numOfEdges; i++)
    {
        myFile >> v1 >> v2 >> weight;
        addEdge(v1-1, v2-1, adjMatrix);
    }
    myFile.close();
}

void nodeData(std::vector<Node*> dataVec)
{
    for(auto d: dataVec)
    {
        std::cout << "Node " << d->name << "\n";
        std::cout << "Index in the graph: " << d->index << "\n";
        std::cout << "Length of the path: " << d->len << "\n";
        std::cout << "Development status: " << d->status << "\n";
        std::cout << "This node's predecessor: " << d->pre << "\n\n";
    }
}

void depthFirstSearch(std::vector<Node*> nodeVec,       // GRAF
                      std::vector<Node*>& dataVec,
                      int** adjMatrix,
                      int startNode,
                      int targetNode)
{
    std::stack<Node*> myStack;
    std::vector<Node*> adjNodes;
    for(int i = 1; i < nodeVec.size(); i++)
    {
        nodeVec[i]->status = 0;
        nodeVec[i]->len = INT_MAX;
        nodeVec[i]->pre = -1;
    }
    nodeVec[startNode]->status = 1;
    nodeVec[startNode]->len = 0;
    nodeVec[startNode]->pre = -1;
    myStack.push(nodeVec[startNode]);
    while(!myStack.empty())
    {
        Node* vertex = myStack.top();
        dataVec.push_back(vertex);
        myStack.pop();
        if(vertex->index == targetNode)
        {
            return;
        }
        // DODAM SOSEDE
        for(int i = 0; i < nodeVec.size(); i++)
        {
            if(adjMatrix[vertex->index][i] != 0)
            {
                adjNodes.push_back(nodeVec[i]);
            }
        }
        for(int i = 0; i < adjNodes.size(); i++)
        {
            if(adjNodes[i]->status == 0)
            {
                adjNodes[i]->status = 1;
                adjNodes[i]->len = vertex->len+1;
                adjNodes[i]->pre = vertex->index;
                myStack.push(adjNodes[i]);
            }
        }
        vertex->status = 2;
    }
}

void breadthFirstSearch(std::vector<Node*> nodeVec,       // GRAF
                        std::vector<Node*>& dataVec,
                        int** adjMatrix,
                        int startNode,
                        int targetNode)
{
    std::queue<Node*> myQueue;
    std::vector<Node*> adjNodes;
    for(int i = 1; i < nodeVec.size(); i++)
    {
        nodeVec[i]->status = 0;
        nodeVec[i]->len = INT_MAX;
        nodeVec[i]->pre = -1;
    }
    nodeVec[startNode]->status = 1;
    nodeVec[startNode]->len = 0;
    nodeVec[startNode]->pre = -1;
    myQueue.push(nodeVec[startNode]);
    while(!myQueue.empty())
    {
        Node* vertex = myQueue.front();
        myQueue.pop();
        if(vertex->index == targetNode)
        {
            return;
        }
        // DODAM SOSEDE
        for(int i = 0; i < nodeVec.size(); i++)
        {
            if(adjMatrix[vertex->index][i] != 0)
            {
                adjNodes.push_back(nodeVec[i]);
            }
        }
        for(int i = 0; i < adjNodes.size(); i++)
        {
            if(adjNodes[i]->status == 0)
            {
                adjNodes[i]->status = 1;
                adjNodes[i]->len = vertex->len+1;
                adjNodes[i]->pre = vertex->index;
                myQueue.push(adjNodes[i]);
            }
        }
        vertex->status = 2;
    }
}

bool printPath(std::vector<Node*> nodeVec,
               int startNode,
               int targetNode)
{
    if(startNode == targetNode)
    {
        std::cout << "Found path: " << nodeVec[targetNode]->name << " ";
    }
    else
    {
        if(nodeVec[targetNode]->pre == -1)
        {
            std::cout << "Path between " << nodeVec[startNode]->name << " and " << nodeVec[targetNode]->name << " not found.";
            return false;
        }
        else
        {
            printPath(nodeVec, startNode, nodeVec[targetNode]->pre);
            std::cout << nodeVec[targetNode]->name << " ";
        }
    }
    return true;
}

int main()
{
    std::chrono::steady_clock::time_point start, end;
    std::vector<Node*> nodeVec;
    std::vector<Node*> dataVec;
    int** adjMatrix;

    bool running = true;
    int selection = 0;
    int startNode = 0;
    int targetNode = 0;
    do
    {
        menu();
        std::cin >> selection;
        switch(selection)
        {
            case 1:
                readGraph("graf.txt", nodeVec, adjMatrix);
                break;
            case 2:
                std::cout << "Please input the index of the starting node in the graph:\n";
                std::cin >> startNode;
                std::cout << "Please input the index of the target node in the graph:\n";
                std::cin >> targetNode;
                start = steady_clock::now();
                depthFirstSearch(nodeVec, dataVec, adjMatrix, startNode-1, targetNode-1);
                end = steady_clock::now();
                std::cout << "Duration in microseconds: " << duration_cast<microseconds>(end-start).count() << "us\n\n";
                nodeData(dataVec);
                if(printPath(nodeVec, startNode-1, targetNode-1))
                {
                    std::cout << "\nLength of the path (price): " << nodeVec[targetNode-1]->len;
                }
                std::cout << "\n";
                break;
            case 3:
                std::cout << "Please input the index of the starting node in the graph:\n";
                std::cin >> startNode;
                std::cout << "Please input the index of the target node in the graph:\n";
                std::cin >> targetNode;
                start = steady_clock::now();
                breadthFirstSearch(nodeVec, dataVec, adjMatrix, startNode-1, targetNode-1);
                end = steady_clock::now();
                std::cout << "Duration in microseconds: " << duration_cast<microseconds>(end-start).count() << "us\n\n";
                nodeData(dataVec);
                std::cout << "\n";
                if(printPath(nodeVec, startNode-1, targetNode-1))
                {
                    std::cout << "\nLength of the path (price): " << nodeVec[targetNode-1]->len;
                }
                std::cout << "\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Wrong selection.";
                break;
        }
    } while(running);

    for(int i = 0; i < nodeVec.size(); i++)
    {
        delete nodeVec[i];
    }
    nodeVec.clear();

    for(int i = 0; i < nodeVec.size(); i++)
    {
        delete[] adjMatrix[i];
    }

    delete[] adjMatrix;
}