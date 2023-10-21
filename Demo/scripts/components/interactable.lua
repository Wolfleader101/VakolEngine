INTERACTABLE = 
{
	is_throwable = false,
	is_activated = false,

	interact_count = 0,

	interacting_agent = nil,

	interact = function(self)
		if not (self.is_activated) then
			self.interact_count = self.interact_count + 1;
			self.is_activated = true;
		else if (self.is_activated and self.interact_count > 0) then
				self.interact_count = self.interact_count - 1;
				self.is_activated = false;
			end
		end
	end,
};