local emotions = nil;
local affordance = nil;
local navigation = nil;
local rb = nil;

local wandering = true;
local travellingToObject = false;
local holdingRubbish = false;

local heldRubbish = {
    item = nil,
    type = ""
}

local currentTarget = nil;

local entityDistance = math.huge;
local yDifference = nil;

local currentFear = 0.0;

local interactDistance = 1000.0;
local minInteractDistance = 4.0;
local avoidanceDistance = 3.0;

local tickCounter = 0;
local tickWaitAmount = 5;
local tickCounterEngage = false;

function init()
    entity:add_model("assets/models/ai/hamid/hamid.fbx", 0.015);

    rb = entity:add_rigid();
    rb.mass = 10.0;
    rb.rot_lock = BVector3.new(true, false, true);
    rb.type = BodyType.Dynamic;
    rb.angularVelocity = Vector3.new(0.0, 0.0, 0.0);

    entity:get_transform().pos = Vector3.new(-25, 5, -15);

    entity:add_box_collider(Vector3.new(0.9, 1.75, 0.3));

    entity:add_script("emotions", "components/emotion.lua")
    emotions = entity:get_script("emotions");

    entity:add_script("affordance", "components/affordance.lua")
    affordance = entity:get_script("affordance");

    entity:add_script("navigation", "components/navigation.lua")
    navigation = entity:get_script("navigation");

    navigation.set_target(scene:get_camera():get_pos(), false);

    navigation.MAX_DISTANCE = 0.01;

    navigation.MOVE_SPEED = 0.035;
    navigation.ROTATE_SPEED = 2.5;
    navigation.BRAKE_FORCE = 1.5;

    navigation.set_state("wander");
end

function find_closest_bin(rubbishType)
    local closestBin = nil; -- The closest bin to the AI
    local shortestDistance = math.huge; -- Initially set to a very high value

    if (rubbishType == nil) then
        print("ERROR: No type given for the bin in 'find_closest_bin'!");
	end

    if (rubbishType == "rubbish") then
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
    end

    if (rubbishType == "recycling") then
        -- Loop through all the rubbish bins
        for i, bin in pairs(scene.globals.recyclingBins) do
            -- Get the distance between the AI and the current bin
            local distance = distance(entity:get_transform().pos, bin:get_transform().pos);

            -- Check to see if the distance is shorter than the current shortest distance
            if (distance < shortestDistance) then
                shortestDistance = distance;

                closestBin = bin;
            end
        end
    end
    
    return closestBin;  -- Return the closest bin
end

function rubbish_behaviour()
    -- Checks to see if the rubbish entity has been set
    if (next(scene.globals.apples) ~= nil) then
        local currentDistance = 0.0;

        if (travellingToObject == false) then
            closestDistance = math.huge;
            currentTarget = nil; -- Initialize to nil

            for key, value in pairs(scene.globals.apples) do
                -- Getting the distance between the AI and the entity
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

                entityDistance = distance(entity:get_transform().pos, value:get_transform().pos);

                -- Checking to see that the distance between the AI and the entity is within the interact distance
                if ((entityDistance < closestDistance) and yDifference < 10.0) then
                    closestDistance = entityDistance;

                    currentTarget = value;

                    wandering = false;
                    travellingToObject = true;
                end
            end

            if currentTarget ~= nil then
                navigation.set_state("chase");
                navigation.set_target(currentTarget:get_transform().pos, false);
            end
        end

        -- Checking to see if the AI is already travelling to the entity and is holding the rubbish
        if (travellingToObject == true and holdingRubbish == true) then
            for key, value in pairs(scene.globals.rubbishBins) do
                -- Getting the distance between the AI and the entity
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

                entityDistance = distance(entity:get_transform().pos, value:get_transform().pos);

                if ((entityDistance < minInteractDistance) and yDifference < 10.0) then
                    currentTarget = nil;

                    emotions.add_emotion_value(emotions.JOY, 0.2);

                    yDifference = nil;
                    currentFear = nil;

                    travellingToObject = false;
                    holdingRubbish = false;
                    wandering = true;

                    navigation.set_state("wander");

                    print("PRINT")

                    navigation.set_target(Vector3.new(0.0, 0.0, 0.0), false);
                end
            end
        end
    end
end

function on_contact(other_ent)
    if(heldRubbish.item == nil) then
        local affordanceComp = other_ent:get_script("affordance");
        local interactableComp = other_ent:get_script("interactable");

        if(interactableComp == nil or affordanceComp == nil) then
            return;
        end
        
        if(affordanceComp.AFFORDANCES.HOLDING == 1.0) then
            local type = nil;
            if (affordanceComp.AFFORDANCES.TRASHING == 1.0) then
                type = "trash";

                holdingRubbish = true;

                currentTarget = find_closest_bin("rubbish");

                navigation.set_target(currentTarget:get_transform().pos, false);  -- Set destination to closest bin

                print("Hamid has grabbed '" .. other_ent:get_tag() .. "'");
                print("Hamid is heading to '" .. currentTarget:get_tag() .. "'");

                tickCounterEngage = true;
            end

            if (affordanceComp.AFFORDANCES.RECYCLING == 1.0) then
                type = "recycling";
            end
            
            if (type ~= nil) then
                heldRubbish.item = other_ent;
                heldRubbish.type = type;

                interactableComp.interact(entity);
            end
        end
    end
end

function tick()
    if (tickCounterEngage == false) then
        -- Check if the AI is wandering around
        if (wandering == true) then
            navigation.set_state("wander");

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
        end

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
    else
        tickCounter = tickCounter + 1;

        rb.rot_lock = BVector3.new(true, true, true);
        rb.angularVelocity = Vector3.new(0.0, 0.0, 0.0);

		if (tickCounter >= tickWaitAmount) then
			tickCounter = 0;

			tickCounterEngage = false;

            rb.rot_lock = BVector3.new(true, false, true);
		end
    end
end