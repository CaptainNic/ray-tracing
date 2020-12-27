#pragma once

#include <array>

#include "vec3.h"

namespace rt {
    template<int PointCount>
    class Perlin {
    public:
        Perlin() {
            for (auto& entry : m_randVec) {
                entry = unit_vector(Vec3::random(-1, 1));
            }

            GeneratePerm(m_permX);
            GeneratePerm(m_permY);
            GeneratePerm(m_permZ);
        }

        double noise(const Point3& p) const {
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            auto i = static_cast<int>(floor(p.x()));
            auto j = static_cast<int>(floor(p.y()));
            auto k = static_cast<int>(floor(p.z()));
            Vec3 c[2][2][2];

            // Determine noise values for point and surrounding indices,
            // then perform trilinear filtering to smooth it out.
            for (int di = 0; di < 2; di++) {
                for (int dj = 0; dj < 2; dj++) {
                    for (int dk = 0; dk < 2; dk++) {
                        auto index = m_permX[(i + di) & 255]
                                   ^ m_permY[(j + dj) & 255]
                                   ^ m_permZ[(k + dk) & 255];
                        c[di][dj][dk] = m_randVec[index];
                    }
                }
            }

            return PerlinInterpolation(c, u, v, w);
        }

        double turbulence(const Point3& p, int depth = 7) const {
            auto accumulator = 0.0;
            auto tempPoint = p;
            auto weight = 1.0;

            // Turbulence is the sum of multiple frequencies of noise.
            for (int i = 0; i < depth; ++i) {
                accumulator += weight * noise(tempPoint);
                weight *= 0.5;
                tempPoint *= 2;
            }

            return std::fabs(accumulator);
        }

    private:
        std::array<Vec3, PointCount> m_randVec;
        std::array<int, PointCount> m_permX;
        std::array<int, PointCount> m_permY;
        std::array<int, PointCount> m_permZ;

        static void GeneratePerm(std::array<int, PointCount>& perm) {
            // Generate values from 0 -> PointCount
            for (auto i = 0; i < PointCount; ++i) {
                perm[i] = i;
            }

            // Swap values at each index w/ a random index from 0 -> index.
            for (auto i = PointCount - 1; i > 0; --i) {
                int target = randInt(0, i);
                int tmp = perm[i];
                perm[i] = perm[target];
                perm[target] = tmp;
            }
        }

        static double PerlinInterpolation(Vec3 c[2][2][2], double u, double v, double w) {
            // Use a Hermite cubic to round off interpolation.
            // Without this, the grid is still visible in some spots.
            auto uu = u * u * (3 - (2 * u));
            auto vv = v * v * (3 - (2 * v));
            auto ww = w * w * (3 - (2 * w));

            auto accumulator = 0.0;
            for (auto i = 0; i < 2; i++) {
                for (auto j = 0; j < 2; j++) {
                    for (auto k = 0; k < 2; k++) {
                        // Perlin interpolation works by using random unit vectors instead
                        // of floats for the lattice points, which results in more randomness.
                        // Additionally, a dot product is used to move the min/max off the lattice.
                        Vec3 weightV(u - i, v - j, w - k);
                        accumulator += ((i * uu) + ((1 - i) * (1 - uu)))
                                     * ((j * vv) + ((1 - j) * (1 - vv)))
                                     * ((k * ww) + ((1 - k) * (1 - ww)))
                                     * c[i][j][k].dot(weightV);
                    }
                }
            }

            return accumulator;
        }
    };
}