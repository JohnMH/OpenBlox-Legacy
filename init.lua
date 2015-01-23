local getService = game.GetService;

local cp = getService(game, "ContentProvider");

local music_url = "http://incompetech.com/music/royalty-free/mp3-royaltyfree/Pixel%20Peeker%20Polka%20-%20faster.mp3";

cp:Preload(music_url)

local new = Instance.new;
local newColor3 = Color3.new;
local newUDim2 = UDim2.new;

local p = new("ScreenGui", getService(game, "StarterGui"))
local rand = math.random;

local snd = Instance.new("Sound");
--snd.Looped = true;
snd.SoundId = music_url;
snd:Play(); -- because yes

local guis = {};

while true do -- TODO: fix events
	local snd = Instance.new("Sound");
	snd.SoundId = music_url;
	snd:Play();

	local f = new("Frame");
	f.BackgroundColor3 = newColor3(rand(),rand(),rand());
	f.BorderColor3 = newColor3(rand(),rand(),rand());
	f.Position = newUDim2(rand(), 0, rand(), 0);
	f.Size = newUDim2(0, rand()*100, 0, rand()*100);
	f.Parent = p;

	guis[#guis+1] = f;
	for i = 1, #guis do
		guis[i].Rotation = guis[i].Rotation + 1
	end

	game:SetTitle(string.format("%d frames - %d FPS", #guis, game:GetFPS()));
	wait(1/30);
end