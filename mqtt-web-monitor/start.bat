@echo off
chcp 65001 >nul
setlocal enableextensions enabledelayedexpansion

REM �̶����ű�����Ŀ¼����
cd /d %~dp0
echo ��ǰĿ¼: %cd%

REM ��ӡ Node / npm �汾��·��
echo ��� Node.js �� npm ����...
where node
node --version 2>nul
where npm
npm --version 2>nul

if %errorlevel% neq 0 (
    echo ����: δ�ҵ�Node.js��npm�����Ȱ�װNode.js����ѡ"Add to PATH"
    echo ���ص�ַ: https://nodejs.org/
    pause
    exit /b 1
)

REM ������װ�����״λ�ȱʧʱ��
if not exist "node_modules" (
    echo ���ڰ�װ����(��ϸ��־д�� install.log)...
    npm install --verbose > install.log 2>&1
    if %errorlevel% neq 0 (
        echo ����: ������װʧ�ܣ���鿴 install.log ��ȡ��ϸ������Ϣ
        pause
        exit /b 1
    )
)

echo ����ͨ�� npm ����...
call npm start
set "npmExit=%errorlevel%"
echo npm start �˳���: %npmExit%

if not "%npmExit%"=="0" (
    echo npm ����ʧ�ܣ����� node ֱ�������������� package.json��...
    echo.
    node server\app.js
    set "nodeExit=%errorlevel%"
    echo node ֱ�������˳���: %nodeExit%
)

echo �ű���������������˳�...
pause
