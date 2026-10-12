#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>
using namespace std;

struct Node {
    int id;
    string name;
    string type;
    int priority;
    int city; // 0 = shared, 1..4 = city
    bool source;
};
struct Edge {
    int id;
    int u, v;
    int repairTime;
    int repairEffort;
    bool damaged;
};

vector<Node> nodes;
vector<Edge> edges;
vector<vector<int>> adjacency;
int mainSource = 0;
int totalSimulatedHours = 0;
int totalRepairEffort = 0;
int completedRepairs = 0;

int addNode(const string& name, const string& type, int priority, int city, bool source = false) {
    int id = static_cast<int>(nodes.size());
    nodes.push_back({id, name, type, priority, city, source});
    adjacency.push_back({});
    return id;
}

int addEdge(int u, int v) {
    int id = static_cast<int>(edges.size());
    int repairTime = 1 + ((id * 3 + u) % 6);   // illustrative simulated hours
    int repairEffort = 1 + ((id * 5 + v) % 8); // illustrative effort units
    edges.push_back({id, u, v, repairTime, repairEffort, false});
    adjacency[u].push_back(id);
    adjacency[v].push_back(id);
    return id;
}

int otherEnd(int edgeId, int nodeId) {
    const Edge& e = edges[edgeId];
    return (e.u == nodeId) ? e.v : e.u;
}

void buildGrid() {
    nodes.clear(); edges.clear(); adjacency.clear();
    totalSimulatedHours = totalRepairEffort = completedRepairs = 0;
    mainSource = addNode("Main Power Station", "Power Source", 0, 0, true);
    for (int city = 1; city <= 4; ++city) {
        string number = to_string(city);
        int sub = addNode("City " + number + " Substation", "Substation", 0, city);
        int dist = addNode("City " + number + " Distribution", "Distribution Hub", 0, city);
        addEdge(mainSource, sub);
        addEdge(sub, dist);
        struct Facility { string name; string type; int priority; };
        vector<Facility> facilities = {
            {"4 Houses", "Residential", 4},
            {"Hospital", "Hospital", 10},
            {"Police Station", "Emergency Service", 9},
            {"Fire Station", "Emergency Service", 9},
            {"Clinic", "Healthcare", 6},
            {"Schools and College", "Education", 4},
            {"Library and Museum", "Public Facility", 2},
            {"Grocery and Market", "Essential Retail", 5},
            {"Office and Business District", "Business", 5},
            {"Industrial Park", "Industry", 5},
            {"Park and Recreation", "Recreation", 2},
            {"Bus and Train Stations", "Transport", 7},
            {"Place of Worship", "Community Facility", 2}
        };
        for (const auto& f : facilities) {
            int facility = addNode("City " + number + " " + f.name, f.type, f.priority, city);
            addEdge(dist, facility);
        }
    }
    int shared = addNode("Shared Infrastructure Hub", "Shared Hub", 0, 0);
    addEdge(mainSource, shared);
    int airport = addNode("Airport", "Transport", 8, 0);
    int water = addNode("Water Treatment Facility", "Critical Infrastructure", 10, 0);
    int waste = addNode("Waste Management Facility", "Public Infrastructure", 3, 0);
    addEdge(shared, airport);
    addEdge(shared, water);
    addEdge(shared, waste);
}

vector<bool> computePowered() {
    vector<bool> powered(nodes.size(), false);
    queue<int> q;
    powered[mainSource] = true;
    q.push(mainSource);
    while (!q.empty()) {
        int current = q.front(); q.pop();
        for (int edgeId : adjacency[current]) {
            if (edges[edgeId].damaged) continue;
            int next = otherEnd(edgeId, current);
            if (!powered[next]) {
                powered[next] = true;
                q.push(next);
            }
        }
    }
    return powered;
}

void displayGrid() {
    cout << "\n=== POWER GRID: NODES ===\n";
    for (const auto& n : nodes) {
        cout << "N" << n.id << " | " << n.name << " | " << n.type;
        if (n.priority > 0) cout << " | priority=" << n.priority;
        cout << '\n';
    }
    cout << "\n=== POWER LINES ===\n";
    for (const auto& e : edges) {
        cout << "L" << e.id << ": " << nodes[e.u].name << " <-> " << nodes[e.v].name
             << " | repair time=" << e.repairTime << "h | effort=" << e.repairEffort
             << " | " << (e.damaged ? "DAMAGED" : "FUNCTIONAL") << '\n';
    }
}

void showPowerStatus() {
    vector<bool> powered = computePowered();
    int downCritical = 0;
    cout << "\n=== FACILITY POWER STATUS ===\n";
    for (const auto& n : nodes) {
        cout << n.name << ": " << (powered[n.id] ? "POWERED" : "NO POWER") << '\n';
        if (!powered[n.id] && n.priority >= 8) ++downCritical;
    }
    cout << "Critical/high-priority facilities without power: " << downCritical << '\n';
}

