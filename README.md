# TensorRT-YOLOv8 高性能部署工程
基于 NVIDIA TensorRT 实现的 YOLOv8 目标检测纯 C++/CUDA 部署框架，全流程 GPU 加速（预处理 + 推理 + 后处理），支持动态 Batch、批量图片与视频推理，专为工业级低延迟、高吞吐量场景设计。
# 核心特性
```
🎯 原生 YOLOv8 支持：目前测试使用YOLOv8n
⚡ 全流程 GPU 加速：预处理（Resize / 归一化 / 格式转换）、推理、后处理（解码 / NMS）全程在 GPU 内存完成，彻底消除 CPU-GPU 数据传输瓶颈
📦 动态 Batch 支持：构建时配置最大 Batch 范围，推理时自适应批量大小，大幅提升批量处理吞吐量
🎬 多输入源支持：支持单张 / 批量图片、本地视频文件推理，可扩展 RTSP 流
🔧 离线构建 + 在线推理分离：一次构建 TensorRT 引擎，多次重复加载推理，缩短启动时间
🚀 工业级性能：纯 C++/CUDA 实现，无 Python GIL 锁限制，延迟更低、稳定性更高
```
# 环境要求
```
依赖项	    推荐版本
CUDA	    12.4.1
cuDNN	    9.1.0
TensorRT	10.3.0
OpenCV	    4.10.0
CMake	    4.3.1
GCC	        11.4.0
```
# 测试使用硬件
NVIDIA RTX 4080super
# 快速开始
```
为避免网络问题，models/ 目录下已预置了 yolov8n.pt、yolov8n.onnx 及 yolov8n.engine 文件。如果你直接使用这些文件，可以跳过步骤 1 和 2，直接从步骤 3 开始。
注：硬件与软件版本与推荐一致才可直接使用，否则需要重新构建
```
## 步骤 1：模型导出（可选，已预执行）
```
如需重新导出 YOLOv8 ONNX 模型： 自动下载 yolov8n.pt 并导出为 ONNX 格式
cd scripts
python3 export_onnx.py
导出的 PyTorch 模型：models/pt/yolov8n.pt
导出的 ONNX 模型：models/onnx/yolov8n.onnx
```
## 步骤 2：构建 TensorRT 引擎（可选，已预执行）
```
将 ONNX 模型编译为 TensorRT 序列化引擎（.engine）：
cd builder
mkdir build && cd build
cmake ..
make -j$(nproc)
执行引擎构建，默认生成 FP16 精度引擎
./builder
生成的引擎文件：models/engine/yolov8n.engine
```
## 步骤 3：运行推理程序（必选）
```
编译并运行主推理程序：
cd pipeline
mkdir build && cd build
cmake ..
make -j$(nproc)
执行目标检测
./tensorRtYolo -e <engine_path> (-i <imgs_dir> | -v <video_path>) -o <output_dir>
engine_path 引擎路径
imgs_dir    图像目录
video_path  视频路径
output_dir  输出目录
```
# 目录结构
```
tensorRtYolo/
├── models/                # 模型文件目录
│   ├── pt/                # PyTorch 原始模型 (.pt)
│   ├── onnx/              # ONNX 中间模型 (.onnx)
│   └── engine/            # TensorRT 序列化引擎 (.engine)
├── scripts/               # 辅助工具脚本
│   └── export_onnx.py     # 自动下载 YOLOv8 模型并导出 ONNX
├── builder/               # 离线引擎构建模块
│   ├── CMakeLists.txt
│   ├── include/           # 头文件
│   └── src/               # 源文件
└── pipeline/              # 推理核心模块（项目核心）
    ├── common/            # 公用头文件
    ├── cuda/              # CUDA 预处理/后处理核函数
    ├── include/           # 头文件
    ├── output/            # 推理结果输出目录
    ├── test/              # 测试数据目录（需自行创建）
    │    ├── images/       # 测试图片
    │    └── videos/       # 测试视频
    ├── src/               # TensorRT 推理逻辑封装
    └── CMakeLists.txt