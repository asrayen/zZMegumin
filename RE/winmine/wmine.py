import ctypes
from ctypes import wintypes
import psutil

PROCESS_ALL_ACCESS = 0x1F0FFF
kernel32 = ctypes.WinDLL('kernel32')

DIFFICULTY_PROFILES = {
    'beginner': (0x1005360, 11, 11),
    'intermediate': (0x1005340, 16, 16),
    'expert': (0x01005340, 32, 18)
}

FLAG_ADDR = 0x01005330


def set_flags(base_addr, width, height):
    buffer_size = width * height
    buffer = (ctypes.c_byte * buffer_size)()
    bytes_read = wintypes.SIZE()
    bytes_written = wintypes.SIZE()

    # 读取内存
    if not kernel32.ReadProcessMemory(h_process, base_addr, buffer,
                                      buffer_size, ctypes.byref(bytes_read)):
        raise RuntimeError(f"读取失败: {ctypes.get_last_error()}")

    # 修改数据
    for i in range(buffer_size):
        if buffer[i] == 0x8f:
            buffer[i] = 0x8e

    # 写回内存
    if not kernel32.WriteProcessMemory(h_process, base_addr,
                                       buffer, buffer_size,
                                       ctypes.byref(bytes_written)):
        raise RuntimeError(f"写入失败: {ctypes.get_last_error()}")


def get_minesweeper_pid():
    process_names = ["winmine.exe", "minesweeper.exe"]  # 多进程名支持
    for proc in psutil.process_iter(['pid', 'name']):
        if proc.info['name'].lower() in process_names:
            return proc.info['pid']
    return None


pid = get_minesweeper_pid()
h_process = None

try:
    if pid:
        print(f"找到扫雷进程PID: {pid}")
        h_process = kernel32.OpenProcess(PROCESS_ALL_ACCESS, False, pid)
        if not h_process:
            raise RuntimeError(f"打开进程失败: {ctypes.get_last_error()}")
        set_flags(*DIFFICULTY_PROFILES['expert'])
        print("地雷已标记！")
    else:
        print("未找到扫雷进程")
finally:
    if h_process:
        kernel32.CloseHandle(h_process)