void generateRandomDamage() {
    cout << "Number of power lines to damage (1-" << edges.size() << "): ";
    int count;
    if (!(cin >> count) || count < 1 || count > static_cast<int>(edges.size())) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Invalid number. No changes made.\n";
        return;
    }
    for (auto& e : edges) e.damaged = false;
    totalSimulatedHours = totalRepairEffort = completedRepairs = 0;
    vector<int> ids(edges.size());
    for (int i = 0; i < static_cast<int>(ids.size()); ++i) ids[i] = i;
    random_device rd;
    mt19937 gen(rd());
    shuffle(ids.begin(), ids.end(), gen);
    for (int i = 0; i < count; ++i) edges[ids[i]].damaged = true;
    cout << "\nTornado scenario generated. Damaged lines: " << count << '\n';
    for (const auto& e : edges) if (e.damaged) {
        cout << "L" << e.id << ": " << nodes[e.u].name << " <-> " << nodes[e.v].name
             << " | estimated repair=" << e.repairTime << "h | effort=" << e.repairEffort << '\n';
    }
    showPowerStatus();
}

int potentialPriorityBehindLine(int edgeId, int unpoweredNode) {
    // Estimates the service importance on the far side of this line in the
    // designed tree-like grid, ignoring current damage on downstream lines.
    vector<bool> seen(nodes.size(), false);
    queue<int> q;
    q.push(unpoweredNode);
    seen[unpoweredNode] = true;
    int total = 0;
    while (!q.empty()) {
        int current = q.front(); q.pop();
        total += nodes[current].priority;
        for (int adjacentEdge : adjacency[current]) {
            if (adjacentEdge == edgeId) continue;
            int next = otherEnd(adjacentEdge, current);
            if (!seen[next]) {
                seen[next] = true;
                q.push(next);
            }
        }
    }
    return total;
}

struct RepairCandidate {
    int edgeId;
    int potentialPriority;
    double score;
};
struct CandidateCompare {
    bool operator()(const RepairCandidate& a, const RepairCandidate& b) const {
        if (a.score != b.score) return a.score < b.score;
        if (edges[a.edgeId].repairEffort != edges[b.edgeId].repairEffort)
            return edges[a.edgeId].repairEffort > edges[b.edgeId].repairEffort;
        return a.edgeId > b.edgeId;
    }
};
priority_queue<RepairCandidate, vector<RepairCandidate>, CandidateCompare> buildRepairQueue() {
    priority_queue<RepairCandidate, vector<RepairCandidate>, CandidateCompare> pq;
    vector<bool> powered = computePowered();
    for (const auto& e : edges) {
        if (!e.damaged) continue;
        // Repair only lines that connect the powered network to an unpowered side.
        if (powered[e.u] == powered[e.v]) continue;
        int farSide = powered[e.u] ? e.v : e.u;
        int potential = potentialPriorityBehindLine(e.id, farSide);
        double score = static_cast<double>(potential) / max(1, e.repairTime);
        pq.push({e.id, potential, score});
    }
    return pq;
}

void showRepairPriorities() {
    auto pq = buildRepairQueue();
    cout << "\n=== REPAIR PRIORITY RECOMMENDATIONS ===\n";
    if (pq.empty()) {
        cout << "No eligible damaged line currently extends the powered network.\n";
        return;
    }
    cout << "Score = potential importance served / estimated repair hours. Repair effort breaks ties.\n";
    int rank = 1;
    while (!pq.empty()) {
        RepairCandidate c = pq.top(); pq.pop();
        const Edge& e = edges[c.edgeId];
        cout << rank++ << ". L" << e.id << " | " << nodes[e.u].name << " <-> " << nodes[e.v].name
             << " | potential priority=" << c.potentialPriority
             << " | time=" << e.repairTime << "h | effort=" << e.repairEffort
             << " | score=" << fixed << setprecision(2) << c.score << '\n';
    }
    cout << "Note: scores are illustrative heuristics, not guaranteed optimal schedules.\n";
}

void repairNextLine() {
    auto pq = buildRepairQueue();
    if (pq.empty()) {
        cout << "No eligible repair found. Check the current grid status.\n";
        return;
    }
    RepairCandidate next = pq.top();
    Edge& e = edges[next.edgeId];
    vector<bool> before = computePowered();
    e.damaged = false;
    totalSimulatedHours += e.repairTime;
    totalRepairEffort += e.repairEffort;
    ++completedRepairs;
    vector<bool> after = computePowered();
    cout << "\nRepair completed: L" << e.id << " (" << nodes[e.u].name << " <-> " << nodes[e.v].name << ")\n";
    cout << "Repair time: " << e.repairTime << "h | Effort: " << e.repairEffort << '\n';
    cout << "Newly powered locations: ";
    bool any = false;
    for (const auto& n : nodes) {
        if (after[n.id] && !before[n.id]) {
            if (any) cout << ", ";
            cout << n.name;
            any = true;
        }
    }
    if (!any) cout << "none immediately";
    cout << "\nCumulative simulated repair time: " << totalSimulatedHours << "h"
         << " | Total effort: " << totalRepairEffort << '\n';

    int downCritical = 0;
    for (const auto& n : nodes) {
        if (!after[n.id] && n.priority >= 8)
            ++downCritical;
    }
    cout << "Critical/high-priority facilities still without power: "
         << downCritical << '\n';
}

