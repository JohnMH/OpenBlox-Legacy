local cp = game:GetService("ContentProvider");
cp:Preload("http://incompetech.com/music/royalty-free/mp3-royaltyfree/Pixel%20Peeker%20Polka%20-%20faster.mp3")
local p = Instance.new("ScreenGui", game:GetService("StarterGui"))
local rand = math.random

local snd = Instance.new("Sound");
snd.SoundId = "http://incompetech.com/music/royalty-free/mp3-royaltyfree/Pixel%20Peeker%20Polka%20-%20faster.mp3";
snd:Play(); -- because yes

local guis = { }

while true do -- TODO: fix events
	local f = Instance.new("Frame")
	f.BackgroundColor3 = Color3.new(rand(),rand(),rand())
	f.BorderColor3 = Color3.new(rand(),rand(),rand())
	f.Position = UDim2.new(rand(), 0, rand(), 0)
	f.Size = UDim2.new(0, rand()*100, 0, rand()*100)
	f.Parent = p
	guis[#guis+1] = f
	for i = 1, #guis do
		guis[i].Rotation = guis[i].Rotation + 1
	end
	game:SetTitle(string.format("%d frames", #guis))
	wait(1/30)
end