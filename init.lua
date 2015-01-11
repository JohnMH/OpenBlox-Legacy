--print(game:GetService("HttpService"):PostAsync("http://localhost/reqinfo.php", "Hi there.", "TextPlain"));

print(Instance.new("Camera"));

local ns = game:GetService("NetworkServer");
--ns:Start();

--[[ Saved for more GUI testing
local frame = Instance.new("Frame");
frame.BackgroundColor3 = Color3.new(1, 0, 0);
frame.BorderColor3 = Color3.new(1, 1, 1);
frame.Position = UDim2.new(0.5, -50, 0.5, -100);
frame.Parent = Instance.new("ScreenGui", game:GetService("StarterGui"));

game:GetService("RunService").RenderStepped:connect(function()
	frame.Rotation = frame.Rotation + 1
end);

local extents = 500;

while true do
	for i = -extents, extents do
		frame.Position = UDim2.new(0.5, i, 0.5, i/2);
		wait();
	end
	wait();
	for i = extents, -extents, -1 do
		frame.Position = UDim2.new(0.5, i, 0.5, i/2);
		wait();
	end
end
]]
