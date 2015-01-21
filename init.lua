--print(game:GetService("HttpService"):PostAsync("http://localhost/reqinfo.php", "Hi there.", "TextPlain"));

local snd = Instance.new("Sound");
snd.SoundId = "http://incompetech.com/music/royalty-free/mp3-royaltyfree/Pixel%20Peeker%20Polka%20-%20faster.mp3";
snd:Play();

local cp = game:GetService("ContentProvider");
--local ns = game:GetService("NetworkServer");
--ns:Start();

-- Saved for more GUI testing
local img = Instance.new("ImageLabel");
img.BackgroundTransparency = 1;
img.Image = "http://thelasportsnetwork.homestead.com/MISC/moon.png";
img.Size = UDim2.new(0, 500, 0, 494);
img.Position = UDim2.new(0.5, -50, 0.5, -100);
img.Parent = Instance.new("ScreenGui", game:GetService("StarterGui"));

game:GetService("RunService").RenderStepped:connect(function()
	img.Rotation = img.Rotation + 3;
	img.ImageColor3 = Color3.new(math.random(), math.random(), math.random());
end);

local extents = 250;

while true do
	for i = -extents, extents do
		img.Position = UDim2.new(0.5, i, 0.5, i/4);
		wait();
	end
	wait();
	for i = extents, -extents, -1 do
		img.Position = UDim2.new(0.5, i, 0.5, i/4);
		wait();
	end
end
