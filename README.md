# wxDemo 项目说明

## 项目简介

wxDemo 是一个基于 wxWidgets 库开发的 C++ 桌面工具集合应用，提供了多种常用的文本处理、加密解密等工具功能。

## 项目结构

```
wxDemo/
├── decrypt/           # 加密解密模块
│   ├── Base64Helper.cpp/.h    # Base64 编码解码工具
│   ├── Md5Helper.cpp/.h        # MD5 哈希计算工具
│   └── ShaHelper.cpp/.h        # SHA 哈希计算工具
├── text/              # 文本处理模块
│   ├── CssHelper.cpp/.h        # CSS 代码处理工具
│   ├── HtmlHelper.cpp/.h       # HTML 处理工具
│   ├── JavaScriptHelper.cpp/.h # JavaScript 处理工具
│   ├── JsonHelper.cpp/.h       # JSON 处理工具
│   └── TimeStampHelper.cpp/.h  # 时间戳处理工具
├── ui/                # UI 组件模块
│   └── TabPanel.cpp/.h         # 标签面板组件
├── HelperBase.cpp/.h  # 工具基类
├── MainFrame.cpp/.h   # 主窗口框架
├── MyTaskBarIcon.cpp/.h # 任务栏图标
├── wxDemo.cpp         # 应用入口
└── 其他项目文件        # Visual Studio 项目配置文件
```

## 功能模块

### 1. 加密解密模块 (decrypt/)

#### Base64Helper
- **Base64 编码**：将普通文本转换为 Base64 编码格式
- **Base64 解码**：将 Base64 编码文本解码为普通文本

#### Md5Helper
- **MD5 哈希计算**：生成文本的 MD5 哈希值

#### ShaHelper
- **SHA 哈希计算**：生成文本的 SHA 哈希值

### 2. 文本处理模块 (text/)

#### CssHelper
- **CSS 压缩**：压缩 CSS 代码，去除空白和注释
- **CSS 格式化**：美化 CSS 代码，添加适当的缩进和换行

#### HtmlHelper
- **HTML 实体编码**：将特殊字符转换为 HTML 实体
- **HTML 实体解码**：将 HTML 实体转换为特殊字符
- **HTML 标签过滤**：过滤 HTML 标签，仅保留文本内容

#### JavaScriptHelper
- **JavaScript 压缩**：压缩 JavaScript 代码
- **JavaScript 混淆**：混淆 JavaScript 代码，增加可读性难度

#### JsonHelper
- **JSON 格式化**：美化 JSON 数据，添加适当的缩进和换行
- **JSON 压缩**：压缩 JSON 数据，去除空白字符

#### TimeStampHelper
- **时间戳转日期时间**：将 Unix 时间戳转换为人类可读的日期时间格式
- **日期时间转时间戳**：将日期时间格式转换为 Unix 时间戳

### 3. UI 模块 (ui/)

#### TabPanel
- **标签面板**：每个标签页对应一个工具实例
- **布局**：左侧输入区（多行文本框），中间操作按钮列，右侧结果显示区（只读多行文本框）

## 技术架构

### 核心类结构

- **HelperBase**：所有工具类的基类，定义了工具的基本接口
- **MainFrame**：主窗口框架，管理工具选择和标签页创建
- **TabPanel**：标签面板，每个实例对应一个工具的操作界面
- **MyTaskBarIcon**：任务栏图标管理

### 工作流程

1. 启动应用程序，显示主窗口
2. 在左侧选择需要使用的工具
3. 系统在右侧创建一个新的标签页，显示该工具的操作界面
4. 在左侧输入区输入需要处理的内容
5. 点击中间的操作按钮执行相应的操作
6. 处理结果显示在右侧结果区

## 编译与运行

### 编译环境

- **开发工具**：Visual Studio
- **依赖库**：wxWidgets

### 编译步骤

1. 打开 `wxDemo.slnx` 解决方案文件
2. 选择合适的配置（Debug/Release）
3. 构建解决方案

### 运行

编译完成后，可执行文件位于 `x64/Debug/` 或 `x64/Release/` 目录下。

## 使用说明

### 工具使用示例

#### Base64 编码/解码
1. 选择 "Base64" 工具
2. 在输入区输入需要编码或解码的文本
3. 点击 "编码" 或 "解码" 按钮
4. 结果显示在右侧结果区

#### JSON 格式化
1. 选择 "JSON" 工具
2. 在输入区输入需要格式化的 JSON 数据
3. 点击 "格式化" 按钮
4. 格式化后的 JSON 数据显示在右侧结果区

#### 时间戳转换
1. 选择 "时间戳" 工具
2. 在输入区输入时间戳或日期时间
3. 点击 "时间戳转日期" 或 "日期转时间戳" 按钮
4. 转换结果显示在右侧结果区

## 项目特点

- **模块化设计**：各个工具功能独立封装，便于扩展和维护
- **友好的用户界面**：基于 wxWidgets 的图形界面，操作简单直观
- **多标签页支持**：可以同时打开多个工具标签页，方便多任务操作
- **实时处理**：输入内容后点击按钮即可实时查看处理结果

## 扩展指南

如需添加新的工具功能，只需：

1. 创建一个新的工具类，继承自 `HelperBase`
2. 实现 `ProcessAction` 方法，处理具体的工具操作
3. 在主窗口中添加对应的工具选择按钮
4. 注册工具到应用程序中

## 依赖项

- **wxWidgets**：跨平台 GUI 库

## 许可证

本项目采用 MIT 许可证。

## 作者

- 作者：[项目作者]
- 日期：2026-01-22
