/*
 * Created by ckalo on 5/19/26.
 */

#include "camera.h"
#include <math.h>

#include "smath.h"

/*
 * let camera constructor to take a projection matrix(float projmat[16])
 * so that user passes different params for ortho and persective
 */

struct camera_projection_matrix scene_create_perspective_projection_matrix(const float fov, const float near,
                                                             const float far, const float aspect)
{
    // Convert FOV to radians and calculate
    float f = 1.0f / tan(fov * (PI / 360.0f));  // cot(fov/2)
    struct camera_projection_matrix mat = {
        .projection_matrix = {
            f / aspect,   0.0f,    0.0f,                               0.0f,
            0.0f,         f,       0.0f,                               0.0f,
            0.0f,         0.0f,   -(far + near) / (far - near),       -1.0f,
            0.0f,         0.0f,   -(2.0f * far * near) / (far - near), 0.0f
        }
    };
    return mat;
}

struct camera_projection_matrix scene_create_orthographic_projection_matrix(const float left, const float right, const float top,
                                         const float bottom, const float near, const float far)
{
    struct camera_projection_matrix mat = {
        .projection_matrix = {
            // 2.0f/(right-left), 0.0f,              0.0f,             -(right+left)/(right-left),
            // 0.0f,              2.0f/(top-bottom), 0.0f,             -(top+bottom)/(top-bottom),
            // 0.0f,              0.0f,              -2.0f/(far-near), -(far+near)/(far-near),
            // 0.0,               0.0,               0.0,              1.0f,
            2.0f/(right-left), 0.0f,               0.0f,            0.0,
            0.0f,              2.0f/(top-bottom),  0.0f,            0.0,
            0.0f,              0.0f,              -2.0f/(far-near), 0.0,
            -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far+near)/(far-near), 1.0f
        }
    };
    return mat;
}

struct camera camera_create(struct camera_projection_matrix projection_matrix, const bool is_column_major)
{
    struct camera camera = {
        .projection_matrix = projection_matrix,
        .is_column_major = is_column_major,
        // vec3f wpos;
        // vec3f front;
        // vec3f up;
        // vec3f right;
        // vec3f wup;
        // float Yaw;
        // float Pitch;
        // float move_speed;
        // float mouse_sensitivity;
        // float zoom;
    };
    return camera;
}