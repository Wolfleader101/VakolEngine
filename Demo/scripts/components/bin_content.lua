COUNT = 0
TYPE = "TRASHING"

function on_contact(other_ent)
    local affordanceComp = other_ent:get_script("affordance")
    if(affordanceComp ~= nil) then
        if(affordanceComp.AFFORDANCES[TYPE]) then
            scene:destroy_entity(other_ent) -- TODO, in the future instead of deleting just disable and store its contents in bin??
            COUNT = COUNT + 1
        end
    end
end