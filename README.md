<h1 align="center">Isaac-CoopModEnabler</h1>

<p align="center">
  <a href="#简体中文">简体中文</a> • 
  <a href="#繁體中文">繁體中文</a> • 
  <a href="#english">English</a>
</p>

<p align="center">
  <b>An effortless proxy DLL hijacking solution to enable online co-op mods for Isaac (AB+ & REP+).</b>
</p>

---

## 简体中文

一个为《以撒的结合》（AB+ & REP+ 版本）在线联机模式解除 Mod 限制的免 Hook、免修改主程序的 DLL 劫持方案。

### 📦 运行要求

使用该 Mod 需要游戏处于以下版本，并安装启用对应的 DLC：
- **以撒的结合：REP+ 版本**：需要拥有以下全部 4 个 DLC：
  1. *The Binding of Isaac: Afterbirth*
  2. *The Binding of Isaac: Afterbirth+* (AB+)
  3. *The Binding of Isaac: Repentance* (REP)
  4. *The Binding of Isaac: Repentance+* (REP+)

### 💡 工作原理

本项目采用 **DLL 代理劫持（Proxy DLL / DLL Hijacking）** 技术：

1. **自动加载**：利用 Windows 加载 DLL 的搜索顺序特征，将 `winmm.dll` 放置在游戏根目录。游戏启动时，会优先加载游戏目录下的此 DLL。
2. **无缝转发**：代理 DLL 内部实现了对系统原版 `winmm.dll` 的 178 个导出函数的无缝转发（通过裸函数 `naked asm` 直接跳转，零性能开销），确保游戏所有的多媒体功能和音频正常运行。
3. **内存热补丁**：在 `DllMain` 初始化时，代理 DLL 会扫描当前进程 `isaac-ng.exe` 在内存中的 `.text` 代码段：
   - **解除联机限制**：定位用于限制在线联机的字节码模式，将限制联机使用 Mod 的三个跳转指令（`JE`）替换为 `NOP`，使游戏按照本地单机逻辑放行所有 Mod。
   - **屏蔽 Analytics**：将遥测/分析上报函数的入口指令直接修改为 `RET`，跳过遥测数据上报。

由于补丁只在内存中生效，**不需要修改游戏原本的 `isaac-ng.exe` 文件**，因此不会破坏 Steam 的文件校验，游戏更新时也无需重新运行任何打补丁程序。

### 🚀 安装与使用方法

1. **获取 DLL**：从本仓库的 **Actions** 编译产物（Artifacts）或 **Releases** 页面下载编译好的 32 位 `winmm.dll`（注意：必须是 32 位，因为以撒主程序是 32 位的）。
2. **部署**：将 `winmm.dll` 复制到你的以撒游戏根目录中（即与 `isaac-ng.exe` 所在的同一目录下）。
3. **运行**：通过 Steam 正常启动游戏即可，联机 Mod 限制已自动解除。
4. **卸载**：如果需要纯净环境，直接删除游戏目录下的 `winmm.dll` 即可，无需恢复备份。

### 🛠️ 编译与构建

本项目主要通过 **GitHub Actions** 进行自动化交叉编译，你无需在本地搭建任何复杂的 C/C++ 32位编译环境。

#### 方式 A：GitHub Actions 编译（推荐）
1. 将本项目推送（Push）到你的 GitHub 个人仓库。
2. GitHub Actions 编译流（Workflow）会自动触发，在 `windows-latest` 环境下通过 CMake + MSVC 交叉编译出标准的 x86 32位 `winmm.dll`。
3. 编译完成后，在 Workflow 运行记录底部的 **Artifacts** 处下载 `winmm-dll`。
4. 推送版本标签（例如 `git tag v1.0.0` 并推送），Actions 会自动为你创建 GitHub Release 并上传编译好的 `winmm.dll`。

#### 方式 B：本地编译（需安装编译器）
- **Zig 编译器**：如果你有 Zig 编译器，可以直接运行 `CoopModEnabler/build_zig.bat`。它会利用 Zig CC 自动交叉编译生成 32 位 DLL。
- **Visual Studio**：如果安装了 VS（包含 C++ 桌面开发组件），在 **x86 Native Tools Command Prompt** 终端中运行 `CoopModEnabler/build.bat`。

### 🧹 仓库维护规范

1. **导出函数一致性**：若修改了代理 DLL 的导出函数，须同步更新 `winmm.def` 文件，确保 178 个导出函数无缝转发，避免游戏启动崩溃。
2. **构建验证**：提交代码前，请使用 `build_zig.bat` 或 `build.bat` 在本地进行 32 位（x86）编译测试，确保无编译错误。
3. **内存地址模式匹配**：若游戏更新导致补丁失效，需使用特征码扫描（Pattern Scanning）工具重新定位限制联机和遥测的内存地址特征，切勿硬编码绝对内存地址。
4. **版本发布**：若需要发布新版本，请在本地打上版本 Tag 并推送至 GitHub（例如 `git tag v1.x.x && git push origin v1.x.x`），Actions 会自动创建 Release 并上传产物。

