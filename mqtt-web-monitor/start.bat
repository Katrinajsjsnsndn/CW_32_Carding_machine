@echo off
chcp 65001 >nul
setlocal enableextensions enabledelayedexpansion

REM 固定到脚本所在目录运行
cd /d %~dp0
echo 当前目录: %cd%

REM 打印 Node / npm 版本与路径
echo 检查 Node.js 与 npm 环境...
where node
node --version 2>nul
where npm
npm --version 2>nul

if %errorlevel% neq 0 (
    echo 错误: 未找到Node.js或npm，请先安装Node.js并勾选"Add to PATH"
    echo 下载地址: https://nodejs.org/
    pause
    exit /b 1
)

REM 依赖安装（仅首次或缺失时）
if not exist "node_modules" (
    echo 正在安装依赖(详细日志写入 install.log)...
    npm install --verbose > install.log 2>&1
    if %errorlevel% neq 0 (
        echo 错误: 依赖安装失败，请查看 install.log 获取详细错误信息
        pause
        exit /b 1
    )
)

echo 尝试通过 npm 启动...
call npm start
set "npmExit=%errorlevel%"
echo npm start 退出码: %npmExit%

if not "%npmExit%"=="0" (
    echo npm 启动失败，改用 node 直接启动（不依赖 package.json）...
    echo.
    node server\app.js
    set "nodeExit=%errorlevel%"
    echo node 直接启动退出码: %nodeExit%
)

echo 脚本结束。按任意键退出...
pause
