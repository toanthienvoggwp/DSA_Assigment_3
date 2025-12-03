#include "KnowledgeGraph.h"
#include "main.h"

// =============================================================================
// Class Edge Implementation
// =============================================================================

template <class T>
Edge<T>::Edge(VertexNode<T>* from, VertexNode<T>* to, float weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

template <class T>
bool Edge<T>::equals(Edge<T> *edge)
{
    if(!edge) return false;
    return this->from->equals(edge->from) && this->to->equals(edge->to);
}

template <class T>
bool Edge<T>::edgeEQ(Edge<T> *&edge1, Edge<T> *&edge2)
{
    if(edge1 == nullptr && edge2 == nullptr) return true;
    else if(edge1 == nullptr || edge2 == nullptr) return false;
    return edge1->equals(edge2);
}

template <class T>
string Edge<T>::toString() {
    // TODO: Return the string representation of the edge
    stringstream ss;
    ss << this->from->toString() << " " << this->to->toString() << " " << weight;
    return ss.str();
}

// TODO: Implement other methods of Edge:


// =============================================================================
// Class VertexNode Implementation
// =============================================================================

template <class T>
VertexNode<T>::VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) {
    this->vertex = vertex;
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
    this->inDegree_ = 0;
    this->outDegree_ = 0;
}

template <class T>
T &VertexNode<T>::getVertex()
{
    return vertex;
}

template <class T>
void VertexNode<T>::connect(VertexNode<T> *to, float weight)
{
    Edge<T> *newedge = new Edge<T>(this, to, weight);
    this->adList.push_back(newedge);
    this->outDegree_++;
    to->inDegree_++;
}

template <class T>
Edge<T>* VertexNode<T>::getEdge(VertexNode<T> *to)
{
    for(auto it : adList)
    {
        if(it->to->equals(to)) return it;
    }
    return nullptr;
}

template <class T>
bool VertexNode<T>::equals(VertexNode<T> *node)
{
    if(!node) return false;
    if(vertexEQ)
        return vertexEQ(this->vertex, node->vertex);
    return this->vertex == node->vertex;
}

template<class T>
void VertexNode<T>::removeTo(VertexNode<T> *to)
{
    for(auto it = adList.begin(); it != adList.end(); it++)
    {
        Edge<T> *temp_edge = *it;
        if(temp_edge->to->equals(to))
        {
            this->outDegree_--;
            temp_edge->to->inDegree_--;
            delete temp_edge;
            this->adList.erase(it);
            return;
        }
    }
}

template <class T>
int VertexNode<T>::inDegree()
{
    return inDegree_;
}

template <class T>
int VertexNode<T>::outDegree()
{
    return outDegree_;
}

template <class T>
string VertexNode<T>::toString()
{
    stringstream ss;
    ss << this->getVertex() << " " << this->inDegree() << " " << this->outDegree();
    return ss.str();
}
// =============================================================================
// Class DGraphModel Implementation
// =============================================================================

template <class T>
bool DGraphModel<T>::checkequal(T &node1, T &node2)
{
    if(vertexEQ) return vertexEQ(node1, node2);
    return node1 == node2;
}

template <class T>
DGraphModel<T>::DGraphModel(bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) {
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
}

template <class T>
DGraphModel<T>::~DGraphModel() {
    // TODO: Clear all vertices and edges to avoid memory leaks
    this->clear();
}

template <class T>
VertexNode<T>* DGraphModel<T>::getVertexNode(T &vertex)
{
    for(auto it : nodeList)
    {
        if(vertexEQ)
        {
            if(vertexEQ(it->vertex, vertex))
            {
                return it;
            }
        }
        else if(it->vertex == vertex) return it;
    }
    return nullptr;
}

template <class T>
string DGraphModel<T>::vertex2Str(VertexNode<T> &node)
{
    if(vertex2str)
        return vertex2str(node.getVertex());
    return "";
}

template <class T>
string DGraphModel<T>::edge2Str(Edge<T> &edge)
{
    stringstream ss;
    ss << this->vertex2Str(*edge.from) << "->" << this->vertex2Str(*edge.to);
    return ss.str();
}

template <class T>
void DGraphModel<T>::add(T vertex) 
{
    if(this->getVertexNode(vertex)) return;
    VertexNode<T> *newnode = new VertexNode<T>(vertex, vertexEQ, vertex2str);
    this->nodeList.push_back(newnode);
}

template <class T>
bool DGraphModel<T>::contains(T vertex)
{
    return this->getVertexNode(vertex);
}

template <class T>
float DGraphModel<T>::weight(T from, T to)
{
    auto temp_from = getVertexNode(from);
    auto temp_to = getVertexNode(to);
    if(!temp_from || !temp_to)
        throw VertexNotFoundException("Vertex not found!");
    auto edge = temp_from->getEdge(temp_to);
    if(!edge)
        throw EdgeNotFoundException("Edge not found!");
    return edge->weight;
}

