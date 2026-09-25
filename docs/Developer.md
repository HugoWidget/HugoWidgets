# HugoWidgets 开发者文档

> 面向开发者：插件框架、宿主程序启动流程、功能页开发流程与构建调试要点。
>
> 使用者请阅读 [HugoWidgets 用户文档](./User.md)。

## 目录

- [1. 技术栈与总体架构](#1-技术栈与总体架构)
- [2. 仓库结构](#2-仓库结构)
- [3. 构建](#3-构建)
- [4. 主界面插件 HugoMain](#4-主界面插件-hugomain)
- [5. 功能页插件 HugoWidgetsPlugin](#5-功能页插件-hugowidgetsplugin)
- [6. 新增功能页与插件](#6-新增功能页与插件)

## 1. 技术栈与总体架构

| 项          | 内容                                                         |
| ----------- | ------------------------------------------------------------ |
| 语言 / 标准 | C++，`CONFIG += c++2b`                                       |
| UI 框架     | Qt6                                                          |
| 构建系统    | qmake                                                        |
| 插件框架    | [WidgetExplorer](https://github.com/howdy213/WidgetExplorer) |
| 核心能力库  | [HugoUtils](https://github.com/HugoWidget/HugoUtils)（含 `WinUtils`） |

当前采用 WidgetExplorer 的旧版框架，便捷性有限，但暂时不会升级到新版本。 

## 2. 仓库结构

```text
HugoWidgets/
├── WidgetExplorer.pro          # 顶层 qmake 工程（SUBDIRS）
├── README.md                   
├── docs/                       # 文档
├── icons/                      # 图标
├── licenses/                   # 第三方许可证副本
├── deps/                       # 依赖库
├── src/
│   ├── WidgetExplorer.pro      # 宿主程序工程
└── plugins/
    ├── HugoMain/               # 主界面插件
    │   ├── HugoMain.pro
    │   ├── hugomainplugin.h/.cpp     # WPlugin 实现
    │   ├── hugomainwidget.h/.cpp     # 主窗口（菜单栏 + 标签页）
    │   ├── IHugoMain.h               # 事件/键名常量
    │   └── functionpagebase.h        # 功能页基类（插件间共享接口）
    └── HugoWidgetsPlugin/      # 功能页插件
        ├── HugoWidgetsPlugin.pro
        ├── HugoWidgetsPlugin.h/.cpp
        └── HugoPages/          # 功能页
```

## 3. 构建

### 3.1 环境要求

- Qt6
- Git（拉取子模块）

### 3.2 拉取与构建

```bash
git clone https://github.com/HugoWidget/HugoWidgets --recursive
```

用 Qt Creator 打开顶层 WidgetExplorer.pro

## 4. 主界面插件 HugoMain

`HugoMain` 负责「壳」：菜单栏、标签页与功能页的装载。

### 4.1 插件初始化

HugoMainPlugin::init 做了三件事：

1. `WinUtils::Console console; console.attach();` —— 附加控制台，便于查看底层库输出（调试友好）。

2. 创建并注册主窗口：

   ```cpp
   widget = new HugoMainWidget(PData);
   widgetManager->addWidget(QUuid::createUuid(), widget, this);
   ```

3. 解析命令行参数并立即执行命令。

### 4.2 主窗口结构

HugoMainWidget 是 `QWidget`，内部持有一个 `QMainWindow` 与一个 `QTabWidget`。

### 4.3 事件常量

IHugoMain.h 定义了事件与键名常量：

| 类别    | 常量                              |
| ------- | --------------------------------- |
| `Event` | `MenuAction`、`ToolAction`、`Tab` |
| `Key`   | `MenuPath`、`TabTitle`            |

### 4.4 功能页基类 `FunctionPageBase`

functionpagebase.h 是**功能页插件与主界面之间的契约**，位于 `HugoMain` 目录但被两个插件共同引用（在 `HugoWidgetsPlugin.pro` 的 `HEADERS` 中以 `../HugoMain/FunctionPageBase.h` 形式加入）。

开发新功能页时**必须实现 `id()`、`name()`、`handleCommand()`**。

## 5. 功能页插件 HugoWidgetsPlugin

### 5.1 页面注册

HugoWidgetsPlugin::init 在设置元数据后，把 5 个功能页交给主界面：

```cpp
auto createPage = [&](FunctionPageBase* base) {
    WMessage msg2;
    msg2.object = base;
    WEvent extEvent(QString("hugo.hugowidget.hugowidgets.addpage"), msg2);
    widgetManager->publish(extEvent);
};
createPage(new HugoXxxPage);
```

要点：

- 通过 `hugo.hugowidget.hugowidgets.addpage` 事件发布功能页，**由主界面插件接收并登记**（加入 `m_allPagesById`、在主页与菜单栏呈现入口）。这就是功能页插件与主界面解耦的关键。
- 功能页对象以裸指针创建，生命周期由接收方（主界面）管理，页面关闭时调用 `cleanup()`。

## 6. 新增功能页与插件

### 6.1 在现有插件中新增一个功能页

1. 在 `plugins/HugoWidgetsPlugin/HugoPages/` 下新建 `HugoXxxPage.h` / `.cpp`（需要界面时可加 `.ui`）。

2. 继承 `FunctionPageBase`，实现三个纯虚函数：

   ```cpp
   class HugoXxxPage : public FunctionPageBase {
       Q_OBJECT
   public:
       explicit HugoXxxPage(QWidget *parent = nullptr);
       ~HugoXxxPage() override;
       QString id() const override;                       // 唯一 id
       QString name() const override;                     // 显示名称
       bool handleCommand(const QString &command,
                          const QVariantMap &args = QVariantMap()) override;
       void init() override;      // 可选：打开页面时初始化
       void cleanup() override;   // 可选：关闭页面时清理
   private slots:
       void on_m_btnDo_clicked();
   };
   ```

3. 把新文件加入 HugoWidgetsPlugin.pro。

4. 在 `HugoWidgetsPlugin::init()` 中追加 `createPage(new HugoXxxPage);`。

5. 重新构建插件——功能入口会自动出现在主页与菜单栏（由主界面根据 `id()`/`name()` 生成）。

### 6.2 新增一个独立插件

1. 在 `plugins/` 下新建目录，参照 HugoMain.pro 编写 `.pro`（`TEMPLATE = lib`、`CONFIG += plugin`、`DESTDIR` 指向 `src/<cfg>/plugins/HugoWidget`、`include` WECore / HugoUtils 的 `.pri`）。
2. 实现 `WPlugin` 的 `init` / `recMsg` / `deinit`，在 `init` 中用 `PluginData::setData/setPlugin` 取回框架对象并 `setMetaData` 设置插件名与作者。
3. 把新插件加入顶层 WidgetExplorer.pro 的 `SUBDIRS`（注意 `CONFIG += ordered`，插件应排在 `src/WidgetExplorer.pro` 之后）。
4. **在插件清单中登记**：在元数据里为其分配数字键，并填写 `Plugin::Path` 等字段，否则不会被加载。
5. 构建后在程序目录验证：插件能被加载且在插件列表/主界面选择对话框中可见。

## 相关文档

- [HugoWidgets 用户文档](./User.md)
- [WECore 插件框架](https://github.com/howdy213/WECore)
- [WidgetExplorer](https://github.com/howdy213/WidgetExplorer)
- [HugoUtils 核心库](https://github.com/HugoWidget/HugoUtils)