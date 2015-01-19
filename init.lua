--print(game:GetService("HttpService"):PostAsync("http://localhost/reqinfo.php", "Hi there.", "TextPlain"));

--print(Instance.new("Camera"));

print(game:GetService("HttpService"):GenerateGUID(true));

local cp = game:GetService("ContentProvider");
--print(cp);
--cp:Preload("http://t3.rbxcdn.com/85868eeed4d9bcb356b6388907018ad9");
cp:Preload("res://icon.png");

--local ns = game:GetService("NetworkServer");
--ns:Start();

-- Saved for more GUI testing
local img = Instance.new("ImageLabel");
img.BackgroundTransparency = 1;
img.Image = "res://icon.png";
img.ImageColor3 = Color3.new(0, 1, 0);
img.Size = UDim2.new(0, 32, 0, 32);
img.Position = UDim2.new(0.5, -50, 0.5, -100);
img.Parent = Instance.new("ScreenGui", game:GetService("StarterGui"));

game:GetService("RunService").RenderStepped:connect(function()
	img.Rotation = img.Rotation + 1;
end);

local extents = 500;

while true do
	for i = -extents, extents do
		img.Position = UDim2.new(0.5, i, 0.5, i/2);
		wait();
	end
	wait();
	for i = extents, -extents, -1 do
		img.Position = UDim2.new(0.5, i, 0.5, i/2);
		wait();
	end
end
