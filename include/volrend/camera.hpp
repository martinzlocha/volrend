#pragma once

#include <array>
#include <memory>
#include "volrend/common.hpp"
#include "glm/mat4x3.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace volrend {
static const float CAMERA_DEFAULT_FOCAL_LENGTH = 600.f;

struct Camera {
    Camera(int width = 256, int height = 256,
           float focal = CAMERA_DEFAULT_FOCAL_LENGTH);
    ~Camera();

    /** Drag helpers **/
    void begin_drag(float x, float y, bool is_pan, bool about_origin);
    void drag_update(float x, float y);
    void end_drag();
    bool is_dragging() const;
    /** Move center by +=xyz, correctly handling drag **/
    void move(const glm::vec3& xyz);

    /** Camera params **/
    // Camera pose model, you can modify these
    glm::vec3 v_back, v_world_up, center;

    // Origin for about-origin rotation
    glm::vec3 origin;

    // Vectors below are automatically updated
    glm::vec3 v_up, v_right;

    // 4x3 affine transform used for actual rendering, automatically updated
    glm::mat4x3 transform;

    // Image size
    int width, height;

    // Focal length
    float focal;

    // GUI movement speed
    float movement_speed = 1.f;

    // CUDA memory used in kernel
    struct {
        float* transform = nullptr;
    } device;

    // Update the transform after modifying v_right/v_forward/center
    // (internal)
    void _update(bool copy_cuda = true);

   private:
    // For dragging
    struct DragState;
    std::unique_ptr<DragState> drag_state_;
};

}  // namespace volrend
