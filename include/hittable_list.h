//
// Created by pingu on 19/04/2023.
//

#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include <hittable.h>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
private:
    std::vector<shared_ptr<hittable>> objects;
public:
    hittable_list() {}

    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(
            const ray &r, double t_min, double t_max, hit_record &rec) const override;

};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double current_closest = t_max;

    for (const auto &obj: objects) {
        if (obj->hit(r, t_min, current_closest, temp_rec)) {
            hit_anything = true;
            current_closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;

}

#endif //RAYTRACER_HITTABLE_LIST_H
