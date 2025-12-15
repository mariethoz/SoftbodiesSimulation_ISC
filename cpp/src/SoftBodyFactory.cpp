#include "SoftBody.h"

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <algorithm>

using namespace sim;

static double PI = 3.14;

// ---------------------------------------------------------------------------
// Hashing for deduplication
// ---------------------------------------------------------------------------

struct VecNode {
    Vector2 point;
    int id;
    VecNode* next = nullptr;
    VecNode(Vector2 p, int i): point(p), id(i) {}
};

class VecList {
    double eps;
    VecNode* root = nullptr;

    bool VecEq(const Vector2& a, const Vector2& b) const {
        return (a - b).length() <= eps;
    }

    bool exist(VecNode* node, const Vector2& p) {
        if (VecEq(node->point, p)) return true;
        if (!node->next) return false;
        return exist(node->next, p);
    }
    int find(VecNode* node, const Vector2& p, int p_id) {
        if (VecEq(node->point, p)) {
            return node->id;
        }
        if (!node->next) {
            node->next = new VecNode(p, p_id);
            return p_id;
        }
        return find(node->next, p, p_id);
    }
    void clear(VecNode* node) {
        if (node->next) clear(node);
        delete node;
    }
public:
    VecList(double tolerance) : eps(tolerance) {}

    bool exist(const Vector2& p){
        if (!root) return false;
        return exist(root, p);
    }
    int find(const Vector2& p, int p_id) {
        if (!root) {
            root = new VecNode(p, p_id);
            return p_id;
        }
        return find(root, p, p_id);
    }
    void set_eps(const double& e) { eps = e; }
    void clear() {
        if (!root) return;
        clear(root);
        delete root;
    }
};


static int getID(
    const Vector2& p,
    VecList& idmap,
    std::vector<Vector2>& pts)
{
    int id = (int)pts.size();
    int it = idmap.find(p, id);
    if (it == id) pts.push_back(p);
    return it;
}

static bool existId(
    const Vector2& p,
    VecList& idmap)
{
    return idmap.exist(p);
}


struct Edge {
    int a, b;
    bool operator==(const Edge& o) const { return a == o.a && b == o.b; }
};

