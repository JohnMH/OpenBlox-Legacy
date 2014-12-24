local sg = game:GetService("StarterGui");

local screen = Instance.new("ScreenGui");
screen.Changed:connect(function(prop)
	spawn(function()
		print(prop, " = ", screen[prop]);
	end);
end);

while true do
	wait(2);
	screen.Parent = sg;
	wait(2);
	screen.Parent = nil;
end