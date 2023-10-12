function init()
entity:add_script("affordance", "components/affordance.lua")

local affordComp = entity:get_script("affordance");

affordComp.AFFORDANCES = {
        SITTABLE = 1.0
}
end