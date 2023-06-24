local CSharpLua = require"CSharpLua"

CSharpLua.Init()

local function script_path()
    local str = string.gsub(debug.getinfo(1).short_src, "^(.+\\)[^\\]+$", "%1")
    return str
end

function myprint(text)
    CSharpLua.SendMessage("ScriptResult",tostring(text))

end
local currentpath = script_path()
CSharpLua.LoadLibrary(currentpath .. "TestCSharpLua.dll","TestCSharpLua.Class1","Main","Hello World")

while true do
    local CloseEvent = CSharpLua.GetMessage("CloseEvent")
    local ScriptRun = CSharpLua.GetMessage("ScriptRun")
    if ScriptRun ~= nil then
        local fn,errmessage = load(ScriptRun,"Teste.lua","t")
        if fn then
            local success,result = pcall(fn)
            

        end
    end
    if CloseEvent ~= nil then
        break
    end
end
