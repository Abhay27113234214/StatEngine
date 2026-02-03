import ctypes 
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
dll_path = os.path.join(current_dir, "..", "cpp_core", "stats_lib.dll")

try:
    lib = ctypes.CDLL(dll_path)
except FileNotFoundError:
    raise RuntimeError(f"Library not found at {dll_path}")

mean = lib.mean
mean.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
mean.restype = ctypes.c_double

ssd = lib.ssd
ssd.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_double]
ssd.restype = ctypes.c_double

median = lib.median
median.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
median.restype = ctypes.c_double