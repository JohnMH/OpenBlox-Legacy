local http = game:GetService("HttpService");

local util = LoadLibrary("RbxUtility");

local ws = http:CreateWebSocket("ws://echo.websocket.org");
print(ws);
ws.OnClose:connect(function()
	print("WebSocket closed");
end);
ws.OnMessage:connect(function(data)
	data = util.DecodeJSON(data);
	print(data["bob"]);
	ws:close();
end);
local toEncode = {["bob"] = "testingThings"};
ws:send(util.EncodeJSON(toEncode));

--[[ Saved for more GUI testing
local frame = Instance.new("Frame");
frame.BackgroundColor3 = Color3.new(1, 0, 0);
frame.BorderColor3 = Color3.new(1, 1, 1);
frame.Position = UDim2.new(0.5, -50, 0.5, -100);
frame.Parent = Instance.new("ScreenGui", game:GetService("StarterGui"));

spawn(function()
	while true do
		for i = 0, 360 do
			frame.Rotation = i;
			wait();
		end
	end
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