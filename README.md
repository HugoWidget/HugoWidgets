# HugoWidgets

> [!NOTE]
>
> 建议阅读 [HugoWidget 简介](https://github.com/HugoWidget/HugoWidget) 以了解开发情况

若想进一步了解本项目，可阅读 [HugoWidgets 用户文档](docs/User.md) 与 [HugoWidgets 开发者文档](docs/Developer.md)

## 项目介绍

帮助广大电教委对希沃功能进行增强与补充

这是[HugoProgs](https://github.com/HugoWidget/HugoProgs)的含UI版本

## 环境依赖

Qt6

## 功能

与HugoProgs类似，但提供更友好的操作方式，并计划内置开机自启，修改配置等功能。

## 项目组成

- HugoMain：该项目主界面，依赖于HugoWidgets实行功能
- HugoWidgetsPlugin：独立于HugoMain，提供各种功能
- 集成了`WEcore`项目框架，你可以开发自己的插件，为项目添加功能

## 项目依赖

[HugoUtils](https://github.com/HugoWidget/HugoUtils)(Hugo系列核心库)

[Qlementine](https://github.com/oclero/qlementine)(Qt 风格组件库)

[WidgetExplorer](https://github.com/howdy213/WidgetExplorer)(Qt 插件框架)

[WECore](https://github.com/howdy213/WECore)(Qt 插件框架核心库)

## 许可证

本项目采用 GPLv3 许可证，详情参见 [LICENSE](LICENSE) 文件。

WidgetExplorer: [Apache License 2.0](licenses/LICENSE-WidgetExplorer)

Qlementine: [MIT 许可证](licenses/LICENSE-Qlementine)

Qt: [LGPLv3 许可证](licenses/LICENSE.LESSER-Qt)

WECore: [Apache License 2.0](licenses/LICENSE-WECore)

HugoUtils: [LGPLv3 许可证](licenses/LICENSE.LESSER-HugoUtils)

WinUtils:  [MIT 许可证](licenses/LICENSE-WinUtils)

hash-library: [zlib 许可证](licenses/LICENSE-hash-library)

swhelper：[MIT 许可证](licenses/LICENSE-swhelper)

cpp-httplib: [MIT 许可证](licenses/LICENSE-cpp-httplib)

mINI: [MIT 许可证](licenses/LICENSE-mINI)

WinReg: [MIT 许可证](licenses/LICENSE-WinReg)

libsharedmemory：[MIT 许可证](licenses/LICENSE-libsharedmemory)

## 免责声明

本项目仅用于研究或教育目的，请勿将本项目用于可能违反当地法律、侵犯著作权或其他软件 EULA 的用途。若将本项目用于非法用途，一切后果由使用者承担，开发者不承担此类行为带来的任何后果或责任。
