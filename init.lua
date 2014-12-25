local sg = game:GetService("StarterGui");

local screen = Instance.new("ScreenGui");

local frame = Instance.new("Frame");
frame.BackgroundColor3 = Color3.new(1, 0, 0);
frame.Parent = screen;

screen.Parent = sg;

for i = 1, 10 do
	print(frame.AbsoluteSize, " ", frame.Size);
	wait(1);
end