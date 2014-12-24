local sg = game:GetService("StarterGui");

local screen = Instance.new("ScreenGui");
screen.Changed:connect(function(prop)
	print(prop, " = ", screen[prop]);
end);

game:GetService("RunService").RenderStepped:connect(function()
	--print("RenderStepped");
end);

while true do
	wait(2);
	screen.Parent = sg;
	wait(2);
	screen.Parent = nil;
end
