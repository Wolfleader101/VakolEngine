function init(scene, entity)
    print("Hello from cameraController");
    camera = scene:get_camera();
    print("Camera Pos")
    print(camera:get_pos().x)
end

function update(scene, entity)
    local dir = { x = 0, y = 0, z = 0 };
    local velocity = 0;
    if (Input:get_key(KEYS["KEY_W"]))
    then
        dir.z = 1;
    end
    if (Input:get_key(KEYS["KEY_S"]))
    then
        dir.z = -1;
    end
    if (Input:get_key(KEYS["KEY_A"]))
    then
        dir.x = -1;
    end
    if (Input:get_key(KEYS["KEY_D"]))
    then
        dir.x = 1;
    end
    if (Input:get_key(KEYS["KEY_LEFT_SHIFT"]))
    then
        velocity = 50 * Time["delta_time"];
    else
        velocity = 30 * Time["delta_time"];
    end
    camera = scene:get_camera();
    old_pos = camera:get_pos();
    forward = camera:get_forward();
    right = camera:get_right();
    
    new_pos = {
        x = old_pos.x + (forward.x * dir.z + right.x * dir.x) * velocity,
        y = old_pos.y + (forward.y * dir.z + right.y * dir.x) * velocity,
        z = old_pos.z + (forward.z * dir.z + right.z * dir.x) * velocity}
    camera:set_pos(new_pos.x, new_pos.y, new_pos.z);
    print(camera:get_pos().z);

end

        -- float velocity = static_cast<float>((isSprinting ? 50.0f : 30.0f) * deltaTime);

        -- position += forward * forwardDir * velocity;
        -- position += right * rightDir * velocity;


    -- void Camera::OnKeyPressed(const int direction) {
    --     if (direction == GLFW_KEY_W) forwardDir = 1.0f;
    --     if (direction == GLFW_KEY_S) forwardDir = -1.0f;
    --     if (direction == GLFW_KEY_A) rightDir = -1.0f;
    --     if (direction == GLFW_KEY_D) rightDir = 1.0f;

    --     if (direction == GLFW_KEY_LEFT_SHIFT) isSprinting = true;
    -- }

    -- void Camera::OnKeyRelease(const int direction) {
    --     if (direction == GLFW_KEY_W) forwardDir = 0.0f;
    --     if (direction == GLFW_KEY_S) forwardDir = 0.0f;
    --     if (direction == GLFW_KEY_A) rightDir = 0.0f;
    --     if (direction == GLFW_KEY_D) rightDir = 0.0f;

    --     if (direction == GLFW_KEY_LEFT_SHIFT) isSprinting = false;
    -- }

    -- void Camera::OnMouseMove(float xoffset, float yoffset) {
    --     xoffset *= 0.05f;  // 0.01f should be replaced with MouseSensitivityX
    --     yoffset *= 0.05f;  // 0.01f should be replaced with MouseSensitivityY

    --     this->yaw += xoffset;
    --     this->pitch += yoffset;

    --     if (pitch > 89.0f) pitch = 89.0f;
    --     if (pitch < -89.0f) pitch = -89.0f;
    -- }