# 51MCU_Project

## 烧录

构建 `flash` 目标时会根据 CH340 的 USB VID/PID 自动识别串口，不需要在
`CMakeLists.txt` 中维护 COM 端口号：

```powershell
cmake --build cmake-build-debug --target flash
```

连接了多个 CH340，或使用其他 USB-UART 时，可以临时指定端口：

```powershell
$env:STC_PORT = "COM9"
cmake --build cmake-build-debug --target flash
```

如果 `stcgal` 不在项目虚拟环境或 `PATH` 中，可以指定其路径：

```powershell
$env:STCGAL_EXE = "D:\path\to\stcgal.exe"
```
