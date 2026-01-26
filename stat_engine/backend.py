import ctypes 
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
dll_path = os.path.join(current_dir, "..", "cpp_core", "stats_lib.dll")

try:
    lib = ctypes.CDLL(dll_path)
except FileNotFoundError:
    raise RuntimeError(f"Library not found at {dll_path}")

class C_StatResult(ctypes.Structure):
    _fields_ = [('mean', ctypes.c_double), ('standard_deviation', ctypes.c_double)]

calculate_stats = lib.calculate_stats
calculate_stats.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
calculate_stats.restype = ctypes.POINTER(C_StatResult)

delete_StatResult = lib.delete_StatResult
delete_StatResult.argtypes = [ctypes.POINTER(C_StatResult)]