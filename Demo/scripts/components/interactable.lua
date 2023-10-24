is_throwable = false;
is_activated = false;
interact_count = 0;
interacting_agent = nil;

function interact(parent)
    if (not is_activated) then
        interact_count = interact_count + 1;
        is_activated = true;
        interacting_agent = parent;
		entity:get_rigid().hasGravity = false; -- TODO use locks instead of this hack
		entity:get_rigid().type = BodyType.Static; -- TODO use locks instead of this hack
        if (is_throwable) then
            if (entity:get_script("throwable") == nil) then
                entity:add_script("throwable", "components/throwable.lua");
            end

            entity:get_script("throwable").interact(parent)
        end

    else
        if (is_activated and interact_count > 0) then
            interact_count = interact_count - 1;
            is_activated = false;
            interacting_agent = nil;
			entity:get_rigid().hasGravity = true; -- TODO use locks instead of this hack
			entity:get_rigid().type = BodyType.Static; -- TODO use locks instead of this hack
        end
    end
end