void introduceNewFailure() {
    cout << "Enter power line ID (number only): L";
    int id;
    if (!(cin >> id) || id < 0 || id >= static_cast<int>(edges.size())) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Invalid line ID.\n";
        return;
    }
    if (edges[id].damaged) {
        cout << "That line is already damaged.\n";
        return;
    }
    edges[id].damaged = true;
    cout << "New failure recorded on L" << id << ". Rechecking power status.\n";
    showPowerStatus();
}

void resetGrid() {
    for (auto& e : edges) e.damaged = false;
    totalSimulatedHours = totalRepairEffort = completedRepairs = 0;
    cout << "Grid reset. All power lines are functional.\n";
}

void searchLine() {
    cout << "Enter power line ID (number only): L";
    int id;
    if (!(cin >> id) || id < 0 || id >= static_cast<int>(edges.size())) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Invalid line ID.\n";
        return;
    }
    const Edge& e = edges[id];
    cout << "L" << e.id << ": " << nodes[e.u].name << " <-> " << nodes[e.v].name
         << " | " << (e.damaged ? "DAMAGED" : "FUNCTIONAL")
         << " | repair time=" << e.repairTime << "h | effort=" << e.repairEffort << '\n';
}

void sortDamagedLines() {
    vector<int> damaged;
    for (const auto& e : edges) if (e.damaged) damaged.push_back(e.id);
    sort(damaged.begin(), damaged.end(), [](int a, int b) {
        if (edges[a].repairTime != edges[b].repairTime)
            return edges[a].repairTime < edges[b].repairTime;
        return edges[a].repairEffort < edges[b].repairEffort;
    });
    cout << "\nDamaged lines sorted by repair time (then effort):\n";
    if (damaged.empty()) { cout << "No damaged lines.\n"; return; }
    for (int id : damaged) {
        const Edge& e = edges[id];
        cout << "L" << id << " | " << nodes[e.u].name << " <-> " << nodes[e.v].name
             << " | " << e.repairTime << "h | effort " << e.repairEffort << '\n';
    }
}

void showCityTree() {
    cout << "Choose city (1-4): ";
    int city;
    if (!(cin >> city) || city < 1 || city > 4) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Invalid city.\n";
        return;
    }
    cout << "\n" << "City " << city << " Distribution Tree\n";
    for (const auto& n : nodes) {
        if (n.city == city && n.type == "Substation") cout << n.name << '\n';
    }
    cout << "  |\n  +-- City " << city << " Distribution\n";
    for (const auto& n : nodes) {
        if (n.city == city && n.type != "Substation" && n.type != "Distribution Hub")
            cout << "      +-- " << n.name << " (priority " << n.priority << ")\n";
    }
}

void printMenu() {
    cout << "\n========== POWER GRID RESTORATION SYSTEM ==========\n"
         << "1. Display power grid and line status\n"
         << "2. Generate random tornado damage\n"
         << "3. Show power outages (BFS)\n"
         << "4. View repair-priority queue\n"
         << "5. Perform next recommended repair\n"
         << "6. Introduce an additional line failure\n"
         << "7. Reset to normal grid\n"
         << "8. Search for a line by ID\n"
         << "9. Sort damaged lines by repair time\n"
         << "10. Display a city's distribution tree\n"
         << "0. Exit\n"
         << "Enter choice: ";
}

int main() {
    buildGrid();
    cout << "Intelligent Power Grid Restoration System\n";
    cout << "Illustrative virtual network; repair values are simulated.\n";
    int choice;
    do {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Please enter a number from 0 to 10.\n";
            continue;
        }
        switch (choice) {
            case 1: displayGrid(); break;
            case 2: generateRandomDamage(); break;
            case 3: showPowerStatus(); break;
            case 4: showRepairPriorities(); break;
            case 5: repairNextLine(); break;
            case 6: introduceNewFailure(); break;
            case 7: resetGrid(); break;
            case 8: searchLine(); break;
            case 9: sortDamagedLines(); break;
            case 10: showCityTree(); break;
            case 0: cout << "Exiting restoration simulator.\n"; break;
            default: cout << "Invalid choice. Choose 0-10.\n";
        }
    } while (choice != 0);
    return 0;
}
