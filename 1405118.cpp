//
// Created by shamsad on 12/9/18.
//

#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <limits>
#include <algorithm>
#include <queue>

using namespace std;


struct Node{
    string label;
    int weight;
    double distance;
    bool visited;
    string parent;
    int stage;
};

vector<Node>Vertices;

struct Graph{
    Node from;
    Node to;
    int pathCost;
};

vector<Graph>Topology;


int numOfStages;
double INF = std::numeric_limits<double>::infinity();


Node getNode(string u){
    for(int i=0; i<Vertices.size(); i++){
        if(Vertices[i].label == u){
            return Vertices[i];
        }
    }
}


void readFile(string filename){
    string line;
    FILE* inputFile = freopen(filename.c_str(), "r", stdin);

    if (inputFile == 0) {
        cerr << "Could not read file " << filename << endl;
        return;
    }

    // how many stages?
    getline(cin, line);
    istringstream iss(line);
    iss >> numOfStages;

    for(int i=0; i<numOfStages; i++){
        // how many hotels?
        int hotels;
        getline(cin, line);
        istringstream iss(line);
        iss >> hotels;

        for(int j=0; j<hotels; j++){
            string nodeId;
            int nodeWeight;
            getline(cin, line);
            istringstream iss(line);
            iss >> nodeId>>nodeWeight;

            Vertices.push_back({nodeId, nodeWeight, INF, false, "nil", (i+1)});
        }

        // how many connections?
        int edges;
        getline(cin, line);
        istringstream iss_(line);
        iss_ >> edges;

        for(int j=0; j<edges; j++){
            string from, to;
            int pathCost;
            getline(cin, line);
            istringstream iss(line);
            iss >> from>>to>>pathCost;

            Node _from, _to;

            _from = getNode(from);
            _to = getNode(to);

            Topology.push_back({_from, _to, pathCost});
        }
    }
}

void printVertices(){
    cout<<"Vertex List\n";
    cout<<"Label\tParent\tWeight\tDistance\tVisited\tStage\n";
    for (auto &City : Vertices) {
        cout<<City.label<<"\t"<<City.parent<<"\t"<< City.weight<<"\t"<<City.distance<<"\t"<<City.visited<<"\t"<<City.stage<<'\n';
    }
}


void printTopology(){
    cout<<"Edge List\n";
    cout<<"From\tTo\tEdge Cost\n";
    for(auto &edge: Topology){
        cout<<edge.from.label<<"\t"<<edge.to.label<<"\t"<<edge.pathCost<<'\n';
    }
}


class Comparator{
public:
    double operator() (const Node& n1, const Node& n2){
        return n1.distance > n2.distance;
    }
};

vector<Graph> getAdjacentVertex(Node u){
    vector<Graph>adjacent;

    for(int i=0; i<Topology.size(); i++){
        if(Topology[i].from.label == u.label){
            adjacent.push_back(Topology[i]);
        }
    }

    return adjacent;
}

void updateVertex(Node u, double cost, string parent){
        for (auto &Vertex : Vertices) {
            if(Vertex.label == u.label){
                Vertex.distance = cost;
                Vertex.parent = parent;
                cout<<"vertex "<< u.label<<" is updated with cost "<<cost<<" and it's parent is "<<parent<<'\n';
            }
        }
}


void traverseTopology(){
    priority_queue<Node, vector<Node>, Comparator>pq;

    pq.push(Vertices[0]);
//    cout<<pq.top().label<<" oola "<<pq.top().distance<<'\n';

    while(!pq.empty()){
//        for(int i=0; i<pq.size(); i++){
//            cout<<pq.top().label<<" "<<pq.top().distance<<'\n';
//        }

        Node u = pq.top();
//        cout<<"label "<<u.label<<" "<<u.distance<<'\n';
        pq.pop();

//        if(!u.visited)u.visited = true;
//        else continue;

        vector<Graph> adj = getAdjacentVertex(u);
//        cout<<u.label<<"'s adjacent nodes are ";
//        for(int k=0;k<adj.size();k++){
//            cout<<adj[k].to.label<<" "<<adj[k].from.label<<" "<<adj[k].pathCost<<'\n';
//        }

//        cout<<'\n';

        for(int i=0; i<adj.size(); i++){
//            cout<<v.label<<" and "<<v.distance<<" yoo "<<v.weight<<" oola "<<u.distance<<'\n';

            Node v = getNode(adj[i].from.label);
            double cost = adj[i].pathCost + adj[i].to.weight + v.distance;
//            cout<<"baal "<<cost<<" bit "<<adj[i].from.distance<<'\n';

//            cout<<"dest er dist "<<getNode(adj[i].to.label).distance<<'\n';

            if(cost < getNode(adj[i].to.label).distance){
//                v.distance = cost;
//                v.parent = u.label;
                cout<<"cost "<<cost<<" "<<getNode(adj[i].to.label).distance<<'\n';
                updateVertex(adj[i].to, cost, u.label);

                pq.push(getNode(adj[i].to.label));

//                cout<<"pushed node "<<adj[i].to.label<<" "<<cost<<'\n';
            }
        }
    }
}



void print(){
    cout<<"List\n";
    cout<<"Node\tDist\tParent\n";
    for(auto &node: Vertices){
        cout<<node.label<<"\t"<<node.distance<<"\t"<<node.parent<<'\n';
    }
}


void getPath(Node u){
    Node v;

    if(u.label == "0"){
        cout<<u.label;
        return;
    }

    for(int i=0; i<Vertices.size(); i++){
        if(Vertices[i].label == u.parent){
            v = Vertices[i];
            break;
        }
    }

    getPath(v);

    cout<<" "<<u.label;
}


int main(int argc, char* argv[]) {
    string fileName;
    fileName = argv[1];

    // push the start vertex
    Vertices.push_back({"0", 0, 0, false, "nil", 0});

    readFile(fileName);

//    printVertices();
//    printTopology();

    traverseTopology();
    printVertices();

//    print();

    vector<string>path;

    double cost = INF;
    Node u;

    for(int i=0; i<Vertices.size(); i++){
        if(Vertices[i].stage == numOfStages && Vertices[i].distance < cost){
            cost = Vertices[i].distance;
            u = Vertices[i];
        }
    }

    getPath(u);
    cout<<'\n';
    return 0;
}