### ❗ 已知问题
1. 已测试 **EID** 可以在解除联机 Mod 限制下正常使用
2. 由于游戏联机的特性，在解除联机 Mod 限制下使用其他模组极大概率出现崩溃，属于正常现象

---

## 繁體中文

一個為《以撒的結合》（AB+ & REP+ 版本）線上聯機模式解除 Mod 限制的免 Hook、免修改主程式的 DLL 劫持方案。

### 📦 運行要求

使用該 Mod 需要遊戲處於以下版本，並安裝啟用對應的 DLC：
- **以撒的結合：REP+ 版本**：需要擁有以下全部 4 個 DLC：
  1. *The Binding of Isaac: Afterbirth*
  2. *The Binding of Isaac: Afterbirth+* (AB+)
  3. *The Binding of Isaac: Repentance* (REP)
  4. *The Binding of Isaac: Repentance+* (REP+)

### 💡 工作原理

本項目採用 **DLL 代理劫持（Proxy DLL / DLL Hijacking）** 技術：

1. **自動載入**：利用 Windows 載入 DLL 的搜尋順序特徵，將 `winmm.dll` 放置在遊戲根目錄。遊戲啟動時，會優先載入遊戲目錄下的此 DLL。
2. **無縫轉發**：代理 DLL 內部實現了對系統原版 `winmm.dll` 的 178 個導出函數的無縫轉發（透過裸函數 `naked asm` 直接跳轉，零效能開銷），確保遊戲所有的多媒體功能和音效正常運作。
3. **記憶體熱補丁**：在 `DllMain` 初始化時，代理 DLL 會掃描當前程序 `isaac-ng.exe` 在記憶體中的 `.text` 代碼段：
   - **解除聯機限制**：定位用於限制線上聯機的位元組碼模式，將限制聯機使用 Mod 的三個跳轉指令（`JE`）替換為 `NOP`，使遊戲按照本地單機邏輯放行所有 Mod。
   - **屏蔽 Analytics**：將遙測/分析上報函數的入口指令直接修改為 `RET`，跳過遙測數據上報。

由於補丁僅在記憶體中生效，**不需要修改遊戲原本的 `isaac-ng.exe` 檔案**，因此不會破壞 Steam 的檔案校驗，遊戲更新時也無需重新執行任何打補丁程式。

### 🚀 安裝與使用方法

1. **獲取 DLL**：從本倉庫的 **Actions** 編譯產物（Artifacts）或 **Releases** 頁面下載編譯好的 32 位 `winmm.dll`（注意：必須是 32 位，因為以撒主程式是 32 位的）。
2. **部署**：將 `winmm.dll` 複製到你的以撒遊戲根目錄中（即與 `isaac-ng.exe` 所在的同一目錄下）。
3. **執行**：透過 Steam 正常啟動遊戲即可，聯機 Mod 限制已自動解除。
4. **解除安裝**：如果需要純淨環境，直接刪除遊戲目錄下的 `winmm.dll` 即可，無需還原備份。

### 🛠️ 編譯與構建

本項目主要透過 **GitHub Actions** 進行自動化交叉編譯，你無需在本地搭建任何複雜的 C/C++ 32位編譯環境。

#### 方式 A：GitHub Actions 編譯（推薦）
1. 將本項目推送（Push）到你的 GitHub 個人倉庫。
2. GitHub Actions 編譯流（Workflow）會自動觸發，在 `windows-latest` 環境下透過 CMake + MSVC 交叉編譯出標準的 x86 32位 `winmm.dll`。
3. 編譯完成後，在 Workflow 執行記錄底部的 **Artifacts** 處下載 `winmm-dll`。
4. 推送版本標籤（例如 `git tag v1.0.0` 並推送），Actions 會自動為你創建 GitHub Release 並上傳編譯好的 `winmm.dll`。

#### 方式 B：本地編譯（需安裝編譯器）
- **Zig 編譯器**：如果你有 Zig 編譯器，可以直接執行 `CoopModEnabler/build_zig.bat`。它會利用 Zig CC 自動交叉編譯生成 32 位 DLL。
- **Visual Studio**：如果安裝了 VS（包含 C++ 桌面開發組件），在 **x86 Native Tools Command Prompt** 終端中執行 `CoopModEnabler/build.bat`。

### 🧹 倉庫維護規範

1. **導出函數一致性**：若修改了代理 DLL 的導出函數，須同步更新 `winmm.def` 檔案，確保 178 個導出函數無縫轉發，避免遊戲啟動崩潰。
2. **建置驗證**：提交代碼前，請使用 `build_zig.bat` 或 `build.bat` 在本地進行 32 位（x86）編譯測試，確保無編譯錯誤。
3. **記憶體位址模式匹配**：若遊戲更新導致補丁失效，需使用特徵碼掃描（Pattern Scanning）工具重新定位限制聯機和遙測的記憶體位址特徵，切勿寫死絕對記憶體位址。
4. **版本發布**：若需要發布新版本，請在本地打上版本 Tag 並推送至 GitHub（例如 `git tag v1.x.x && git push origin v1.x.x`），Actions 會自動建立 Release 並上傳產物。

