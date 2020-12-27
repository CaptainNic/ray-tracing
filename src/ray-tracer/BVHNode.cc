#include "BVHNode.h"

namespace rt {
    inline bool boxCompare(const std::shared_ptr<IHittable> a, const std::shared_ptr<IHittable> b, int axis) {
        AABB boxA;
        AABB boxB;

        // Test for any items that don't have a bounding box (e.g.: infinite plane).
        if (!a->boundingBox(0, 0, boxA) || !b->boundingBox(0, 0, boxB)) {
            std::cerr << "No bounding box in BVHNode::boxCompare." << std::endl;
        }

        return boxA.min()[axis] < boxB.min()[axis];
    }

    bool boxCompareX(const std::shared_ptr<IHittable> a, const std::shared_ptr<IHittable> b) {
        return boxCompare(a, b, 0);
    }

    bool boxCompareY(const std::shared_ptr<IHittable> a, const std::shared_ptr<IHittable> b) {
        return boxCompare(a, b, 1);
    }

    bool boxCompareZ(const std::shared_ptr<IHittable> a, const std::shared_ptr<IHittable> b) {
        return boxCompare(a, b, 2);
    }

    BVHNode::BVHNode()
    {
    }

    BVHNode::BVHNode(
        const std::vector<std::shared_ptr<IHittable>>& objects, 
        size_t start, size_t end, double t0, double t1
    ) {
        // Method for producing decently balanced BVH tree, where each
        // sub-list has a smaller bounding box than the parent.
        // 1. Choose random axis
        // 2. Sort objects on axis
        // 3. Split sorted list into each sub-tree

        // Create modifiable array of the source objects
        // TODO: Since we only care about the entries from start -> end,
        //   this could probably be optimized.
        auto objs = objects;

        auto axis = randInt(0, 2);
        auto comparator = (axis == 0) ? boxCompareX : (axis == 1) ? boxCompareY : boxCompareZ;

        size_t numObjs = end - start;
        if (numObjs == 1) {
            // Both children are same
            m_left = m_right = objs[start];
        }
        else if (numObjs == 2) {
            // Manually sort
            if (comparator(objs[start], objs[start + 1])) {
                m_left = objs[start];
                m_right = objs[start + 1];
            }
            else {
                m_left = objs[start + 1];
                m_right = objs[start];
            }
        }
        else {
            // Sort and split
            std::sort(objs.begin() + start, objs.begin() + end, comparator);
            auto mid = start + (numObjs / 2);
            m_left = std::make_shared<BVHNode>(objs, start, mid, t0, t1);
            m_right = std::make_shared<BVHNode>(objs, mid, end, t0, t1);
        }

        AABB boxLeft;
        AABB boxRight;

        // Test for any items that don't have a bounding box (e.g.: infinite plane).
        if (!m_left->boundingBox(t0, t1, boxLeft) || !m_right->boundingBox(t0, t1, boxRight)) {
            std::cerr << "No bounding box in BVHNode constructor." << std::endl;
        }

        m_box = AABB::surroundingBox(boxLeft, boxRight);
    }

    bool BVHNode::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
    {
        if (!m_box.hit(r, tMin, tMax)) {
            return false;
        }

        // hitLeft feeds into the hitRight calc.
        auto hitLeft = m_left->hit(r, tMin, tMax, rec);
        auto hitRight = m_right->hit(r, tMin, hitLeft ? rec.t : tMax, rec);
        
        return hitLeft || hitRight;
    }

    bool BVHNode::boundingBox(double t0, double t1, AABB& outBox) const
    {
        outBox = m_box;
        return true;
    }
}