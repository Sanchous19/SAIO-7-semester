#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

struct Edge {
    size_t from;
    size_t to;
    int len;
};

using Graph = std::vector<std::vector<Edge>>;

const int INF = 1e9;
const size_t MAXN = 16;
size_t n, m, k;
Graph graph;
int dp[1 << MAXN][MAXN][MAXN + 2];

void InitDp() {
    for (size_t mask = 0; mask < (1 << n); mask++) {
        for (size_t i = 0; i < n; i++) {
            for (size_t kk = 0; kk <= k + 1; kk++) {
                dp[mask][i][kk] = INF;
            }
        }
    }
    for (size_t i = 0; i < n; i++) {
        dp[1 << i][i][k] = 0;
    }
}

int Solve() {
    InitDp();
    for (size_t mask = 1; mask < (1 << n); mask++) {
        for (size_t i = 0; i < n; i++) {
            if ((1 << i) & mask) {
                for (size_t kk = 0; kk <= k; kk++) {
                    for (size_t j = 0; j < n; j++) {
                        if (!((1 << j) & mask)) {
                            int& new_state = dp[mask | (1 << j)][j][kk];
                            new_state = std::min(new_state, dp[mask][i][kk + 1]);
                        }
                    }
                    for (const Edge& edge : graph[i]) {
                        if (!((1 << edge.to) & mask)) {
                            int& new_state = dp[mask | (1 << edge.to)][edge.to][kk];
                            new_state = std::min(new_state, dp[mask][i][kk] + edge.len);
                        }
                    }
                }
            }
        }
    }

    int ans = INF;
    for (int i = 0; i < n; i++) {
        for (size_t kk = 0; kk <= k; kk++) {
            ans = std::min(ans, dp[(1 << n) - 1][i][kk]);
        }
    }
    return ans == INF ? -1 : ans;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> m >> k;
    std::unordered_map<std::string, size_t> cities;
    graph.resize(n);
    for (size_t i = 0; i < n; i++) {
        std::string city;
        std::cin >> city;
        cities[city] = i;
    }
    for (size_t i = 0; i < m; i++) {
        std::string city1, city2;
        int len;
        std::cin >> city1 >> city2 >> len;
        size_t city1_id = cities[city1], city2_id = cities[city2];
        graph[city1_id].push_back(Edge{city1_id, city2_id, len});
        graph[city2_id].push_back(Edge{city2_id, city1_id, len});
    }

    std::cout << Solve();

    return 0;
}