### ❗ 已知問題
1. 經測試 **EID** 可在解除連線 Mod 限制的情況下正常使用
2. 基於遊戲連線本身的特性，解除連線 Mod 限制後使用其他模組極有機會發生崩潰，屬正常情況

---

## English

A proxy DLL hijacking solution for *The Binding of Isaac* (AB+ & REP+ versions) to enable mods in online co-op mode without hooks or modifying the main game executable.

### 📦 Requirements

This mod supports the **Afterbirth+ (AB+)** and **Repentance+ (REP+)** version of the game. The required DLC configurations are:
- **For Repentance+ (REP+)**: Requires all 4 DLCs installed and enabled:
  1. *The Binding of Isaac: Afterbirth*
  2. *The Binding of Isaac: Afterbirth+* (AB+)
  3. *The Binding of Isaac: Repentance* (REP)
  4. *The Binding of Isaac: Repentance+* (REP+)

### 💡 How It Works

This project utilizes **Proxy DLL / DLL Hijacking** technique:

1. **Automatic Loading**: Leverages the Windows DLL search order by placing `winmm.dll` in the game directory. The game will prioritize loading this local DLL over the system one at startup.
2. **Seamless Forwarding**: Internally proxies and forwards all 178 exported functions of `winmm.dll` to the real system library (`C:\Windows\System32\winmm.dll`) using zero-overhead naked assembly jump (`naked asm`) trampolines, ensuring all game audio and multimedia features remain intact.
3. **Memory Hot-patching**: On DLL initialization (`DllMain`), the proxy scans the `.text` segment of `isaac-ng.exe` in memory:
   - **Bypassing Co-op Mod Restriction**: Searches for the bytecode sequence that restricts mod usage in online matches, replacing the three conditional jump (`JE`) instructions with `NOP`s, forcing the game to bypass mod checks.
   - **Disabling Analytics**: Replaces the telemetry/analytics function entry instruction with a `RET`, skipping the data collection process.

Since the modifications only occur in the process memory, **the original game file `isaac-ng.exe` remains untouched**. It doesn't trigger Steam file integrity checks and generally survives game updates.

### 🚀 Installation and Usage

1. **Get the DLL**: Download the pre-built 32-bit `winmm.dll` from the **Actions** artifacts or the **Releases** page of this repository (Make sure it is x86/32-bit, as Isaac is a 32-bit application).
2. **Deploy**: Copy `winmm.dll` into your game directory (next to `isaac-ng.exe`).
3. **Run**: Launch the game normally via Steam. The mod restriction is now automatically lifted.
4. **Uninstall**: Delete `winmm.dll` from your game directory. The game will revert to vanilla instantly with no backup files needed.

### 🛠️ Compilation and Building

The project is configured for **GitHub Actions** out of the box, meaning you don't need to configure a local C/C++ 32-bit compiler toolchain.

#### Method A: Build via GitHub Actions (Recommended)
1. Push this repository to your personal GitHub account.
2. The compilation workflow will trigger automatically on `windows-latest` using CMake and MSVC to produce the 32-bit x86 `winmm.dll`.
3. Once completed, you can download `winmm-dll` from the **Artifacts** section at the bottom of the workflow run summary.
4. Pushing a tag (e.g. `git tag v1.0.0` followed by `git push origin v1.0.0`) will automatically compile and release the DLL on GitHub Releases.

#### Method B: Local Build (Compiler required)
- **Zig Compiler**: If you have Zig installed, run `CoopModEnabler/build_zig.bat`. It will invoke `zig cc` to cross-compile the 32-bit DLL.
- **Visual Studio**: If you have VS with C++ Desktop tools, run `CoopModEnabler/build.bat` from an **x86 Native Tools Command Prompt**.

### 🧹 Repository Maintenance

1. **Export Function Consistency**: If you modify the proxy DLL's exports, you must update the `winmm.def` file accordingly to ensure all 178 exported functions are seamlessly forwarded. This prevents the game from crashing on startup.
2. **Build Verification**: Before committing code, run `build_zig.bat` or `build.bat` to perform a local 32-bit (x86) test compilation and check for errors.
3. **Memory Pattern Matching**: If a game update breaks the patches, use pattern scanning to relocate the online-restriction and telemetry bytecode sequences in memory. Do not hardcode absolute memory addresses.
4. **Releasing Versions**: To release a new version, create and push a git tag (e.g., `git tag v1.x.x && git push origin v1.x.x`). GitHub Actions will automatically create a Release and upload the built artifacts.

### ❗ Known Issues
1. It has been tested that **EID** works normally when the online mod restrictions are lifted.
2. Due to the nature of online gameplay, using other mods while the online mod restrictions are lifted has a high chance of causing crashes, which is considered normal.

---

## ⚠️ Disclaimer / 免责声明 / 免責聲明

- This project is for personal, friends-only co-op games. Do not use mods to ruin other players' experiences in public matches.
- All participants should ideally have the same mods installed to avoid synchronization issues (Desync).
- 本项目仅供联机开黑娱乐与学习研究使用，请勿在公共匹配中作弊或破坏秩序。
