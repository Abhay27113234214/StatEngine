from .backend import lib, calculate_stats, delete_StatResult
import ctypes

class StatArray:
    def __init__(self, py_list):
        self.size = len(py_list)
        self.py_list = py_list
        self._mean = None
        self._std = None
        self._stats_ptr = None
    
    def _compute(self):
        if self._stats_ptr is None:
            data = (ctypes.c_double * self.size)(*self.py_list)
            self._stats_ptr = calculate_stats(data, self.size)
            self._mean = self._stats_ptr.contents.mean
            self._std = self._stats_ptr.contents.standard_deviation

    def mean(self):
        self._compute()
        return self._mean
    
    def std(self):
        self._compute()
        return self._std
    
    def __del__(self):
        if self._stats_ptr is not None:
            delete_StatResult(self._stats_ptr)
