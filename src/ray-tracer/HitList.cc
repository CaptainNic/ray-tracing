#pragma once

#include "HitList.h"

namespace rt {
    bool HitList::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
        HitRecord tempRec;
        bool anyHits = false;
        auto closestSoFar = tMax;

        // Find closest hit by tightening tMax as we go through the list.
        for (const auto& obj : m_objects) {
            if (obj->hit(r, tMin, closestSoFar, tempRec)) {
                anyHits = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return anyHits;
    }

    bool HitList::boundingBox(double t0, double t1, AABB& outBox) const
    {
        if (m_objects.empty()) {
            return false;
        }

        AABB tempBox;
        bool firstBox = true;
        for (const auto& obj : m_objects) {
            if (!obj->boundingBox(t0, t1, tempBox)) {
                // TODO: should we continue here instead? What if we have
                //   a child that doesn't have a bounding box like an empty HitList?
                return false;
            }

            // Keep growing the box.
            outBox = firstBox ? tempBox : AABB::surroundingBox(outBox, tempBox);
            firstBox = false;
        }

        return true;
    }
}