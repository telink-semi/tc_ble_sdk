# tc_ble_sdk Get Started <a id="tc-ble-sdk-快速入门"></a>

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

## Overview <a id="概述"></a>

### About this document <a id="关于本文档"></a>

This document is to help you quickly set up the tc_ble_sdk development environment, obtain the SDK, build and flash the project, and successfully run your first example project. Reading this document requires a basic understanding of C language and embedded development concepts.

### Applicable scope <a id="适用范围"></a>

tc_ble_sdk is applicable to Telink Semiconductor's TLSR825x, TLSR827x, TC321x, TC123x and other SoC series:

- [TLSR825x](https://doc.telink-semi.cn/doc/datasheet/DS-TLSR8258-E_Datasheet_for_Telink_Multi-Standard_Wireless_SoC_TLSR8258.pdf)
- [TLSR827x](https://doc.telink-semi.cn/doc/datasheet/DS-TLSR8278-E_Datasheet_for_Telink_Multi-Standard_Wireless_SoC_TLSR8278.pdf)
- [TC321x](https://doc.telink-semi.cn/doc/datasheet/DS-TC321x-E_Datasheet_for_Telink_SoC_TC3216_TC3215.pdf)

!!! note "Notes"

    The information listed here may not be the latest version. For detailed information on complete and accurate chip series, corresponding EVBs, development platforms, and SDK versions, refer to the latest *Release Notes*. Open the [Release Notes](https://gitee.com/telink-semi/tc_ble_sdk/blob/master/doc/) page, and select the Release Notes corresponding to your current SDK version from the dropdown list on the left.

## Preparation <a id="开发准备"></a>

### Hardware checklist <a id="硬件准备"></a>

| Hardware      | Description                                                |
| ------------- | --------------------------------------------------------- |
| PC            | Windows 10/11 / Linux / macOS (This document uses Windows as an example) |
| EVB           | Select the appropriate Evaluation Board (EVB) based on the *Release Notes* |
| Programmer    | Programmer V3 (recommended)                               |
| USB cable     | Connect your PC and the programmer                        |
| Dupont wire   | Connect the EVB and the programmer                        |

For physical hardware, refer to the section [Connect the hardware](#步骤一连接硬件).

### Software checklist <a id="软件准备"></a>

| Software   | Description                                                                                                      |
| ---------- | --------------------------------------------------------------------------------------------------------------- |
| IDE        | [Telink VS Code Extension](#telink-vs-code-extension-开发环境) or [Telink IoT Studio](#telink-iot-studio-开发环境) |
| Toolchain  | For toolchain information, see *Release Notes*                                                                  |
| Programmer software | BDT (Burning and Debugging Tool)                                                                       |
| SDK        | [GitHub](https://github.com/telink-semi/tc_ble_sdk) / [Gitee](https://gitee.com/telink-semi/tc_ble_sdk.git)     |

## Choose the IDE <a id="选择开发环境"></a>

tc_ble_sdk supports the following two development methods:

- Method 1: [Telink VS Code Extension](#telink-vs-code-extension-开发环境)
- Method 2: [Telink IoT Studio](#telink-iot-studio-开发环境)

Choose one based on your development habits. Both methods do not require simultaneous installation.

### Telink VS Code Extension <a id="telink-vs-code-extension-开发环境"></a>

#### Install VS Code <a id="安装-vs-code"></a>

Before installing the extension, make sure your computer has the latest version of [Visual Studio Code](https://code.visualstudio.com/) installed. Telink VS Code Extension updates are based on the latest version of VS Code. If the VS Code version is too old, the extension may not work properly or may have abnormal functionality.

#### Install Telink VS Code Extension <a id="安装telink-vs-code-extension"></a>

1. Launch VS Code.
2. Click the **Extensions** icon in the left sidebar, or press **Ctrl+Shift+X**.
3. Enter **Telink** in the search box.
4. Find **Telink Development Tool** in the search results and click **Install**.

    ![xYNUBhlc\.png](figures/xYNUBhlc.png)

After installation, the **Telink DEVELOPMENT** icon appears in the left sidebar of VS Code.

#### Install the toolchain <a id="安装工具链"></a>

In Telink VS Code Extension, download the compiler toolchain and auxiliary tools on demand via the built-in manager.

1\. In the **Telink DEVELOPMENT** view on the left side of VS Code, expand the **DEVELOPMENT TOOLS** tree menu.

![RMNXUyMv\.png](figures/RMNXUyMv.png)

2\. Click the toolchain you need, then click **Install Toolchain** on the right.

3\. Click **CMake**, then click **Install Toolchain** on the right.

4\. After successful installation, a corresponding completion notification pops up in the bottom right corner of the VS Code window.

![hy4A0lQZ\.png](figures/hy4A0lQZ.png)

!!! info "Info"

    If you want to use the toolchain in the command line, right-click and select **Open Terminal** from the pop-up menu.

#### Validate the IDE and the dependencies <a id="环境验证"></a>

After installation, follow the steps below to validate whether **Telink VS Code Extension** has been installed successfully:

1. Launch VS Code, confirm the software opens normally, and there are no error messages during startup.
2. In the left sidebar, confirm the **Telink** icon appears, and click it to enter the **Telink DEVELOPMENT** view.
3. In the **DEVELOPMENT TOOLS** tree menu, confirm that the required toolchain has been installed.
4. At the same time, confirm that the auxiliary tool **CMake** also shows it has been installed.

If all the above checks are normal, **Telink VS Code Extension** and its dependencies have been successfully installed and configured, and you can start [downloading the SDK](#下载-sdk) to begin SDK development.

### Telink IoT Studio <a id="telink-iot-studio-开发环境"></a>

#### Download and install Telink IoT Studio <a id="下载并安装-telink-iot-studio"></a>

Download the installation package corresponding to your operating system. The demonstration uses Windows as an example:

- Windows : [TelinkIoTStudio\_V2025.2.zip](https://doc.telink-semi.cn/tools/telink_iot_studio/TelinkIoTStudio_V2025.2.zip)
- Linux : [Telink\_IoT\_Studio\_2025.2\_Installer.run](https://doc.telink-semi.cn/tools/telink_iot_studio/Telink_IoT_Studio_2025.2_Installer.run)

**Install Telink IoT Studio:**

Windows:

1. Extract the downloaded `.zip` file, run *TelinkIoTStudio\_V2025.2.exe*, and follow the wizard to complete the installation.
2. After installation, you must run *TelinkIoTStudio Updater.exe* to get the latest component updates.

Linux:

1. Grant installation package execution permissions to the runtime environment.
2. Run [Telink\_IoT\_Studio\_2025.2\_Installer.run](https://doc.telink-semi.cn/tools/telink_iot_studio/Telink_IoT_Studio_2025.2_Installer.run) and follow the prompts in the terminal to complete the installation.

#### Toolchain description <a id="工具链说明"></a>

Telink IoT Studio offers an all-in-one standalone installation package with all the components required built in, eliminating the need for additional toolchain installation. All required toolchains are installed synchronously when installing the IDE. By clicking **Telink** in the top menu bar, open the corresponding toolchain's **Cygwin Shell**.

![GjUSVdDz\.png](figures/GjUSVdDz.png)

#### Validate the IDE and the dependencies <a id="环境验证-2"></a>

After installation, follow the steps below to validate whether **Telink IoT Studio** has been successfully installed.

Validate the IDE installation:

1. Launch **Telink IoT Studio**, confirm the program loads normally and displays the main interface, and there are no error messages about missing dependency libraries during startup.
2. Click **Telink** in the top menu bar, confirm that toolchain entries such as **Cygwin Shell** in the dropdown menu can be opened normally.

If the above conditions are met, **Telink IoT Studio** has been successfully installed and configured.

Validate the dependencies:

SDK compilation and version management rely on the following tools. Run the corresponding commands in the system terminal to validate.

| Dependencies | Validation commands                                     | Expected versions                        |
| ------------ | ------------------------------------------------------- | ---------------------------------------- |
| Python       | `python --version` or `python3 --version`              | Python 3.8 or higher (recommended 3.8~3.12) |
| Git          | `git --version`                                         | Git 2.30 or higher                       |

After confirming that all the above dependencies have been correctly installed, you can start [downloading the SDK](#下载-sdk) to begin SDK development.

## Get the SDK <a id="获取-sdk"></a>

This chapter introduces how to obtain the official Telink SDK, the top-level directory structure of the SDK, and how to import the SDK project into the IDE.

### Download the SDK <a id="下载-sdk"></a>

Download the latest SDK via [GitHub](https://github.com/telink-semi/tc_ble_sdk) / [Gitee](https://gitee.com/telink-semi/tc_ble_sdk.git), or run the following git clone command to clone the repository locally:

`git clone https://github.com/telink-semi/tc_ble_sdk.git`

or

`git clone https://gitee.com/telink-semi/tc_ble_sdk.git`

### SDK directory overview <a id="sdk-目录概览"></a>

tc_ble_sdk/

├── algorithm/ # Algorithm library

├── application/ # General application programs

├── boot/ # Boot code

├── common/ # Common utility functions

├── drivers/ # Peripheral drivers

├── proj\_lib/ # Core library (protocol stack, PM, etc.)

├── stack/ # Protocol stack

└── vendor/ # User application code

For the full directory structure and the function of each directory, refer to the [Developer Handbook](https://doc.telink-semi.cn/doc/en/software/res/sdk/ble/tc_ble_sdk_multi_connection_en/tc_ble_sdk_multi_connection_en/).

### Import the project <a id="导入工程"></a>

Depending on your development environment, choose to import the project in VS Code or Telink IoT Studio.

#### Import the project in VS Code <a id="在-vs-code-中导入工程"></a>

tc_ble_sdk is a standard Telink VS Code Extension project. Click **File** -> **Open Folder**... in the VS Code menu bar, select the root directory of the project, and load it.

After loading, click the **Telink** icon in the left sidebar to enter the Telink Extension view. At this point, the **PROJECT OUTLINE** panel displays the directory structure of the current project, indicating the project has been successfully imported.

![adfee325-9e78-460e-8071-0f9e04afc90b\.png](figures/d73e15da54f3702ba502f6874ff77c9d.png)

After loading, if the **PROJECT OUTLINE** panel is empty, it means the current project is an older version of Telink IoT Studio project, and a conversion operation is required.

![d91778c1af2b59e12f052ee9d7550ee5\.png](figures/d91778c1af2b59e12f052ee9d7550ee5.png)

After successful project import, [compile the project in VS Code](#在-vs-code-中编译工程).

#### Import the project in Telink IoT Studio <a id="在-telink-iot-studio-中导入工程"></a>

If you choose to use Telink IoT Studio as the development environment, follow the steps below to import the project:

1. Click **File** -> **import** in the top menu bar.
2. In the pop-up dialog box, expand **General** directory, select **Existing Projects into Workspace**, and click **Next**.
3. Select the project directory to import.

    ![rVIXdM3K\.png](figures/rVIXdM3K.png)

4. Select the project you have already downloaded from **Browse**, select the project on the corresponding chip platform, and click **Finish**. The project is successfully imported.

    ![z05jPCh8\.png](figures/1546b95e5eb0c7a3f3e75acd4e91618e.png)

After successful project import, the figure below is shown:

![UsyTzLfm\.png](figures/a7d126213f4608ecc2e60b66c05fca50.png)

After successful project import, [compile the project in Telink IoT Studio](#在-telink-iot-studio-中编译工程).

## Compile and run the first example <a id="编译并运行第一个示例"></a>

This chapter guides you through the complete development loop: from selecting an example and code compilation to final flashing and execution.

### Select examples <a id="选择示例"></a>

It is recommended to use the officially provided `acl_peripheral_demo` as the first example project. This example features streamlined functionality with minimal dependencies, allowing quick verification of the basic connection function of the development board as a BLE Peripheral. This example uses the TC321x development board for demonstration.

For more examples, refer to *...\tc\_ble\_sdk\tc\_ble\_sdk\vendor*.

### Compile the project <a id="编译工程"></a>

Depending on your development environment, choose to compile the project in VS Code or Telink IoT Studio.

#### Compile the project in VS Code <a id="在-vs-code-中编译工程"></a>

1. In the left sidebar of VS Code, find **PROJECT OUTLINE**, and under the **Other targets** directory, select the `acl_peripheral_demo` project.

    ![561484490512ae7683a7de336577d774\.png](figures/561484490512ae7683a7de336577d774.png)

2. Click the build button on the right to compile.

    After compilation, output files (`.elf`, `.bin`, `.ls`) are generated, with `ELF` format files used for debugging and `BIN` format files for flashing.

    `ELF` and `BIN` files can be found under **PROJECT OUTLINE** after compilation. Taking the TL321x platform `acl_peripheral_demo` as an example, the storage path is *...\tc\_ble\_sdk\tc\_ble\_sdk\cmake\_builds\TC\_BLE\_SDK\_321X\TC32\_ELF\_MCULIB\_V5\_GCC12\_2*

    Compilation successful, seeing a similar output:

    ![801fe00e126c3814a4e84d13e1aa3d78\.png](figures/801fe00e126c3814a4e84d13e1aa3d78.png)

#### Compile the project in Telink IoT Studio <a id="在-telink-iot-studio-中编译工程"></a>

1. In the **Project Explorer** on the left, select the project you want to compile.
2. Click the small dropdown arrow next to the Build button (hammer icon) on the top toolbar.
3. Select the configuration you want to compile from the dropdown menu, and the system begins compiling.

    ![KeDG960i\.png](figures/6ebe5507bc4f513f7372d9d917f12163.png)

4. Observe the console output; if there are no error messages and **Build Finished** is displayed, the compilation is successful.

    ![t9bUXdep\.png](figures/b72583560fa2ebb8abcdd021511fb35b.png)

Taking the TC321x platform `acl_peripheral_demo` as an example, the compiled `bin` file is stored in the *...\tc\_ble\_sdk\tc\_ble\_sdk\build\TC321X\acl\_peripheral\_demo* directory.

### Flash the firmware <a id="烧录程序"></a>

Firmware flashing is performed using the BDT tool.

#### Step 1: Connect the hardware <a id="步骤一连接硬件"></a>

Before using the BDT tool, connect the PC, the programmer, and the target board as follows:

- PC ↔ Programmer: Connect via USB cable. If the green indicator light on the programmer stays on, it indicates the programmer has been successfully recognized by the PC.

- Programmer ↔ Target board: Connect using the Dupont wire:

    - Power cable: VCC ↔ VCC; GND ↔ GND

    - Data cable (Single-wire SWM bus): Connect the programmer's SWM pin to the target board's SWS (Swire) pin.

![zQuliQjg\.png](figures/connect_hardware.jpg)

#### Step 2: Install the BDT <a id="步骤二-安装-bdt"></a>

1. Download BDT.

    Visit [Telink Developer Center - Development Tools](https://www.telink-semi.cn/development-tools) and download the corresponding toolkit according to your operating system.

    ![zQuliQjg\.png](figures/zQuliQjg.png)
2. Extract the toolkit. (The BDT tool is a green no-installation version that can be used after extraction.)

    Take Windows as an example:

    First, extract the downloaded toolkit to a custom path, such as *C:\Telink\BDT*.

    Then, enter the extracted release directory, for example *C:\Telink\BDT\release\_v5.9.2*.

    Finally, double-click and run *Telink BDT.exe* to launch the tool.

    ![vEJjDVYV\.png](figures/vEJjDVYV.png)
3. Choose the programmer version and chip compatibility

    Telink provides four versions of programmers (Programmer). When launching the BDT interface, it is recommended to select the **Programmer V1.0 ~ V3.0** programmer version.

    ![bXIoTEeZ\.png](figures/bXIoTEeZ.png)

#### Step 3: Flash the firmware <a id="步骤三烧录固件"></a>

1. Detect the device.

    Open the BDT software. Before firmware flashing, ensure the tool has detected the programmer, as shown in the figure below.

    ![PtCQKTDc\.png](figures/e89dc3000c720383a086005420a68428.png)

    - If the BDT does not find the device, click **Refresh** to view available devices.
    - If the BDT finds multiple devices, all of them will be listed. After clicking **Refresh**, the first device is activated by default, and you can also manually switch target devices from the list.

2. Configure and flash

    a. Select the chip series of the target board.

    In the chip dropdown menu at the top, select the target chip for your development board, such as **TC321x**.

    ![HiXloTix\.png](figures/37b2b3c5da1bfbcdc34476e3c9f0fc9a.png)

    !!! info "Info"

        B91 includes TLSR951x and TLSR921x, B92 includes TLSR952x and TLSR922x, B80/B80B includes TLSR8208 and TLSR8373, B85 includes TLSR825x and TLSR8359, B87 includes TLSR827x and TLSR8355.

    b. Select the download mode as **EVK**.

    ![rhFFi2tI\.png](figures/5ccde3bd8bb65512793b6c95693b823d.png)

    c. Click **Setting** button to open the configuration window and switch to the **Flash** tab. In the **Download Addr** bar, set the initial offset address for firmware, with the default value **0x000000**.

    !!! info "Info"

        You can download the target firmware to the SRAM or OTP target area by configuring the **SRAM** or **OTP** tab


    ![VQVyClPH\.png](figures/af58f3823a055371e9fcb459c36799b2.png)
    
    d. Click **File** -> **Open**/**Reopen** in the menu bar and select the `.bin` firmware you compiled. After successful loading, the full file path is displayed at the bottom of the main interface.![E9Cvzp8V\.png](figures/28c9e0400a5c7da554de26bb9e5a5f3d.png)
    ![j7oxcdEg\.png](figures/1f4d0a4694d6efadd66b6868f5c4a11c.png)
    
    e. Check the connection status between the target board and the PC.
    
    - If the connection status is normal, the bottom left corner of the main interface displays **evk device: ok**.
    - If the bottom left corner of the main interface displays **usb device: not found**, it means the target board is not properly connected to the PC.
    
    ![Tu5y7O8v\.png](figures/45343c8050265ec367da4c95cc56ef24.png)
    
    f. Click **Activate** first, then click **Unlock** button or check the **auto unlock** option to remove Flash's write protection.
    
    If flash is in write protection, the Download will fail, so before downloading firmware to flash, ensure flash is programmable.
    ![nFWjaksi\.png](figures/f40c6e6be06e1c88228ebb0f118a909e.png)
    
    g. Click **Download** button and wait for the log window to prompt flashing successful.
    ![5nXxhBi6\.png](figures/f0662c8737d231fe86d37454a033eb89.png)
    
    h. You can set the reset mode to **auto mode** before flashing, or switch to **manual mode** after flashing and click the **Reset** button to manually reset the MCU to run the program.
    ![ymJ63XmE\.png](figures/2572f1960c736db6b6ff0715005532a6.png)

### Verify the running results <a id="验证运行结果"></a>

After firmware flashing is complete, follow the steps below to verify whether the example program is running properly on the development board.

**Operation steps**

1. After flashing is complete, the development board automatically resets; if it does not reset automatically, press the **Reset** button on the development board to reset.
2. Enable Bluetooth on your phone, search for a BLE device named `periphr_demo` and complete the connection.
3. Press the volume up/down button on the matrix keyboard of the development board (red box position in the figure), and observe whether the phone volume changes.

For the GPIO pins corresponding to the buttons, refer to the configuration file *TC321X\_C1T357A20.h* in the project.

![OplbP7LC\.jpg](figures/OplbP7LC.jpg)

**Expected results**

When the program runs normally, the following conditions should be met:

- The phone can search for and successfully connect to the `periphr_demo` device.
- After successful connection, the LED indicator on the development board lights up.
- After pressing the volume up/down button on the matrix keyboard, the phone volume can be synchronously increased or decreased.

If all the above phenomena meet expectations, it indicates the example program is running successfully, and the development environment, program compilation, firmware flashing, and Bluetooth communication functions have all been verified successfully.

## References and resources <a id="参考文档和资源"></a>

**Document navigation**

| Document                                                                                                                                            | Description               |
| --------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------- |
| [Telink tc\_ble\_sdk Developer Handbook](https://doc.telink-semi.cn/doc/en/software/res/sdk/ble/tc_ble_sdk_multi_connection_en/tc_ble_sdk_multi_connection_en/) | Learn SDK functions and specific usage methods |

**Community and resources**

| Resource                                                                                                      | Description                |
| ------------------------------------------------------------------------------------------------------------- | -------------------------- |
| [Telink Official Forum](https://forum.telink-semi.cn/)                                                        | Technical exchange and support |
| [Telink Official Website](https://www.telink-semi.com/)                                                      | Product center and documentation center |
| [GitHub](https://github.com/telink-semi/tc_ble_sdk) / [Gitee](https://gitee.com/telink-semi/tc_ble_sdk.git) | SDK source code repository |

## Appendix 1: Frequently Asked Questions <a id="附录1常见问题"></a>

### Environment installation failed <a id="环境安装失败"></a>

- Possible reason: The installation path contains Chinese characters.
- Solution: Modify the installation path.
- Possible reason: Antivirus software was not closed during installation.
- Solution: Close the antivirus software running on your computer.

### Unable to generate the required `.bin` file when compiling with CMake in VS Code <a id="在-vs-code-中使用-cmake-编译时无法生成所需的-bin-文件"></a>

- Possible reason: The `.elf` file name generated by CMake does not match the `.elf` file name expected by the script `tl_check_fw.sh`, causing the script execution to fail and thus unable to generate the final `.bin` file.
- Solution: Check and ensure that the `.elf` file name output in the CMake configuration is exactly the same as the `.elf` file name referenced in the `tl_check_fw.sh` script. If any inconsistency is found, modify both to the same file name, then recompile.

### Flashing failed <a id="烧录失败"></a>

- Possible reason: The development board has not been **Activate**d or **Unlock**ed.
- Solution: You need to **Activate** + **Unlock** again before each **download**.

### Phone cannot connect to the board <a id="手机连接不到板子"></a>

- Possible reason: The development board does not have an antenna installed.
- Solution: Install an antenna at the reserved antenna connector.
