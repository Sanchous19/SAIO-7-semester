#include <algorithm>
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
    long long cost;
};

struct Graph {
    std::vector<std::vector<size_t>> graph;
    std::vector<Edge> edges;
    
    void PushEdge(Edge edge) {
        Edge back_edge{edge.to, edge.from, 0, 0, -edge.cost};
        PushOneEdge(std::move(edge));
        PushOneEdge(std::move(back_edge));
    }
    
private:
    void PushOneEdge(Edge edge) {
        graph[edge.from].push_back(edges.size());
        edges.push_back(std::move(edge));
    }
};


Graph graph;
size_t s, t;
long long sum_value = 0;
std::vector<long long> potentials;

void Input() {
    size_t n, m;
    std::cin >> n >> m;
    s = 0;
    t = n + m + 1;
    graph.graph.resize(n + m + 2);
    graph.edges.reserve(n + m + n * m);
    
    for (size_t i = 1; i <= n; i++) {
        long long capacity;
        std::cin >> capacity;
        sum_value += capacity;
        graph.PushEdge(Edge{s, i, capacity, 0, 0});
    }
    for (size_t i = 1; i <= m; i++) {
        long long capacity;
        std::cin >> capacity;
        graph.PushEdge(Edge{n + i, t, capacity, 0, 0});
    }
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= m; j++) {
            long long cost;
            std::cin >> cost;
            graph.PushEdge(Edge{i, n + j, graph.edges[2 * (i - 1)].capacity, 0, cost});
        }
    }
}

std::vector<long long> Levits() {
    std::vector<long long> dist(graph.graph.size(), std::numeric_limits<long long>::max());
    std::vector<int> type(graph.graph.size(), 2);
    std::queue<size_t> main_queue, express_queue;
    main_queue.push(s);
    dist[s] = 0;
    type[s] = 1;
    
    while (!main_queue.empty() || !express_queue.empty()) {
        std::queue<size_t>& queue = express_queue.empty() ? main_queue : express_queue;
        size_t v = queue.front();
        queue.pop();
        for (size_t edge_id : graph.graph[v]) {
            Edge& edge = graph.edges[edge_id];
            if (edge.flow < edge.capacity && dist[edge.from] + edge.cost < dist[edge.to]) {
                dist[edge.to] = dist[edge.from] + edge.cost;
                if (type[edge.to] == 2) {
                    main_queue.push(edge.to);
                }
                else if (type[edge.to] == 0) {
                    express_queue.push(edge.to);
                }
                type[edge.to] = 1;
            }
        }
        type[v] = 0;
    }
    return dist;
}

std::vector<size_t> Dijkstra() {
    std::vector<long long> dist(graph.graph.size(), std::numeric_limits<long long>::max());
    std::vector<size_t> prev_edge(graph.graph.size());
    dist[s] = 0;
    prev_edge[s] = std::numeric_limits<size_t>::max();
    std::set<std::pair<long long, size_t>> queue;
    queue.insert({0, s});
    while (!queue.empty()) {
        size_t v = queue.begin()->second;
        queue.erase(queue.begin());
        
        for (size_t edge_id : graph.graph[v]) {
            Edge& edge = graph.edges[edge_id];
            long long new_dist = dist[edge.from] + edge.cost + potentials[edge.from] - potentials[edge.to];
            if (edge.flow < edge.capacity && new_dist < dist[edge.to]) {
                queue.erase({dist[edge.to], edge.to});
                dist[edge.to] = new_dist;
                prev_edge[edge.to] = edge_id;
                queue.insert({dist[edge.to], edge.to});
            }
        }
    }
    
    for (size_t v = 0; v < graph.graph.size(); v++) {
        if (potentials[v] == std::numeric_limits<long long>::max()) {
            potentials[v] = std::numeric_limits<long long>::max();
        }
        else {
            potentials[v] += dist[v];
        }
    }
    
    std::vector<size_t> way;
    for (size_t edge_id = prev_edge[t]; edge_id != std::numeric_limits<size_t>::max();
         edge_id = prev_edge[graph.edges[edge_id].from]){
        way.push_back(edge_id);
    }
    std::reverse(way.begin(), way.end());
    return way;
}

long long MinCostMaxFlow() {
    long long cost = 0, flow = 0;
    potentials = Levits();
    while (flow != sum_value) {
        long long add_flow = std::numeric_limits<long long>::max();
        std::vector<size_t> way = Dijkstra();
        for (size_t edge_id : way) {
            Edge& edge = graph.edges[edge_id];
            add_flow = std::min(add_flow, edge.capacity - edge.flow);
        }
        for (size_t edge_id : way) {
            Edge& edge = graph.edges[edge_id];
            Edge& back_edge = graph.edges[edge_id ^ 1];
            edge.flow += add_flow;
            back_edge.flow -= add_flow;
            cost += edge.cost * add_flow;
        }
        flow += add_flow;
    }
    return cost;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    Input();
    std::cout << MinCostMaxFlow();

    return 0;
}