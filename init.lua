--print(game:GetService("HttpService"):PostAsync("http://localhost/reqinfo.php", "Hi there.", "TextPlain"));

local cp = game:GetService("ContentProvider");

cp:Preload("res://icon.png");

--local ns = game:GetService("NetworkServer");
--ns:Start();

-- Saved for more GUI testing
local img = Instance.new("ImageLabel");
img.BackgroundTransparency = 1;
img.Image = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAFWSURBVBgZBcE/SFQBAAfg792dppJeEhjZn80MChpqdQ2iscmlscGi1nBPaGkviKKhONSpvSGHcCrBiDDjEhOC0I68sjvf+/V9RQCsLHRu7k0yvtN8MTMPICJieaLVS5IkafVeTkZEFLGy0JndO6vWNGVafPJVh2p8q/lqZl60DpIkaWcpa1nLYtpJkqR1EPVLz+pX4rj47FDbD2NKJ1U+6jTeTRdL/YuNrkLdhhuAZVP6ukqbh7V0TzmtadSEDZXKhhMG7ekZl24jGDLgtwEd6+jbdWAAEY0gKsPO+KPy01+jGgqlUjTK4ZroK/UVKoeOgJ5CpRyq5e2qjhF1laAS8c+Ymk1ZrVXXt2+9+fJBYUwDpZ4RR7Wtf9u9m2tF8Hwi9zJ3/tg5pW2FHVv7eZJHd75TBPD0QuYze7n4Zdv+ch7cfg8UAcDjq7mfwTycew1AEQAAAMB/0x+5JQ3zQMYAAAAASUVORK5CYII=";
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
