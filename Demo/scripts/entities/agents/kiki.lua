function init()
entity:add_script("affordance", "components/affordance.lua")

local affordComp = entity:get_script("affordance");

affordComp.AFFORDANCES = {
    HITTABLE = 1.0,
    KICKABLE = 1.0
}
end

-- function perceiveObjectAffordance(object)
--     local objectAffordance = object:get_script("affordance").AFFORDANCES
--     PERCEIVED_AFFORDANCES.SITTABLE = someFunctionToDeterminePerception(objectAffordance.SITTABLE)
-- end

function tick()
end