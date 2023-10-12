function init()
entity:add_script("affordance", "components/affordance.lua")

local affordComp = entity:get_script("affordance");

affordComp.AFFORDANCES = {
        HOLDING = 1.0,
        THROWING = 1.0
}
end