template <class T>
vector<T> DGraphModel<T>::getOutwardEdges(T from)
{
    vector<T> result;
    VertexNode<T> *node = getVertexNode(from);
    if(!node) throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*node));
    for(auto it : node->adList)
    {
        result.push_back(it->to->getVertex());
    }
    return result;
}

template <class T>
void DGraphModel<T>::connect(T from, T to, float weight) 
{
    auto temp_from = getVertexNode(from);
    auto temp_to = getVertexNode(to);
    if(!temp_from)
        throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*temp_from));
    if(!temp_to)
        throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*temp_to));
    if(this->connected(from, to)) return;

    temp_from->connect(temp_to, weight);
}

template <class T>
void DGraphModel<T>::disconnect(T from, T to)
{
    auto temp_from = getVertexNode(from);
    auto temp_to = getVertexNode(to);
    if(!temp_from)
        throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*temp_from));
    if(!temp_to)
        throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*temp_to));
    if(this->connected(from, to))
    {
        temp_from->removeTo(temp_to);
    }
}

template <class T>
bool DGraphModel<T>::connected(T from, T to)
{
    auto temp_from = getVertexNode(from);
    auto temp_to = getVertexNode(to);
    if(!temp_from)
        throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*temp_from));
    if(!temp_to)
        throw VertexNotFoundException("Vertex not found!" + this->vertex2Str(*temp_to));
    return temp_from->getEdge(temp_to);
}

template <class T>
int DGraphModel<T>::size()
{
    return nodeList.size();
}

template <class T>
bool DGraphModel<T>::empty()
{
    return this->size() == 0;
}

template <class T>
void DGraphModel<T>::clear()
{
   for(auto node : nodeList)
   {
    for(auto edge : node->adList)
    {
        delete edge;
    }
    node->adList.clear();
    delete node;
   }
   this->nodeList.clear();
}

template <class T>
int DGraphModel<T>::inDegree(T vertex)
{
    auto node = getVertexNode(vertex);
    if(!node) throw VertexNotFoundException("Vertex not found!");
    return node->inDegree_;
}

template <class T>
int DGraphModel<T>::outDegree(T vertex)
{
    auto node = getVertexNode(vertex);
    if(!node) throw VertexNotFoundException("Vertex not found!");
    return node->outDegree_;
}

template <class T>
vector<T> DGraphModel<T>::vertices()
{
    vector<T> result;
    for(auto node : nodeList)
    {
        result.push_back(node->getVertex());
    }
    return result;
}

template <class T>
string DGraphModel<T>::toString() {
    string result = "";
    for (VertexNode<T>* node : this->nodeList) {
        string nodeStr = this->vertex2Str(*node); 
        result += "Vertex: " + nodeStr + "\n";
        if (!node->adList.empty()) {
            result += "  Edges:\n";
            for (Edge<T>* edge : node->adList) {
                // Sử dụng hàm edge2Str của DGraphModel để lấy chuỗi biểu diễn cạnh
                result += "    -> " + this->edge2Str(*edge) + "\n"; 
            }
        }
    }
    return result;
}

template <class T>
string DGraphModel<T>::BFS(T start)
{
    if(!this->contains(start)) throw VertexNotFoundException("Vertex not found!");
    auto startnode = getVertexNode(start);
    vector<VertexNode<T>*> visited;
    vector<VertexNode<T>*> queues;
    int currentidx = 0;
    visited.push_back(startnode);
    queues.push_back(startnode);
    string result = "";

    while(currentidx < queues.size())
    {
        auto current = queues[currentidx];
        currentidx++;
        result += this->vertex2Str(*current) + " ";

        for(auto edge : current->adList)
        {
            auto neighbor = edge->to;
            bool isvisited = false;
            
            for(auto v : visited)
            {
                if(v == neighbor)
                {
                    isvisited = true;
                    break;
                }
            }

            if(!isvisited)
            {
                visited.push_back(neighbor);
                queues.push_back(neighbor);
            }
        }
    }

    if(!result.empty()) result.pop_back();
    return result;
}

template <class T>
string DGraphModel<T>::DFS(T start)
{
    if(!this->contains(start)) throw VertexNotFoundException("Vertex not found!");
    auto startnode = getVertexNode(start);
    vector<VertexNode<T>*> visited;
    vector<VertexNode<T>*> stacks;
    stacks.push_back(startnode);
    stringstream temp;

    while(!stacks.empty())
    {
        auto current = stacks.back();
        stacks.pop_back();

        bool isvisited = false;
        for(auto v : visited)
        {
            if(v == current)
            {
                isvisited = true;
                break;
            }
        }
        if(!isvisited)
        {
            visited.push_back(current);
            temp << this->vertex2Str(*current) << " ";
        }
        for(auto it = current->adList.rbegin(); it != current->adList.rend(); it++)
        {
            auto edge = *it;
            auto neighbor = edge->to;
            stacks.push_back(neighbor);
        }
    }
    string result = temp.str();
    if(!result.empty()) result.pop_back();
    return result;
}

