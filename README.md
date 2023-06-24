# CSharpLua API Documentation

## Overview

The CSharpLua library provides a bridge between Lua and C#, allowing communication and interaction between the two languages by messages. It enables you to execute Lua scripts within a C# application and access C# functionality from Lua scripts.


## Installation

To use the CSharpLua library in your C# project, follow these steps:

1. Download the latest release of the CSharpLua library from the official repository.
2. Use pinvoke from CSharpLua

```csharp
  [DllImport("CSharpLua.dll", CharSet = CharSet.Ansi)]
  public static extern void CSharpLua_SendMessage(string name, string message);

[DllImport("CSharpLua.dll")]
public static extern bool CSharpLua_GetMessage(string name, [MarshalAs(UnmanagedType.LPStr)] out string message);
private static bool Inited;    
```

## Usage

To use the CSharpLua library in your application, follow these steps:

1. Inicialize lua script

```lua
local CSharpLua = require"CSharpLua"

CSharpLua.Init()
```

2. Load a dll:

```lua
CSharpLua.LoadLibrary(currentpath .. "TestCSharpLua.dll","TestCSharpLua.Class1","Main","Hello World")
```

3. Access C# functionality from Lua scripts using the provided API.
```lua
CSharpLua.SendMessage("ping","1")
```
```csharp
ClassTest.GetMessage("ping",out string test);
Console.WriteLine(test);
```
## Lua API


For detailed information on the Lua API, refer to the [Lua API Def](src/CSharpLua.def.lua).


## Examples

Here are some examples demonstrating the usage of the CSharpLua library:

[Example](example)

For more examples, refer to the [Examples](example) directory.

## Contributing

Contributions to the CSharpLua library are welcome! If you find a bug, have a feature request, or want to contribute code.

## License

The CSharpLua library is licensed under the [MIT License](LICENSE).

---

