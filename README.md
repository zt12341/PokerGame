# 卡牌游戏项目

基于cocos2d-x 3.17开发的卡牌匹配游戏，采用MVC架构设计。

## 功能特性

### 已实现功能
1. **手牌区翻牌替换** - 点击手牌堆顶部卡牌，卡牌会移动到底牌位置并替换
2. **桌面牌和手牌区顶部牌匹配** - 点击桌面卡牌，如果点数与底牌相差1，则进行匹配
3. **回退功能** - 支持撤销之前的操作，卡牌会反向移动到原来的位置

### 游戏规则
- 卡牌匹配规则：点数相差1即可匹配（如3可以匹配2或4）
- A可以和K匹配（循环匹配）
- 无花色限制
- 胜利条件：清空所有游戏区域的卡牌

## 项目结构

```
Classes/
├── configs/        # 静态配置相关类
│   ├── models/     # 配置数据模型
│   └── loaders/    # 配置加载器
├── models/         # 运行时动态数据模型
├── views/          # 视图层，UI展示组件
├── controllers/    # 控制器层，协调模型和视图
├── managers/       # 管理器层，提供全局性服务
├── services/       # 服务层，处理业务逻辑
└── utils/          # 工具类，提供通用功能
```
(最初基于cocos的test1模板开始开发）
## 编译和运行

### Windows平台

#### 前提条件
- Visual Studio 2017或更新版本
- CMake 3.1或更新版本
- Python 2.7或3.x（用于下载依赖）

#### 编译步骤

1. **下载cocos2d-x依赖**
   ```bash
   cd cocos2d
   python download-deps.py
   ```

2. **使用CMake生成项目文件**
   ```bash
   # 在项目根目录创建build文件夹
   mkdir build
   cd build
   
   # 生成Visual Studio项目文件
   cmake .. -G "Visual Studio 16 2019"
   ```

3. **编译项目**
   ```bash
   # 使用CMake编译
   cmake --build . --config Debug
   
   # 或者打开生成的.sln文件，在Visual Studio中编译
   ```

#### 直接使用Visual Studio项目文件

也可以直接使用预配置的Visual Studio项目：
```bash
cd proj.win32
# 打开test1.sln文件，直接在Visual Studio中编译运行
```

### 其他平台

- **Android**: 进入`proj.android`目录，使用Android Studio打开项目
- **iOS/Mac**: 进入`proj.ios_mac`目录，使用Xcode打开项目
- **Linux**: 进入`proj.linux`目录，使用make编译

## 操作说明

### 游戏控制
- **鼠标左键点击卡牌** - 选择和移动卡牌
- **点击Undo按钮** - 撤销上一步操作
- **ESC键** - 退出游戏

### 游戏界面布局
- **游戏区域** (1080x1500) - 显示待消除的卡牌
- **手牌堆区域** (1080x580) - 显示可用的手牌
- **底牌区域** - 显示当前的匹配目标卡牌

## 架构设计特点

### MVC架构优势
1. **可维护性** - 清晰的模块划分和职责分离
2. **可扩展性** - 易于添加新功能和新类型
3. **可测试性** - 各模块相对独立，便于单元测试

### 关键设计原则
- **单一职责原则** - 每个类只负责一个特定功能
- **依赖注入** - 通过构造函数和初始化方法传递依赖
- **回调机制** - 视图通过回调函数与控制器通信
- **智能指针管理** - 合理的内存管理和生命周期控制

## 扩展指南

### 添加新卡牌类型
详见[程序设计文档.md](程序设计文档.md)的第4章节

### 添加新撤销功能
详见[程序设计文档.md](程序设计文档.md)的第5章节

## 资源文件说明

### 卡牌数字资源
- 位置: `Resources/res/number/`
- 命名规则: `{size}_{color}_{face}.png`
  - size: big/small
  - color: red/black  
  - face: A,2,3...K

### 花色资源
- 位置: `Resources/res/suits/`
- 文件: club.png, diamond.png, heart.png, spade.png

### 卡牌背景
- 位置: `Resources/res/card_general.png`

## 版本信息

- **cocos2d-x版本**: 3.17
- **目标分辨率**: 1080x2080
- **开发语言**: C++11
- **支持平台**: Windows, Mac, iOS, Android, Linux

## 许可证

本项目采用MIT许可证，详见LICENSE文件。

## 联系方式

如有问题或建议，请通过GitHub Issues反馈。

