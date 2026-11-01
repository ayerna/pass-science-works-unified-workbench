#pragma once
// C++ parsers/validators for ASCII STL and ANSYS-style nodal CSV.
// Both reject malformed and non-finite data with an actionable message.
#include <array>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace wb {

struct Triangle {
    std::array<double, 3> normal{};
    std::array<std::array<double, 3>, 3> v{};
};

struct NodeResult {
    long long id = 0;
    double x = 0, y = 0, z = 0, temp = 0;
};

inline bool finite3(const std::array<double, 3>& p) {
    return std::isfinite(p[0]) && std::isfinite(p[1]) && std::isfinite(p[2]);
}

inline std::string trim(const std::string& s) {
    const auto a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    const auto b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

inline std::vector<Triangle> parse_stl_ascii(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open STL file: " + path);
    std::vector<Triangle> tris;
    std::string line;
    if (!std::getline(in, line) || trim(line).rfind("solid", 0) != 0)
        throw std::runtime_error("STL must start with 'solid <name>' (ASCII STL only)");
    bool in_facet = false, in_loop = false;
    Triangle cur{};
    int vcount = 0;
    while (std::getline(in, line)) {
        std::string t = trim(line);
        std::istringstream ss(t);
        std::string kw;
        ss >> kw;
        if (kw == "facet") {
            if (in_facet) throw std::runtime_error("STL: nested facet");
            std::string normal_kw;
            ss >> normal_kw;
            if (normal_kw != "normal" || !(ss >> cur.normal[0] >> cur.normal[1] >> cur.normal[2]))
                throw std::runtime_error("STL: malformed 'facet normal ni nj nk' line");
            if (!finite3(cur.normal)) throw std::runtime_error("STL: non-finite facet normal");
            in_facet = true; vcount = 0;
        } else if (kw == "outer") {
            if (!in_facet) throw std::runtime_error("STL: 'outer loop' outside a facet");
            in_loop = true;
        } else if (kw == "vertex") {
            if (!in_loop || vcount >= 3) throw std::runtime_error("STL: vertex outside loop or too many vertices");
            if (!(ss >> cur.v[vcount][0] >> cur.v[vcount][1] >> cur.v[vcount][2]))
                throw std::runtime_error("STL: malformed vertex line");
            if (!finite3(cur.v[vcount])) throw std::runtime_error("STL: non-finite vertex");
            ++vcount;
        } else if (kw == "endloop") {
            if (vcount != 3) throw std::runtime_error("STL: facet does not have exactly 3 vertices");
            in_loop = false;
        } else if (kw == "endfacet") {
            if (in_loop) throw std::runtime_error("STL: endfacet before endloop");
            tris.push_back(cur); in_facet = false;
        } else if (kw == "endsolid") {
            if (in_facet) throw std::runtime_error("STL: endsolid inside a facet");
            if (tris.empty()) throw std::runtime_error("STL: file contains no triangles (empty geometry)");
            return tris;
        } else if (kw.empty()) {
            continue;
        } else {
            throw std::runtime_error("STL: unexpected line: " + t);
        }
    }
    throw std::runtime_error("STL: missing 'endsolid' (truncated file)");
}

inline std::vector<NodeResult> parse_ansys_csv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open CSV file: " + path);
    std::string header;
    if (!std::getline(in, header)) throw std::runtime_error("CSV: empty file, no header row");
    if (trim(header) != "node_id,x_m,y_m,z_m,temperature_C")
        throw std::runtime_error("CSV: header must be exactly node_id,x_m,y_m,z_m,temperature_C");
    std::vector<NodeResult> nodes;
    std::string line;
    long long lineno = 1;
    while (std::getline(in, line)) {
        ++lineno;
        if (trim(line).empty()) continue;
        std::istringstream ss(line);
        std::string f[5];
        for (int i = 0; i < 5; ++i) {
            if (!std::getline(ss, f[i], ',')) throw std::runtime_error("CSV line " + std::to_string(lineno) + ": expected 5 columns");
        }
        std::string extra;
        if (std::getline(ss, extra)) throw std::runtime_error("CSV line " + std::to_string(lineno) + ": too many columns");
        NodeResult r;
        try {
            std::size_t used = 0;
            r.id = std::stoll(trim(f[0]), &used);
            if (used != trim(f[0]).size()) throw std::invalid_argument("bad id");
            r.x = std::stod(trim(f[1])); r.y = std::stod(trim(f[2]));
            r.z = std::stod(trim(f[3])); r.temp = std::stod(trim(f[4]));
        } catch (...) {
            throw std::runtime_error("CSV line " + std::to_string(lineno) + ": non-numeric value");
        }
        if (!std::isfinite(r.x) || !std::isfinite(r.y) || !std::isfinite(r.z) || !std::isfinite(r.temp))
            throw std::runtime_error("CSV line " + std::to_string(lineno) + ": non-finite value");
        nodes.push_back(r);
    }
    if (nodes.empty()) throw std::runtime_error("CSV: header only, no data rows (empty result set)");
    return nodes;
}

}  // namespace wb
