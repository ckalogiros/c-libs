/*
 * Created by ckalo on 5/19/26.
 */
#ifndef GRAPHICS_TEST_CAMERA_H
#define GRAPHICS_TEST_CAMERA_H
#include "my_types.h"

struct camera_projection_matrix {
    float projection_matrix[16];
};

struct camera {
    struct camera_projection_matrix projection_matrix;
    bool is_column_major; // used in "glUniformMatrix4fv()"
    // camera Attributes
    vec3f wpos;
    vec3f front;
    vec3f up;
    vec3f right;
    vec3f wup;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float move_speed;
    float mouse_sensitivity;
    float zoom;
};

struct camera_projection_matrix
scene_create_perspective_projection_matrix(const float fov, const float near,
                                           const float far, const float aspect);

struct camera_projection_matrix
scene_create_orthographic_projection_matrix(const float left, const float right, const float top,
                                            const float bottom, const float near, const float far);

struct camera camera_create(struct camera_projection_matrix projection_matrix, const bool is_column_major);

#endif //GRAPHICS_TEST_CAMERA_H