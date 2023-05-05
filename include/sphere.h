//
// Created by pingu on 19/04/2023.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include <vec3.h>
#include <hittable.h>

class sphere : public hittable {
private:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
public:
    sphere() {}

    sphere(point3 c, double r, shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {};

    bool hit(
            const ray &r, double t_min, double t_max, hit_record &rec) const override;

};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;
    double sqrtd = sqrt(discriminant);

    // find the nearest root that lies within tmin-tmax
    double root = (-half_b - sqrtd) / a; // first solution
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a; // second solution
        if (root < t_min || t_max < root) {
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

#endif //RAYTRACER_SPHERE_H
