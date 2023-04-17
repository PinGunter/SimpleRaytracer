#include <vec3.h>
#include <color.h>
#include <ray.h>
#include <iostream>
#include <fstream>

point3 sphere(0, 0, -1);
double sphere_radius = 0.5;

double hit_sphere(const point3 &center, double radius, const ray &r) {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

color ray_color(const ray &r) {
    double t = hit_sphere(sphere, sphere_radius, r);
    if (t > 0.0) {
        vec3 normal = unit_vector(r.at(t) - sphere);
        return 0.5 * color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const double image_width = 3500;
    const double image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    point3 origin = point3(0, 0, 0);
    vec3 horizontal = vec3(viewport_width, 0, 0);
    vec3 vertical = vec3(0, viewport_height, 0);
    point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Render
    std::ofstream output_file("output/image.ppm");
    if (output_file.good()) {
        output_file << "P3\n"
                    << image_width << ' ' << image_height << "\n255\n";

        for (int j = image_height - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                double u = double(i) / (image_width - 1);
                double v = double(j) / (image_height - 1);
                ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
                color pixel_color = ray_color(r);
                write_color(output_file, pixel_color);
            }
        }

        output_file.close();

        std::cerr << "\nDone.\n";
    } else {
        std::cerr << "Error opening file\n";
    }

}