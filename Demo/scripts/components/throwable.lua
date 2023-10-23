THROWABLE = 
{
	can_throw = true,
	has_thrown = false,
	is_holding = false,

	hold = function(self, parent)
		if not self.is_holding then
			entity:get_transform().pos = parent;
		end
	end,

	throw = function(self, force)
		if self.can_throw then
			print("Throw Force: [x: " .. force.x .. " " .. ", y: " .. force.y .. ", z: " .. force.z .. "]");
			self.has_thrown = true;
		end
	end
};