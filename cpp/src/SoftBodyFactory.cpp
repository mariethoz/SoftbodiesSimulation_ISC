#include "SoftBody.h"


#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <corecrt_math_defines.h>
#include <algorithm>

using namespace sim;


// ---------------------------------------------------------------------------
// Hashing for deduplication
// ---------------------------------------------------------------------------

static constexpr double POINT_EPS = 4; // quantize at 4 units

struct VecHash {
    size_t operator()(const Vector2& v) const noexcept {
        // quantize coordinates to integer bins
        long long xi = (long long)std::llround(v.x / POINT_EPS);
        long long yi = (long long)std::llround(v.y / POINT_EPS);
        // combine
        size_t h1 = std::hash<long long>()(xi);
        size_t h2 = std::hash<long long>()(yi);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

struct VecEq {
    bool operator()(const Vector2& a, const Vector2& b) const noexcept {
        return std::abs(a.x - b.x) <= POINT_EPS && std::abs(a.y - b.y) <= POINT_EPS;
    }
};

static int getID(
    const Vector2& p,
    std::unordered_map<Vector2,int,VecHash,VecEq>& idmap,
    std::vector<Vector2>& pts)
{
    auto it = idmap.find(p);
    if (it != idmap.end()) return it->second;

    int id = (int)pts.size();
    pts.push_back(p);
    idmap[p] = id;
    return id;
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
// Divide a segment into n equal subdivisions
// ---------------------------------------------------------------------------
static std::vector<Vector2> divideSegment(const Vector2& P1, const Vector2& P2, int n) {
    std::vector<Vector2> out;
    if (P1 == P2) {
        out.push_back(P1);
        return out;
    }
    if (n <= 0) {
        out.push_back(P1);
        out.push_back(P2);
        return out;
    }

    for (int i = 0; i <= n; ++i) {
        double t = double(i) / double(n);
        out.push_back(interpolate(P1, P2, t));
    }
    return out;
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
    std::unordered_map<Vector2,int,VecHash,VecEq>& idmap,
    std::vector<Vector2>& pts,
    std::unordered_set<Edge, EdgeHash>* edgeSet,
    const std::vector<Vector2>& polygon,
    const std::vector<std::vector<Vector2>>& segments,
    double spacing
)
{
    // ---- 1. Build ordered border point list ----
    std::vector<Vector2> ring;
    for (auto& seg : segments)
        for (int i = 0; i < seg.size() - 1; i++)
            ring.push_back(seg[i]);
    if (ring.size() < 3) return;
    // ---- 2. Loop and build inward rings ----
    int n_r = 0;
    while (ring.size() > 3) {
        std::vector<Vector2> nextRing;
        std::vector<int> nextRingIdx;
        int N = ring.size();
        bool is_clockwise = isClockwise(ring);

        for (int i = 0; i < N; i++) {
            int ip = (i - 1 + N) % N;  // previous index
            int in = (i + 1) % N;      // next index

            auto a = ring[ip];
            auto b = ring[i];
            auto c = ring[in];

            auto AB = (a - b).normalized();
            auto BC = (c - b).normalized();

            double dot = AB.dot(BC);
            dot = std::clamp(dot, -1.0, 1.0);
            double angle = std::acos(dot);

            // Example: stop if angle < 120 degrees
            double maxAngle = 120.0 * M_PI / 180.0; // convert to radians
            if (angle < maxAngle) {
                int A1 = getID(a, idmap, pts);
                int A2 = getID(c, idmap, pts);
                edgeSet->insert({std::min(A1,A2), std::max(A1,A2)});
                continue;
            }

            // Bisector direction
            auto bisector = (AB + BC);
            if (bisector.length() < 1e-6) {
                bisector = Vector2(-AB.y,AB.x);
                if (!is_clockwise) {
                    bisector = -bisector;
                }
            }

            auto pIn = b + spacing * bisector.normalized();

            if (pointInConvexPolygon(pIn, ring)) {
                nextRing.push_back(pIn);
                nextRingIdx.push_back(i);
            }
        }

        if (nextRing.size() < 3) break;

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

            int A1 = getID(a1, idmap, pts);
            int A2 = getID(a2, idmap, pts);
            int A3 = getID(a3, idmap, pts);
            int B1 = getID(b1, idmap, pts);
            int B2 = getID(b2, idmap, pts);
            int B3 = getID(b3, idmap, pts);

            edgeSet->insert({std::min(A1,B1), std::max(A1,B1)});
            //edgeSet->insert({std::min(A1,B2), std::max(A1,B2)});
            edgeSet->insert({std::min(A2,B1), std::max(A2,B1)});
            edgeSet->insert({std::min(A2,B2), std::max(A2,B2)});
            //edgeSet->insert({std::min(A2,B3), std::max(A2,B3)});
            edgeSet->insert({std::min(A3,B2), std::max(A3,B2)});
            //edgeSet->insert({std::min(A3,B3), std::max(A3,B3)});
            edgeSet->insert({std::min(B1,B2), std::max(B1,B2)});
            edgeSet->insert({std::min(B2,B3), std::max(B2,B3)});
        }
        ring.clear();
        nextRingIdx.clear();
        for (auto nr: nextRing){
            int i = getID(nr, idmap, pts);
            if (std::find(nextRingIdx.begin(), nextRingIdx.end(), i) == nextRingIdx.end()) {
                ring.push_back(pts[i]);
                nextRingIdx.push_back(i);
            }
        }
        nextRingIdx.clear();
    }
}


// ---------------------------------------------------------------------------
// High-level function: create SoftBody from polygon
// ---------------------------------------------------------------------------

SoftBody* SoftBody::createFromPolygon(
    const std::vector<Vector2>& polygon, Vector2 centroid, int u,
    double mass, double radius,
    double stiffness, double damping,
    double friction, double restitution)
{
    std::unordered_set<Edge, EdgeHash> edgeSet;
    std::unordered_map<Vector2,int,VecHash,VecEq> idmap;
    std::vector<Constraint*> _constraints;
    std::vector<Particle*> _particles;
    std::vector<Vector2> pts;
    std::vector<std::vector<Vector2>> segments;

    
    // 1) Create subdivided border points
    for (int i = 0; i < (int)polygon.size(); ++i) {
        Vector2 p1 = polygon[i];
        Vector2 p2 = polygon[(i + 1) % polygon.size()];
        double L = dist(p1, p2);
        int n = std::max(1, (int)std::floor(L / double(u))); // number of subdivisions
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
    meshPolygone(idmap, pts, &edgeSet, polygon, segments, u);

    // // 3) Add centroid constraint
    // int c_idx = getID(centroid, idmap, pts);
    // for (auto seg: segments) {
    //     for (int j = 0; j < seg.size(); ++j) {
    //         Vector2 s1 = seg[j];
    //         int id1 = getID(s1, idmap, pts);

    //         // Add edge to the boundary
    //         Edge e{ std::min(id1,c_idx), std::max(id1,c_idx) };
    //         edgeSet.insert(e);
    //     }
    // }

    // // 4) Add particle contours
    // for (auto seg: segments) {
    //     for (int j = 0; j < seg.size() - 1; ++j) {
    //         Vector2 c1 = seg[j];
    //         Vector2 c2 = seg[j+1];
    //         std::vector<Vector2> contour = divideSegment(c1, c2, u-1);
    //         for (int i = 0; i < contour.size() - 1; ++i) {
    //             Vector2 s1 = contour[i];
    //             Vector2 s2 = contour[i+1];
    //             // add s1 and s2 (getID will dedupe)
    //             int id1 = getID(s1, idmap, pts);
    //             int id2 = getID(s2, idmap, pts);

    //             // Add edge along the boundary
    //             Edge e{ std::min(id1,id2), std::max(id1,id2) };
    //             edgeSet.insert(e);
    //         }
    //         // add s1 and s2 (getID will dedupe)
    //         int id1 = getID(contour.back(), idmap, pts);
    //         int id2 = getID(c2, idmap, pts);

    //         // Add edge along the boundary
    //         Edge e{ std::min(id1,id2), std::max(id1,id2) };
    //         edgeSet.insert(e);
    //     }
    // }


    // 5) Build particles for all pts
    _particles.reserve(pts.size());
    for (auto &p : pts) {
        _particles.push_back(new Particle(p, mass, radius));
    }

    // 6) Convert edgeSet into constraints
    for (auto e : edgeSet) {
        _constraints.push_back(new Constraint(_particles[e.a], _particles[e.b], stiffness, damping));
    }

    return new SoftBody(_particles, _constraints, friction, restitution);
}