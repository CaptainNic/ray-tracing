#pragma once

#include "HitList.h"
#include "hittable.h"

namespace rt {

    /// <summary>
    /// Bounding Volume Hierarchy
    ///  - a binary tree of bounding boxes for list of objects
    /// </summary>
    class BVHNode : public IHittable {
    public:
        BVHNode();

        BVHNode(const HitList& list, double t0, double t1)
            : BVHNode(list.objects(), 0, list.objects().size(), t0, t1)
        {}

        BVHNode(
            const std::vector<std::shared_ptr<IHittable>>& objects,
            size_t start, size_t end, double t0, double t1);

        // Inherited via IHittable
        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
        virtual bool boundingBox(double t0, double t1, AABB& outBox) const override;

    private:
        // Child nodes
        std::shared_ptr<IHittable> m_left;
        std::shared_ptr<IHittable> m_right;
        
        // Bounding box containing child nodes.
        AABB m_box;
    };
}