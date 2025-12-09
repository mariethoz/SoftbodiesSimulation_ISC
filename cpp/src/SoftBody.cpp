#include "SoftBody.h"
#include <unordered_map>
#include <array>

using namespace sim;

static std::vector<Vector2> divideSegment(const Vector2& P1, const Vector2& P2, int n)
{
    std::vector<Vector2> out;
    if (n <= 0) {
        out.push_back(P1);
        out.push_back(P2);
        return out;
    }

    out.reserve(n+1);
    for (int i=0;i<=n;i++) {
        double t = double(i) / double(n);
        out.push_back(interpolate(P1, P2, t));
    }
    return out;
}

struct VecHash {
    size_t operator()(const Vector2& v) const {
        long long X = (long long)(v.x * 1000000);
        long long Y = (long long)(v.y * 1000000);
        return (X << 32) ^ (Y & 0xffffffff);
    }
};

struct VecEq {
    bool operator()(const Vector2& a, const Vector2& b) const {
        return std::abs(a.x - b.x) < 1e-6 && std::abs(a.y - b.y) < 1e-6;
    }
};

static int getID(const Vector2& p,
                 std::unordered_map<Vector2,int,VecHash,VecEq>& map,
                 std::vector<Vector2>& outPts)
{
    auto it = map.find(p);
    if (it != map.end()) return it->second;

    int id = (int)outPts.size();
    outPts.push_back(p);
    map[p] = id;
    return id;
}


void subdivideTriangle(
    const Vector2& A, const Vector2& B, const Vector2& C,
    double u,
    std::vector<Vector2>& outPoints,
    std::vector<std::array<int,3>>& outTris)
{
    // For uniqueness
    std::unordered_map<Vector2,int,VecHash,VecEq> idmap;

    // Step 1: compute edge segment counts
    double lenAB = dist(A, B);
    double lenBC = dist(B, C);
    double lenCA = dist(C, A);

    int na = (int)std::floor(lenAB / u);
    int nb = (int)std::floor(lenBC / u);
    int nc = (int)std::floor(lenCA / u);

    auto segAB = divideSegment(A, B, na);
    auto segBC = divideSegment(B, C, nb);
    auto segCA = divideSegment(C, A, nc);

    std::vector<std::vector<Vector2>> segs = {segAB, segBC, segCA};
    std::vector<int> counts = {na, nb, nc};

    // Step 2: choose base edge = longest
    int base_idx = 0;
    if (counts[1] > counts[base_idx]) base_idx = 1;
    if (counts[2] > counts[base_idx]) base_idx = 2;

    auto base = segs[base_idx];

    auto edge1 = segs[(base_idx + 1) % 3];
    auto edge2 = segs[(base_idx + 2) % 3];
    std::reverse(edge2.begin(), edge2.end());

    // Step 3: compute triangle height
    double area = std::abs((B - A).x*(C - A).y - (B - A).y*(C - A).x) * 0.5;
    double baseLen = dist(base.front(), base.back());
    double h = (2.0 * area) / baseLen;

    // Step 4: number of floors
    double delta_h = u * std::sqrt(3.0)/2.0;
    int nFloors = (int)(h / delta_h);

    nFloors = std::min(nFloors, (int)edge1.size());
    nFloors = std::min(nFloors, (int)edge2.size());

    // Convert base to indices
    std::vector<int> baseID;
    for (auto& p : base)
        baseID.push_back(getID(p, idmap, outPoints));

    // Step 5: build floors
    for (int k = 1; k <= nFloors; k++)
    {
        double t = double(k) / double(nFloors);

        // theoretical (continuous) floor endpoints
        Vector2 Pk = interpolate(A, B, t);
        Vector2 Qk = interpolate(A, C, t);

        // snap to discrete subdivided edges
        Vector2 P_snap = nearestPoint(Pk, edge1);
        Vector2 Q_snap = nearestPoint(Qk, edge2);

        // intermediate floor
        double L = dist(P_snap, Q_snap);
        int nSeg = (int)std::floor(L / u);
        auto intermed = divideSegment(P_snap, Q_snap, nSeg);

        // Convert floor to indices
        std::vector<int> interID;
        for (auto& p : intermed)
            interID.push_back(getID(p, idmap, outPoints));

        // ---- link base and this floor by triangles ----
        int n = std::min((int)baseID.size(), (int)interID.size()) - 1;

        for (int i = 0; i < n; i++)
        {
            // Two triangles forming a quad
            outTris.push_back({   baseID[i], baseID[i+1], interID[i] });
            outTris.push_back({ baseID[i+1],  interID[i], interID[i+1] });

        }

        // use this floor for next iteration
        baseID = interID;
    }
}

SoftBody::SoftBody(std::vector<Particle *> particles, std::vector<Constraint *> constraints, double friction, double restitution)
    : particles(particles), constraints(constraints),
      friction(friction), restitution(restitution) {}

SoftBody::~SoftBody() {
    std::cout << "SoftBody destroyed\n";
};

void SoftBody::applyForce(const Vector2 &f) {
    for (auto& p : particles) {
        p->applyForce(f);
    }
}

void SoftBody::solveConstraint() {
    for (auto& c : constraints) {
        c->applyConstraint();
    }
}


void SoftBody::update(double dt) {
    for (auto& p : particles) {
        p->update(dt);
    }
}