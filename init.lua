local sg = game:GetService("StarterGui");

local screen = Instance.new("ScreenGui");
screen.Changed:connect(function(prop)
	print(prop, " = ", screen[prop]);
end);

game:GetService("RunService").Stepped:connect(function(time, step)
	print("Stepped: " .. time .. " " .. step);
end);

while true do
	wait(2);
	screen.Parent = sg;
	wait(2);
	screen.Parent = nil;
end
