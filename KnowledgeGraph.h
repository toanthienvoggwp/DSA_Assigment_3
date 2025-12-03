#ifndef KNOWLEDGEGRAPH_H
#define KNOWLEDGEGRAPH_H

#include "main.h"

// Forward declaration
template <class T> class VertexNode;
template <class T> class DGraphModel;

// =====================================
// Class Edge
// =====================================
template <class T>
class Edge {
    #ifdef TESTING
        friend class TestHelper;
    #endif
private:
    VertexNode<T>* from;
    VertexNode<T>* to;
    float weight;

public:
    Edge(VertexNode<T>* from = nullptr, VertexNode<T>* to = nullptr, float weight = 0);
    
    bool equals(Edge<T>* edge);
    static bool edgeEQ(Edge<T>*& edge1, Edge<T>*& edge2);
    string toString();

    friend class VertexNode<T>;
    friend class DGraphModel<T>;
};

// =====================================
// Class VertexNode
// =====================================
template <class T>
class VertexNode {
    #ifdef TESTING
        friend class TestHelper;
    #endif
private:
    T vertex;
    int inDegree_;
    int outDegree_;
    vector<Edge<T>*> adList; 
    
    // Function pointers
    bool (*vertexEQ)(T&, T&);
    string (*vertex2str)(T&);

public:
    VertexNode(T vertex, bool (*vertexEQ)(T&, T&) = nullptr, string (*vertex2str)(T&) = nullptr);
    
    T& getVertex();
    void connect(VertexNode<T>* to, float weight = 0);
    Edge<T>* getEdge(VertexNode<T>* to);
    bool equals(VertexNode<T>* node);
    void removeTo(VertexNode<T>* to);
    int inDegree();
    int outDegree();
    string toString();

    friend class Edge<T>;
    friend class DGraphModel<T>;
};

// =====================================
// Class DGraphModel
// =====================================
template <class T>
class DGraphModel {
    #ifdef TESTING
        friend class TestHelper;
    #endif
private:
    vector<VertexNode<T>*> nodeList;
    
    // Function pointers
    bool (*vertexEQ)(T&, T&);
    string (*vertex2str)(T&);

public:
    DGraphModel(bool (*vertexEQ)(T&, T&) = nullptr, string (*vertex2str)(T&) = nullptr);
    ~DGraphModel();

    VertexNode<T>* getVertexNode(T& vertex);
    string vertex2Str(VertexNode<T>& node);
    string edge2Str(Edge<T>& edge);
    bool checkequal(T &node1, T &node2);

    void add(T vertex);
    bool contains(T vertex);
    float weight(T from, T to);
    vector<T> getOutwardEdges(T from);
    
    void connect(T from, T to, float weight = 0);
    void disconnect(T from, T to);
    bool connected(T from, T to);

    int size();
    bool empty();
    void clear();
    
    int inDegree(T vertex);
    int outDegree(T vertex);
    vector<T> vertices();
    
    string toString();
    string BFS(T start);
    string DFS(T start);
};

// =====================================
// Class KnowledgeGraph
// =====================================
class KnowledgeGraph {
    #ifdef TESTING
        friend class TestHelper;
    #endif
private:
    DGraphModel<string> graph;
    vector<string> entities;

public:
    static bool stringEQ(string &a, string &b);
    static string string2str(string &a);
    KnowledgeGraph();
    
    void addEntity(string entity);
    void addRelation(string from, string to, float weight = 1.0f);
    
    vector<string> getAllEntities();
    vector<string> getNeighbors(string entity);
    
    string bfs(string start);
    string dfs(string start);
    
    bool isReachable(string from, string to);
    string toString();
    
    vector<string> getRelatedEntities(string entity, int depth = 2);
    string findCommonAncestors(string entity1, string entity2);
    vector<pair<string,int>> collectancestor(string &start);
};

#endif // KNOWLEDGEGRAPH_H