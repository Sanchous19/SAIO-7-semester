#include <iostream>
#include <set>
#include <vector>


struct Edge {
    size_t from;
    size_t to;
    long long len;
};

std::istream& operator>>(std::istream& in, Edge& edge) {
    in >> edge.from >> edge.to >> edge.len;
    return in;
}


using Graph = std::vector<std::vector<Edge>>;

std::istream& operator>>(std::istream& in, Graph& graph) {
    size_t n, m;
    in >> n >> m;
    graph.resize(n + 1);
    while (m--) {
        Edge edge;
        in >> edge;
        graph[edge.from].push_back(edge);
    }
    return in;
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Graph graph;
    std::cin >> graph;

    std::vector<long long> dist(graph.size(), 0);
    std::set<std::pair<long long, size_t>> queue;
    queue.insert({0, 1});
    while (!queue.empty()) {
        size_t v = std::prev(queue.end())->second;
        queue.erase(std::prev(queue.end()));

        for (const Edge& edge : graph[v]) {
            if (dist[edge.from] + edge.len > dist[edge.to]) {
                queue.erase({dist[edge.to], edge.to});
                dist[edge.to] = dist[v] + edge.len;
                queue.insert({dist[edge.to], edge.to});
            }
        }
    }

    std::cout << dist[graph.size() - 1];

    return 0;
}