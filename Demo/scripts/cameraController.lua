function init(scene, entity)
    print("Hello from cameraController");
    camera = scene:get_camera();
    print("Camera Pos")
    print(camera:get_pos().x)
end


function update(scene, entity)
    print("Camera Pos")
    camera = scene:get_camera();
    print(camera:get_pos().x)
end