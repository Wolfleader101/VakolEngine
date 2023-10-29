local emotions = nil;
local affordance = nil;
local navigation = nil;
local rb = nil;
local transform = nil;
local target = nil;

local wandering = false;
local travellingToObject = false;
local holdingRubbish = false;
local initialRubbishContact = false;

local rubbishBins = scene.globals.rubbishBins;
local recyclingBins = scene.globals.recyclingBins;
local rubbish = {};
local currentRubbish = nil;

local yDifference = nil;

local entityDistance = nil;

local currentFear = 0.0;

local interactDistance = 5.0;
local minInteractDistance = 0.5;
local avoidanceDistance = 3.0;

function init()
    entity:add_model("assets/models/ai/hamid/hamid.fbx", 1.0);

    transform = entity:get_transform();

    rb = entity:add_rigid();
    rb.mass = 10.0;
    rb.rot_lock = BVector3.new(true, true, true);
    rb.type = BodyType.Dynamic;

    entity:get_transform().pos = Vector3.new(-30, 5, -20);
    entity:get_transform().scale = Vector3.new(0.015, 0.015, 0.015);

    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("emotions", "components/emotion.lua")
    emotions = entity:get_script("emotions");

    entity:add_script("affordance", "components/affordance.lua")
    affordance = entity:get_script("affordance");

    entity:add_script("navigation", "components/navigation.lua")
    navigation = entity:get_script("navigation");

    target = scene:get_camera():get_pos();

    navigation.TARGET = target;
    navigation.MAX_DISTANCE = 0.8;

    navigation.MOVE_SPEED = 0.025;
    navigation.ROTATE_SPEED = 2.5;
    navigation.BRAKE_FORCE = 1.0;
end

function find_closest_bin()
    local closestBin = nil; -- The closest bin to the AI
    local shortestDistance = math.huge; -- Initially set to a very high value

    -- Loop through all the rubbish bins
    for i, bin in pairs(scene.globals.rubbishBins) do
        -- Get the distance between the AI and the current bin
        local distance = distance(entity:get_transform().pos, bin:get_transform().pos);

        -- Check to see if the distance is shorter than the current shortest distance
        if (distance < shortestDistance) then
            shortestDistance = distance;

            closestBin = bin;
        end
    end
    
    return closestBin;  -- Return the closest bin
end

function rubbish_behaviour()
    rubbish = {};  -- Reset the rubbish table in case some are removed

    -- Populate the rubbish table with all the rubbish entities
    for i, v in ipairs(scene.globals.apples) do 
        rubbish[#rubbish + 1] = v;
    end

    -- Checks to see if the rubbish entity has been set
    if (next(rubbish) == nil) then
        -- Checking to see if the AI isn't already travelling to the entity
        if (travellingToObject == false) then
            for key, value in pairs(rubbish) do
                -- Getting the distance between the AI and the entity
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

                entityDistance = distance(entity:get_transform().pos, value:get_transform().pos);
                -- Checking to see that the distance between the AI and the entity is within the interact distance
                if ((entityDistance < interactDistance) and yDifference < 0.5) then
                    navigation.DESTINATION = value:get_transform().pos;

                    wandering = false;

                    travellingToObject = true;
                end
            end
        end

        -- Checking to see if the AI is already travelling to the entity and is not holding the rubbish
        if (travellingToObject == true and holdingRubbish == false) then
            yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

            entityDistance = distance(entity:get_transform().pos, value:get_transform().pos);
        
            -- Checking to see if the AI is close enough to the object to grab it (Done in the on_contact function)
            if ((entityDistance < minInteractDistance) and yDifference < 0.5) then
                holdingRubbish = true;
                initialRubbishContact = true;

                closestBin = find_closest_bin();

                if (closestBin) ~= nil then
                    navigation.DESTINATION = closestBin:get_transform().pos;  -- Set destination to closest bin
                end
            end
        end

        -- Checking to see if the AI is already travelling to the entity and is holding the rubbish
        if (travellingToObject == true and holdingRubbish == true) then
            for key, value in pairs(rubbishBins) do
                -- Getting the distance between the AI and the entity
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

                entityDistance = distance(entity:get_transform().pos, value:get_transform().pos);

                if ((entityDistance < minInteractDistance) and yDifference < 0.5) then
                    destroy_entity(currentRubbish);

                    emotions.add_emotion_value(emotions.JOY, 0.2);

                    yDifference = nil;
                    currentFear = nil;

                    travellingToObject = false;
                    holdingRubbish = false;
                    wandering =  true;
                end
            end
        end
    end
end

function on_contact(other_ent)
    -- Check to see if the AI has initially contacted a piece of rubbish
    if ((initialRubbishContact == true) and (holdingRubbish == true)) then
        if (emotions ~= nil) then
            local entityAffordance = other_ent:get_script("affordance");

            -- Check if the item affords holding
            if (entityAffordance ~= nil and entityAffordance.AFFORDANCES.HOLDING == 1.0) then
                currentRubbish = other_ent;
                holdingRubbish = true;  -- Now the AI is holding rubbish
                initialRubbishContact = false;  -- Reset initial contact flag

                -- If the item is interactable, interact with it
                if(other_ent:get_script("interactable") ~= nil) then
                    other_ent:get_script("interactable").interact(entity);
                end
            end
        end
    end
end

function tick()
    target = scene:get_camera():get_pos();

    -- Check if the AI is wandering around
    if (wandering ==  true) then
        navigation.wander();

        -- Check if the AI has emotions
        if (emotions ~= nil) then
            -- Check if the fear of the AI is above a certian amount before performing an action
            if (emotions.get_emotion(emotions.FEAR) ~= nil and emotions.get_emotion(emotions.FEAR) > 0.1) then
                -- AVOID AI LOGIC GOES HERE
                for i, aiEntity in pairs(scene.globals.emotional_entities) do
                    -- Make sure the AI is not itself
                    if (aiEntity ~= entity) then
                        local distanceToEntity = distance(entity:get_transform().pos, aiEntity:get_transform().pos);  -- Assuming you have a distance function

                        if (distanceToEntity < avoidanceDistance) then
                            -- Logic to avoid the AI entity goes here
                            -- For example, updating the navigation.DESTINATION to a location away from aiEntity
                        end
                    end
                end
            end
        end
    else
        -- Check if the AI has emotions
        if (emotions ~= nil) then
            -- Check if the fear of the AI is below a certian amount before performing an action
            if (emotions.get_emotion(emotions.FEAR) ~= nil and emotions.get_emotion(emotions.FEAR) < 0.1) then
                -- AI will be wandering before he hits the rubbish_behaviour, so grab the current fear value and set it
                if (wandering == true) then
                    currentFear = emotions.get_emotion(emotions.FEAR);
                end

                -- This will make sure that while he is doing his rubbish behaviour he will stay at his current fear level
                emotions.set_emotion(emotions.FEAR, currentFear);

                rubbish_behaviour();
            end
        end
    end
end