// TODO: Implement other methods of DGraphModel:

// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

bool KnowledgeGraph::stringEQ(string &a, string &b)
{
    return a == b;
}

string KnowledgeGraph::string2str(string &a)
{
    return a;
}

KnowledgeGraph::KnowledgeGraph() : graph(stringEQ, string2str) {}

void KnowledgeGraph::addEntity(string entity)
{
    for(auto it : entities)
    {
        if(it == entity)
            throw EntityExistsException("Entity already exists!");
    }
    entities.push_back(entity);
}

void KnowledgeGraph::addRelation(string from, string to, float weight)
{
    if(!graph.contains(from) || !graph.contains(to))
    {
        throw EntityNotFoundException("Entity not found!");
    }
    graph.connect(from, to, weight);
}

vector<string> KnowledgeGraph::getNeighbors(string entity)
{
    if(!graph.contains(entity))
        throw EntityNotFoundException("Entity not found!");
    return graph.getOutwardEdges(entity);
}

string KnowledgeGraph::bfs(string start)
{
    if(!graph.contains(start))
        throw EntityNotFoundException("Entity not found!");
    return graph.BFS(start);
}

string KnowledgeGraph::dfs(string start)
{
    if(!graph.contains(start))
        throw EntityNotFoundException("Entity not found!");
    return graph.DFS(start);
}

bool KnowledgeGraph::isReachable(string from, string to)
{
    if(!graph.contains(from) || !graph.contains(to))
        throw EntityNotFoundException("Entity not found!");
    return graph.connected(from, to);
}

string KnowledgeGraph::toString()
{
    return graph.toString();
}

vector<string> KnowledgeGraph::getRelatedEntities(string entity, int depth)
{
    if(!graph.contains(entity))
        throw EntityNotFoundException("Entity not found!");
    
    vector<string> result;
    vector<string> visited;
    vector<pair<string,int>> queues; //<entity,current depth>
    queues.push_back({entity, 0});
    visited.push_back(entity);
    int count = 0;

    while(count < queues.size())
    {
        pair<string,int> current = queues[count++];
        auto current_depth = current.second;
        auto current_entity = current.first;

        if(current_depth >= depth) continue;
        vector<string> neighbor = getNeighbors(current_entity);
        for(auto it : neighbor)
        {
            bool isvisited = false;
            for(auto v : visited)
            {
                if(v == it)
                {
                    isvisited = true;
                    break;
                }
            }
            if(!isvisited)
            {
                visited.push_back(current_entity);
                result.push_back(current_entity);
                queues.push_back({current_entity, current_depth + 1});
            }
        }
    }
    return result;
}

string KnowledgeGraph::findCommonAncestors(string entity1, string entity2)
{
    if(!graph.contains(entity1) || !graph.contains(entity2))
        throw EntityNotFoundException("Entity not found!");
    vector<pair<string,int>> ancestor1 = collectancestor(entity1);
    vector<pair<string,int>> ancestor2 = collectancestor(entity2);
    string result;
    int mindist = 9999999;
    bool found = false;

    for(auto it_1 : ancestor1)
    {
        auto name_1 = it_1.first;
        auto dist_1 = it_1.second;
        for(auto it_2 : ancestor2)
        {
            auto name_2 = it_2.first;
            auto dist_2 = it_2.second;

            if(graph.checkequal(name_1, name_2))
            {
                found = true;
                int total = dist_1 + dist_2;
                if(total < mindist)
                {
                    mindist = total;
                    result = name_1;
                }
            }
        }
    }
    if(!found) return "No common ancestor";
    return result;
}

vector<pair<string,int>> KnowledgeGraph::collectancestor(string &start)
{
    vector<string> visited;
    vector<pair<string,int>> result;
    vector<pair<string,int>> queues;
    int count = 0;
    queues.push_back({start,0});
    visited.push_back(start);
    while(count < queues.size())
    {
        pair<string,int> current = queues[count++];
        string current_entity = current.first;
        int current_dist = current.second;

        if(current_entity != start)
            result.push_back(current);
        for(auto temp : this->entities)
        {
            if(temp == current_entity) continue;
            if(graph.connected(temp,current_entity))
            {
                bool isvisited = false;
                for(auto v : visited)
                {
                    if(v == temp)
                    {
                        isvisited = true;
                        break;
                    }
                }

                if(!isvisited)
                {
                    visited.push_back(current_entity);
                    queues.push_back({current_entity,current_dist + 1});
                }
            }
        }
    }
    return result;
}

// TODO: Implement other methods of KnowledgeGraph:



// =============================================================================
// Explicit Template Instantiation
// =============================================================================

template class Edge<string>;
template class Edge<int>;
template class Edge<float>;
template class Edge<char>;

template class VertexNode<string>;
template class VertexNode<int>;
template class VertexNode<float>;
template class VertexNode<char>;

template class DGraphModel<string>;
template class DGraphModel<int>;
template class DGraphModel<float>;
template class DGraphModel<char>;