struct EdgeHash {
    size_t operator()(const Edge& e) const noexcept {
        size_t h1 = std::hash<int>()(e.a);
        size_t h2 = std::hash<int>()(e.b);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

// ---------------------------------------------------------------------------
// Convex polygon point-in-test (fast): check if always same sign
// ---------------------------------------------------------------------------
static bool pointInConvexPolygon(const Vector2& p, const std::vector<Vector2>& poly) {
    int n = (int)poly.size();
    if (n < 3) return false;

    // Determine sign using first non-zero cross
    double sign = 0.0;
    for (int i = 0; i < n; ++i) {
        const Vector2& a = poly[i];
        const Vector2& b = poly[(i + 1) % n];
        Vector2 edge = b - a;
        Vector2 ap = p - a;
        double c = edge.cross(ap);
        if (std::abs(c) > 1e-9) {
            if (sign == 0.0) sign = c;
            else if (sign * c < 0.0) return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Check if polygon vertices are ordered clockwise
// ---------------------------------------------------------------------------
inline bool isClockwise(const std::vector<Vector2>& poly) {
    double sum = 0.0;
    int n = (int)poly.size();
    for (int i = 0; i < n; ++i) {
        const Vector2& a = poly[i];
        const Vector2& b = poly[(i + 1) % n];
        sum += (b.x - a.x) * (b.y + a.y);
    }
    double shoelace = 0.0;
    for (int i=0;i<n;++i) {
        const Vector2& a = poly[i];
        const Vector2& b = poly[(i+1)%n];
        shoelace += (a.x * b.y) - (b.x * a.y);
    }
    return shoelace < 0;
}


// ---------------------------------------------------------------------------
// Create mesh of predefined point
// ---------------------------------------------------------------------------
static void meshPolygone(
    VecList& idmap,
    std::vector<Vector2>& pts,
    std::unordered_set<Edge, EdgeHash>* edgeSet,
    const std::vector<Vector2>& polygon,
    const std::vector<std::vector<Vector2>>& segments,
    double spacing, double grid_spacing, double border_spacing
)
{
    // ---- 1. Build ordered border point list ----
    std::vector<Vector2> ring;
    for (auto& seg : segments)
        for (int i = 0; i < seg.size() - 1; i++)
            ring.push_back(seg[i]);
    if (ring.size() < 3) return;
    // ---- 2. Loop and build inward rings ----
    bool border = true;
    bool is_clockwise = isClockwise(ring);
    double maxAngle = 120.0 * PI / 180.0; // convert to radians
    idmap.set_eps(grid_spacing);
    int loop = 3;
    while (ring.size() > 3) {//(loop > 0) {
        loop--;
        std::vector<Vector2> nextRing;
        std::vector<int> nextRingIdx;
        int N = ring.size();
        std::cout << "Ring size " << N << "\n";

        Vector2 AB;
        Vector2 CB;
        int ip;
        int in;
        for (int i = 0; i < N; i++) {
            ip = (i - 1 + N) % N;  // previous index
            in = (i + 1) % N;      // next index
            auto a = ring[ip];
            auto b = ring[i];
            auto c = ring[in];

            AB = (a - b).normalized();
            CB = (c - b).normalized();

            double dot = AB.dot(CB);
            dot = std::clamp(dot, -1.0, 1.0);
            double angle = std::acos(dot);
            // Example: stop if angle < 120 degrees
            if (angle < maxAngle && !border) {
                int A1 = getID(a, idmap, pts);
                int A2 = getID(c, idmap, pts);
                edgeSet->insert({std::min(A1,A2), std::max(A1,A2)});
                continue;
            }

            // Bisector direction
            Vector2 bisector = (AB + CB).normalized();
            if (bisector.length() < 1e-6) {
                if (!is_clockwise)
                    bisector = Vector2(-AB.y,AB.x);
                else
                    bisector = Vector2(AB.y,-AB.x);
            }
            bool inside = !is_clockwise ? AB.cross(CB) < 0 : AB.cross(CB) > 0;
            if (!inside) bisector = -bisector;

            auto pIn = b + spacing * bisector;
            if (!existId(pIn, idmap)) {
                getID(pIn, idmap, pts);
                nextRing.push_back(pIn);
                nextRingIdx.push_back(i);
            } else if(nextRing.size() > 0) {
                nextRing.push_back(nextRing.back());
                nextRingIdx.push_back(i);
            }
        }
        if (border) {
            for (int j = 0; j < nextRingIdx[0]; j++){
                nextRing.push_back(nextRing.back());
                nextRingIdx.push_back(j);
            }
        }
        std::cout << "nRing size " << nextRing.size() << "\n";
        if (ring.size() < 3) break;
        // ---- 3. add edges between rings ----
        int nN = nextRingIdx.size();
        for (int i = 0; i < nN; i++) {
            int rip = (nextRingIdx[i] - 1 + N) % N;  // previous index
            int ri = nextRingIdx[i];
            int rin = (nextRingIdx[i] + 1 + N) % N;      // next index
            Vector2 a1 = ring[rip];
            Vector2 a2 = ring[ri];
            Vector2 a3 = ring[rin];
            int ip = (i - 1 + nN) % nN;  // previous index
            int in = (i + 1 + nN) % nN;      // next index
            Vector2 b1 = nextRing[ip];
            Vector2 b2 = nextRing[i];
            Vector2 b3 = nextRing[in];
            
            if (border) idmap.set_eps(border_spacing); 
            int A1 = getID(a1, idmap, pts);
            int A2 = getID(a2, idmap, pts);
            int A3 = getID(a3, idmap, pts);
            if (border) idmap.set_eps(grid_spacing);
            int B1 = getID(b1, idmap, pts);
            int B2 = getID(b2, idmap, pts);
            int B3 = getID(b3, idmap, pts);

            edgeSet->insert({std::min(A2,B1), std::max(A2,B1)});
            edgeSet->insert({std::min(A2,B2), std::max(A2,B2)});
            edgeSet->insert({std::min(A2,B3), std::max(A2,B3)});
            if (border){
                edgeSet->insert({std::min(A1,B1), std::max(A1,B1)});
                edgeSet->insert({std::min(A1,B2), std::max(A1,B2)});
                edgeSet->insert({std::min(A3,B2), std::max(A3,B2)});
                edgeSet->insert({std::min(A3,B3), std::max(A3,B3)});
            }
            edgeSet->insert({std::min(B1,B2), std::max(B1,B2)});
            edgeSet->insert({std::min(B2,B3), std::max(B2,B3)});

        }
        ring.clear();
        for (auto p: nextRing){
            auto it = std::find(ring.begin(), ring.end(), p);
            if (it == ring.end()){
                ring.push_back(p);
            }
        }
        nextRingIdx.clear();
        nextRing.clear();
        if (border) {
            border = false;
            idmap.set_eps(grid_spacing);
        }
    }
    std::cout << "Ring size " << ring.size() << " end\n";
}


// ---------------------------------------------------------------------------
// High-level function: create SoftBody from polygon
// ---------------------------------------------------------------------------

SoftBody* SoftBody::createFromPolygon(
    const std::vector<Vector2>& polygon, int mesh_unit,
    double mass, double radius,
    double stiffness, double damping,
    double friction, double restitution,
    bool is_pinned
)
{
    std::vector<Particle*> _particles;
    std::vector<Constraint*> _constraints;
    std::vector<Particle*> _border;

    std::vector<Vector2> pts;
    std::vector<std::vector<Vector2>> segments;

    double grid_space = mesh_unit*0.9;
    double border_space = radius *0.9;
    std::unordered_set<Edge, EdgeHash> edgeSet;
    VecList idmap(border_space);
    idmap.clear();
    
    // 1) Create subdivided border points
    for (int i = 0; i < (int)polygon.size(); ++i) {
        Vector2 p1 = polygon[i];
        Vector2 p2 = polygon[(i + 1) % polygon.size()];
        double L = dist(p1, p2);
        int n = std::max(1, (int)std::floor(L / double(radius))); // number of subdivisions
        std::vector<Vector2> seg = divideSegment(p1, p2, n);
        segments.push_back(seg);
        // divideSegment gives n+1 points along the segment including endpoints
        for (int j = 0; j < seg.size() - 1; ++j) {
            Vector2 s1 = seg[j];
            Vector2 s2 = seg[j+1];
            // add s1 and s2 (getID will dedupe)
            int id1 = getID(s1, idmap, pts);
            int id2 = getID(s2, idmap, pts);

            // Add edge along the boundary
            Edge e{ std::min(id1,id2), std::max(id1,id2) };
            edgeSet.insert(e);
        }
    }

    // 2) Generate interior grid and edges
    meshPolygone(idmap, pts, &edgeSet, polygon, segments, mesh_unit, grid_space, border_space);

    // 3) Add centroid constraint
    int c_idx = pts.size()-1;
    for (auto seg: segments) {
        for (int j = 0; j < seg.size(); ++j) {
            Vector2 s1 = seg[j];
            int id1 = getID(s1, idmap, pts);

            // Add edge to the boundary
            Edge e{ std::min(id1,c_idx), std::max(id1,c_idx) };
            edgeSet.insert(e);
        }
    }

    // 4) Build particles for all pts
    for (auto& p : pts) {
        _particles.push_back(new Particle(p, mass, radius, is_pinned));
    }
    idmap.set_eps(border_space);
    for (auto& p: polygon) {
        auto idx = getID(p, idmap, pts);
        _border.push_back(_particles[idx]);
    }
    // 6) Convert edgeSet into constraints
    for (auto e : edgeSet) {
        _constraints.push_back(new Constraint(_particles[e.a], _particles[e.b], stiffness, damping));
    }
    return new SoftBody(_border, _particles, _constraints, friction, restitution, mesh_unit);
}
