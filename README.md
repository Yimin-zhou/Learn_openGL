# Learn_openGL
## 简介
这是一个基于openGL的简单渲染器，目的是学习和总结openGL以及一些渲染知识。

## 进度
### 核心功能
- [x] 初始化 OpenGL, 窗口
- [x] 实现基本渲染循环
- [x] VAO, VBO， EBO
- [x] shaderBuilder和使用shader
- [ ] 建立摄像机系统（透视、正交）
- [ ] 实现纹理加载和绑定
- [ ] 实现基础光照模型
- [ ] 实现基础材质模型（漫反射、镜面反射、环境光）PBR
- [ ] 
### 模型加载
- [	] obj模型加载
- [ ] fbx模型加载
- [ ] 实现骨骼动画

### 高级渲染技术
- [ ] 实现多重采样抗锯齿MSAA
- [ ] 天空盒
- [ ] 为定向光、点光源和聚光灯实现阴影映射
- [ ] 添加法线映射以增强表面细节
- [ ] 实现屏幕空间环境光遮蔽SSAO
- [ ] 设置延迟渲染管线
- [ ] 实现泛光效果
- [ ] 添加HDR渲染与色调映射
- [ ] 实现视差遮挡映射以增强深度感知

### 优化技术
- [ ] 使用统一缓冲对象 UBOs 在着色器间高效共享数据
- [ ] 实现视锥体剔除以减少绘制调用
- [ ] 设置实例化渲染以高效渲染多个对象
- [ ] 实现遮挡剔除以跳过隐藏对象的渲染

### 后期处理效果
- [ ] 实现后期处理框架
- [ ] 添加高斯模糊、色彩分级和晕影等效果
- [ ] 实现抗锯齿技术（MSAA、FXAA、SMAA）

### 额外功能
- [ ] 建立 GUI 系统以实时调整参数（使用 Dear ImGui 或类似工具）
- [ ] 实现粒子系统，用于火焰、烟雾、雨等效果
- [ ] 添加支持骨骼动画
- [ ] 实现地形生成和渲染

### 调试和性能分析
- [x] 实现 OpenGL 调试和错误检查
- [ ] 设置屏幕显示以获取渲染统计信息和诊断信息
- [ ] 使用性能分析工具识别并优化性能瓶颈
