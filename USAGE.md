# Qt学生成绩管理系统 - 使用说明

## 当前状态
✅ **程序正常运行** - 原始版本已验证可正常工作

## 快速启动
```bash
# 直接运行已编译的程序
build\Desktop_Qt_6_9_2_MinGW_64_bit-Release\release\StudentManagement.exe
```

## 项目文件说明

### 可执行文件
- `build\Desktop_Qt_6_9_2_MinGW_64_bit-Release\release\StudentManagement.exe` - 主程序

### 源代码文件
- `main.cpp` - 程序入口
- `mainwindow.h/cpp` - 主窗口类
- `student.h/cpp` - 学生数据类
- 其他相关UI和图表类文件

### 优化相关文件（可选）
- `optimized_mainwindow.h/cpp` - 优化版本实现
- `StudentManagementOptimized.pro` - 优化版本项目配置
- 各种优化文档和分析报告

## 注意事项
1. 原始程序已恢复可运行状态
2. VSCode中的头文件错误是IntelliSense问题，不影响程序运行
3. 优化版本需要Qt开发环境才能编译

## 如需重新编译
在有Qt开发环境的情况下：
```bash
qmake StudentManagement.pro
make
