# tc\_ble\_sdk README

- [English](./README.md)

# SDK 介绍

tc\_ble\_sdk 是一个面向低功耗蓝牙（Bluetooth® Low Energy，BLE）多连接物联网应用的软件开发平台，基于泰凌微电子 TLSR825x、TLSR827x、TC321x、TC123x 等系列 SoC，帮助开发者快速构建高性能、低功耗的 BLE 产品。

SDK 提供完整的软件框架，包括 BLE 协议栈、硬件驱动、系统服务及示例工程，支持从产品原型开发到量产部署的完整开发流程。

tc\_ble\_sdk 采用多角色、多连接架构，可满足无线电脑外设、智能遥控、智能音频、可穿戴设备、电子货架标签等典型 BLE 多连接应用的开发需求。

>- tc\_ble\_sdk，最多支持 4 个 Central 和 4 个 Peripheral 连接，不支持角色自由转换。

**核心能力**

| 类别 | 能力 |
| --- | --- |
| 无线连接 | BLE 协议栈、多角色、多连接、PAwR/PAST、L2CAP CoC |
| 驱动 | GPIO、UART、SPI、I²C、USB、Flash、Timer、ADC、PWM、DMA 等 |
| 开发框架 | 裸机框架、应用开发框架、多连接管理 |
| 系统服务 | 电源管理、存储管理、时钟管理、系统初始化 |
| 安全 | SMP、安全连接、AES-CCM |
| 工具 | 调试日志、flash 存储管理、Assert |
| OTA | 无线固件升级 |
| Examples | 典型参考工程 |

**典型应用**

| 无线技术 | 应用领域 | 典型产品 |
| --- | --- | --- |
| Bluetooth® LE / 2.4GHz 专有协议 | 无线电脑外设 | 无线鼠标、键盘、游戏手柄等 |
| Bluetooth® LE | 智能遥控 | 红外遥控器、语音遥控器、学习型遥控器等 |
| Bluetooth® LE | 可穿戴设备 | 智能手环、智能手表、健康监测设备等 |
| 2.4GHz 专有协议 | 低延迟无线外设 | 低延迟无线鼠标、游戏键盘等 |
| Bluetooth® LE | 工业物联网 | 温湿度传感器、无线开关、智能仪表、电子货架标签等 |

**支持信息**

关于完整、准确的芯片型号、对应的开发板、开发平台以及 SDK 版本的详细信息，请参考 [Release Notes](./tc_ble_sdk_Release_Note.md)。

# 文档和资源

**文档导航**

| 文档 | 说明 |
| --- | --- |
| [快速入门](./doc/tc_ble_sdk_get_started_cn.md) | 开发环境配置、SDK获取及快速上手方法 |
| [开发手册](https://doc.telink-semi.cn/doc/zh/software/res/sdk/ble/tc_ble_sdk_multi_connection_cn/tc_ble_sdk_multi_connection_cn/) | 详细的软件架构、仓库结构及功能模块说明 |
| [Release Notes](./tc_ble_sdk_Release_Note.md) | 支持平台、版本说明及详细变化 |

**社区与资源**

| 资源 | 说明 |
| --- | --- |
| [Telink 官方论坛](https://forum.telink-semi.cn/) | 技术交流与支持 |
| [Telink 官方网站](https://www.telink-semi.com/) | 产品中心及文档中心 |
| [Github](https://github.com/telink-semi/tc_ble_sdk) / [Gitee](https://gitee.com/telink-semi/tc_ble_sdk.git) | SDK 源码仓库 |

# 许可证

本项目采用以下许可证：

**Apache License, Version 2.0**

Licensed under the Apache License, Version 2.0 (the "License");

You may not use this file except in compliance with the License.

You may obtain a copy of the License at:

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.
