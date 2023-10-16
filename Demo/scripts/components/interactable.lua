INTERACTABLE = 
{
	is_throwable = false,
	is_activated = false,

	interacting_agent = nil,

	interact = function(self)
		if not self.is_activated then
			print("An interaction has occured.");
			self.is_activated = true;
		end
	end,
};