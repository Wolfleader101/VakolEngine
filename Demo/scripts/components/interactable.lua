INTERACTABLE = 
{
	is_throwable = false,
	is_activated = false,

	interact_count = 0,

	interacting_agent = nil,

	interact = function(self, parent)
		if (not self.is_activated) then
			self.interact_count = self.interact_count + 1;
			self.is_activated = true;

			self.interacting_agent = parent;
			
			print(self.is_throwable);
		else if (self.is_activated and self.interact_count > 0) then
				self.interact_count = self.interact_count - 1;
				self.is_activated = false;

				self.interacting_agent = nil;
			end
		end
	end,
};