#include <rtweekend.h>
#include <color.h>
#include <sphere.h>
#include <hittable_list.h>
#include <camera.h>
#include <material.h>
#include <fstream>
#include <iostream>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

color ray_color(const ray &r, const hittable &world, int depth) {
    if (depth <= 0) return color(0, 0, 0);
    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    std::vector<color> image(image_width * image_height);
    const int samples_per_pixel = 200;
    const int max_depth = 50;
    // Camera
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 90, aspect_ratio);

    // Render
    auto start = high_resolution_clock::now();
    std::ofstream output_file("output/image.ppm");
    if (output_file.good()) {
        output_file << "P3\n"
                    << image_width << ' ' << image_height << "\n255\n";
#pragma omp parallel for shared(image_height, image_width, image, world, cam), default(none), collapse(2)
        for (int j = image_height - 1; j >= 0; --j) {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int k = 0; k < samples_per_pixel; ++k) {
                    double u = (i + random_double()) / (image_width - 1);
                    double v = (j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world, max_depth);
                }
                image[(image_height - j - 1) * image_width + i] = pixel_color;
            }
        }
        write_color(output_file, image, samples_per_pixel);
        output_file.close();
        auto end = high_resolution_clock::now();
        auto time = duration_cast<std::chrono::milliseconds>(end - start);
        std::cerr << "\nDone.\n";
        std::cerr << "\nTook: " << time.count() / 1000.0 << " seconds\n";
    } else {
        std::cerr << "Error opening file\n";
    }

}