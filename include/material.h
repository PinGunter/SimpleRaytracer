//
// Created by pingu on 04/05/2023.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <rtweekend.h>

struct hit_record;

class material {
public:
    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const = 0;
};

class lambertian : public material {
public:
    color albedo;

    lambertian(const color &a) : albedo(a) {}

    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const override {
        vec3 scatter_direction = rec.normal + random_unit_vector();

        // in case we are unlucky and the random unit vector is exactly the opposite
        // direction of the normal
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class metal : public material {
public:
    color albedo;

    metal(const color &a) : albedo(a) {}

    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

};

#endif //RAYTRACER_MATERIAL_H
