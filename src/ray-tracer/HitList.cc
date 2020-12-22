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
}