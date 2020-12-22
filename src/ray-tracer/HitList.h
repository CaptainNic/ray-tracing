#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

namespace rt {
    class HitList : public IHittable {
    public:
        HitList() {}
        HitList(std::shared_ptr<IHittable> obj) { add(obj); }

        void clear() { m_objects.clear(); }
        void add(std::shared_ptr<IHittable> obj) { m_objects.push_back(obj); }

        virtual bool hit(const Ray&, double, double, HitRecord&) const;

    private:
        std::vector<std::shared_ptr<IHittable>> m_objects;
    };
}