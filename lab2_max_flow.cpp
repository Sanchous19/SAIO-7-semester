#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>


struct Edge {
    size_t from;
    size_t to;
    long long capacity;
    long long flow;
};

std::istream& operator>>(std::istream& in, Edge& edge) {
    in >> edge.from >> edge.to >> edge.capacity;
    edge.flow = 0;
    return in;
}

struct Graph {
    std::vector<std::vector<size_t>> graph;
    std::vector<Edge> edges;
    
    void PushEdge(Edge edge) {
        graph[edge.from].push_back(edges.size());
        edges.push_back(std::move(edge));
    }
};


Graph graph;
size_t s, t;
std:: vector<long long> dist;

void Input() {
    size_t n, m;
    std::cin >> n >> m;
    std::cin >> s >> t;
    
    graph.graph.resize(n + 1);
    graph.edges.reserve(2 * m);
    while (m--) {
        Edge edge;
        std::cin >> edge;
        Edge back_edge{edge.to, edge.from, 0, 0};
        graph.PushEdge(std::move(edge));
        graph.PushEdge(std::move(back_edge));
    }
}

bool BFS() {
    static size_t max = std::numeric_limits<size_t>::max();
    
    std::queue<size_t> queue;
    dist.assign(graph.graph.size(), max);
    dist[s] = 0;
    queue.push(s);
    while (!queue.empty() && dist[t] == max) {
        size_t v = queue.front();
        queue.pop();
        for (size_t edge_id : graph.graph[v]) {
            const Edge& edge = graph.edges[edge_id];
            if (dist[edge.to] == max && edge.flow < edge.capacity) {
                dist[edge.to] = dist[edge.from] + 1;
                queue.push(edge.to);
            }
        }
    }
    return dist[t] != max;
}

long long DFS(size_t v, long long flow) {
    if (v == t) {
        return flow;
    }
    
    for (size_t edge_id : graph.graph[v]) {
        Edge& edge = graph.edges[edge_id];
        if (dist[edge.to] != dist[edge.from] + 1 || edge.capacity - edge.flow == 0) {
            continue;
        }
        
        long long pushed = DFS(edge.to, std::min(flow, edge.capacity - edge.flow));
        if (pushed) {
            edge.flow += pushed;
            graph.edges[edge_id ^ 1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

long long Dinic() {
    long long ans = 0;
    while (BFS()) {
        while (long long flow = DFS(s, std::numeric_limits<long long>::max())) {
            ans += flow;
        }
    }
    return ans;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    freopen("flow.in", "r", stdin);
    freopen("flow.out", "w", stdout);
    
    Input();
    
    std::cout << Dinic() << "\n";
    for (size_t i = 0; i < graph.edges.size(); i += 2) {
        std::cout << graph.edges[i].flow << "\n";
    }

    return 0;
}