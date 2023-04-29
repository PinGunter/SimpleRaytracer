#include <rtweekend.h>
#include <color.h>
#include <sphere.h>
#include <hittable_list.h>
#include <camera.h>

#include <fstream>
#include <iostream>


color ray_color(const ray &r, const hittable &world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // Camera
    camera cam;


    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Render
    std::ofstream output_file("output/image.ppm");
    if (output_file.good()) {
        output_file << "P3\n"
                    << image_width << ' ' << image_height << "\n255\n";

        for (int j = image_height - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int k = 0; k < samples_per_pixel; ++k) {
                    double u = (i + random_double()) / (image_width - 1);
                    double v = (j + random_double()) / (image_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world);
                }
                write_color(output_file, pixel_color, samples_per_pixel);
            }
        }

        output_file.close();

        std::cerr << "\nDone.\n";
    } else {
        std::cerr << "Error opening file\n";
    }

}