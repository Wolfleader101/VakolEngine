local emotions = nil;
local navigation = nil;
local rb = nil;
local transform = nil;

local wandering = false;
local travellingToObject = false;
local holdingRubbish = false;

local rubbish = {};
local rubbishBins = {};
local recyclingBins = {};

local yDifference = nil;

local distance = nil;

local interactDistance = 5.0;
local minInteractDistance = 0.5;

function init()
    rb = entity:add_rigid();

    rb.mass = 10.0;
    rb.rot_lock = BVector3.new(true, true, true);

    transform = entity:get_transform();

    -- entity:add_script("navigation", "components/navigation.lua")
    -- navigation = entity:get_script("navigation");

    entity:add_script("emotions", "fcms/emoiton.lua")
    emotions = entity:get_script("emotions");
end

function add_rubbish_entity(inputEntity)
    -- table.insert(rubbish, inputEntity);
end

function add_rubbish_bin(inputEntity)
    -- table.insert(rubbishBins, inputEntity);
end

function add_recycling_bin(inputEntity)
    -- table.insert(recyclingBins, inputEntity);
end

function move_to_position(inputTarget)
    -- navigation.DESTINATION = inputTarget;
end

function rubbish_behaviour()
	--[[
    -- Checks to see if the rubbish entity has been set
    if (next(rubbish) == nil) then
        -- Checking to see if the AI is already travelling to the entity
        if (travellingToObject == false) then
            for key, value in pairs(rubbish) do
                -- Getting the distance between the AI and the entity
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

                distance = Distance(entity:get_transform().pos, value:get_transform().pos);

                -- Checking to see that the distance between the AI and the entity is within the interact distance
                if ((distance < interactDistance) and yDifference < 0.5) then
                    navigation.DESTINATION = value:get_transform().pos;

                    wandering = false;

                    travellingToObject = true;
                end
            end
        end

        if (travellingToObject == true and holdingRubbish == false) then
            yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

            distance = Distance(entity:get_transform().pos, value:get_transform().pos);
        
            -- Checking to see if the AI is close enough to the object to grab it
            if ((distance < minInteractDistance) and yDifference < 0.5) then
                -- GRAB OBJECT 

                holdingRubbish = true;

                -- move_to_position(CLOSEST RUBBISH BIN);
            end
        end

        if (travellingToObject == true and holdingRubbish == true) then
            for key, value in pairs(rubbishBins) do
                -- Getting the distance between the AI and the entity
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);

                distance = Distance(entity:get_transform().pos, value:get_transform().pos);

                if ((distance < minInteractDistance) and yDifference < 0.5) then
                    -- DEPOSIT rubbish

                    yDifference = nil;

                    travellingToObject = false;
                    holdingRubbish = false;
                    wandering =  true;
                end
            end
        end
    end
    --]]
end

function tick()
    --[[
    -- Check if the AI is wandering around
    if (wandering ==  true) then
        -- WANDER AROUND
    else
        -- Check if the AI has emotions
        if (emotions ~= nil) then
            -- Check if the fear of the AI is below a certian amount before performing an action
            if (emotions.get_emotion(emotions.FEAR)~= nil and emotions.get_emotion(emotions.FEAR) < 0.1) then
                rubbish_behaviour();
            end
        end
    end
    --]]
end