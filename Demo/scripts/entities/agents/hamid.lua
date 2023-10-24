local emotions = nil;
local navigation = nil;
local rb = nil;
local transform = nil;
local rubbish = nil;

local travellingToObject = false;
local holdingRubbish = false;

local rubbish = nil;
local rubbishBins = nil;
local recyclingBins = nil;

local xDifference = nil;
local yDifference = nil;
local zDifference = nil;

local interactDistance = 5.0;

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

function set_rubbish_entity(inputEntity)
    -- rubbish = inputEntity;
end

function move_to_position(inputTarget)
    -- navigation.DESTINATION = inputTarget;
end

function tick()
    --[[
    -- WANDER AROUND

    -- Checks to see if the rubbish entity has been set
    if (rubbish ~= nil) then
        -- Checking to see if the AI is already travelling to the entity
        if (travellingToObject == false) then
            for key, value in pairs(rubbish) do
                -- Getting the distance between the AI and the entity
                xDifference = math.abs(entity:get_transform().pos.x - value:get_transform().pos.x);
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);
                zDifference = math.abs(entity:get_transform().pos.z - value:get_transform().pos.z);

                -- Checking to see that the distance between the AI and the entity is within the interact distance
                if ((xDifference < interactDistance or zDifference < interactDistance) and yDifference < interactDistance) then
                    navigation.DESTINATION = value:get_transform().pos;
                end
            end
        end

        if (travellingToObject == true and holdingRubbish == false) then
            xDifference = math.abs(entity:get_transform().pos.x - value:get_transform().pos.x);
            yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);
            zDifference = math.abs(entity:get_transform().pos.z - value:get_transform().pos.z);
        
            -- Checking to see if the AI is close enough to the object to grab it
            if ((xDifference < 0.5 or zDifference < 0.5) and yDifference < 0.5) then
                -- GRAB OBJECT 

                holdingRubbish = true;

                -- SET DESTINATION TO CLOSEST RUBBISH BIN
            end
        end

        if (travellingToObject == true and holdingRubbish == true) then
            for key, value in pairs(rubbishBins) do
                -- Getting the distance between the AI and the entity
                xDifference = math.abs(entity:get_transform().pos.x - value:get_transform().pos.x);
                yDifference = math.abs(entity:get_transform().pos.y - value:get_transform().pos.y);
                zDifference = math.abs(entity:get_transform().pos.z - value:get_transform().pos.z);

                if ((xDifference < 0.5 or zDifference < 0.5) and yDifference < 0.5) then
                    -- DEPOSIT rubbish

                    xDifference = nil;
                    yDifference = nil;
                    zDifference = nil;

                    travellingToObject = false;
                    holdingRubbish = false;
                end
            end
        end
    end
    --]]
end