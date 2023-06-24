---@meta CSharpLua

---@class CSharpLua
---@field Init fun():nil
---@field LoadLibrary fun(assemblypath: string,typename: string,methodname: string,argument: string):integer,integer
---@field GetMessage fun(name:string):string
---@field SendMessage fun(name:string,message: string):string
---@field Destroy fun():nil


---@type CSharpLua
local ret = {}
return ret