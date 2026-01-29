from .backend import lib, calculate_stats, delete_StatResult
import ctypes

class StatArray:
    def __init__(self, py_list):
        if not py_list:
            raise ValueError("StatArray cannot be empty")
        self.size = len(py_list)
        self.py_list = py_list
        self._mean = None
        self._std = None
        self._variance = None
        self._median = None
        self._stats_ptr = None
    
    def _compute(self):
        if self._stats_ptr is None:
            data = (ctypes.c_double * self.size)(*self.py_list)
            if (self.size > 0):
                self._stats_ptr = calculate_stats(data, self.size)
                self._mean = self._stats_ptr.contents.mean
                self._std = self._stats_ptr.contents.standard_deviation
                self._variance = self._stats_ptr.contents.variance
                self._median = self._stats_ptr.contents.median

    @property
    def mean(self):
        self._compute()
        return self._mean
    
    @property
    def std(self):
        self._compute()
        return self._std

    @property
    def variance(self):
        self._compute()
        return self._variance
    
    @property
    def median(self):
        self._compute()
        return self._median
    
    def __enter__(self):
        self._compute()
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        if self._stats_ptr is not None:
            delete_StatResult(self._stats_ptr)
            self._stats_ptr = None


    def __del__(self):
        if self._stats_ptr is not None:
            delete_StatResult(self._stats_ptr)
            self._stats_ptr = None
