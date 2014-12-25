local sg = game:GetService("StarterGui");

local screen = Instance.new("ScreenGui");

local frame = Instance.new("Frame");
frame.BackgroundColor3 = Color3.new(1, 0, 0);
frame.BorderColor3 = Color3.new(1, 1, 1);
frame.Position = UDim2.new(0.5, -50, 0.5, -100);
frame.Parent = screen;

screen.Parent = sg;

while true do
	for i = 0, 360 do
		frame.Rotation = i;
		wait();
	end
end

--[[
local downPos = UDim2.new(0, 25, 0, 25);

local frame2 = Instance.new("Frame");
frame2.BackgroundColor3 = Color3.new(0, 1, 0);
frame2.BorderColor3 = Color3.new(1, 1, 1);
frame2.Position = downPos;
frame2.Parent = frame;

local frame3 = Instance.new("Frame");
frame3.BackgroundColor3 = Color3.new(0, 0, 1);
frame3.BorderColor3 = Color3.new(1, 1, 1);
frame3.BorderSizePixel = 10;
frame3.Position = downPos;
frame3.Parent = frame2;

while true do
	local tweenBy = 200;
	for i = 1, tweenBy do
		--frame.Rotation = i;
		frame.Position = UDim2.new(0.5, 100 - i, 0.5, 0 - i);
		wait();
	end
	--frame.Rotation = 0;
	for i = tweenBy, 1, -1 do
		frame.Position = UDim2.new(0.5, 100 - i, 0.5, 0 - i);
		wait();
	end
end
]]