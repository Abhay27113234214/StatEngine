from .backend import lib, mean, median, calculate_moment
import math
import ctypes

class StatArray:
    def __init__(self, py_list):
        if not py_list:
            raise ValueError("StatArray cannot be empty")
        self.size = len(py_list)
        self._cont_list = (ctypes.c_double * self.size)(*py_list)
        self._mean = None
        self._ssd = None
        self._median = None

    def _check_closed(self):
        if self._cont_list is None:
            raise ValueError("Cannot perform operations on a closed StatArray")

    def mean(self):
        self._check_closed()
        if self._mean is None:
            m = mean(self._cont_list, self.size)
            if math.isnan(m):
                raise ValueError("The list passed was empty")
            self._mean = m
        return self._mean
    
    def std(self, ddof = 0):
        self._check_closed()
        if (ddof >= self.size):
            raise ValueError("Invalid value passed for the ddof parameter")
        if self._ssd is None:
            self.mean()
            self._ssd = calculate_moment(self._cont_list, self.size, 2, self._mean) * self.size
        return (self._ssd/(self.size - ddof))**(1/2)

    def variance(self, ddof = 0):
        self._check_closed()
        if (ddof >= self.size):
            raise ValueError("Invalid value passed for the ddof parameter")
        if self._ssd is None:
            self.mean()
            self._ssd = calculate_moment(self._cont_list, self.size, 2, self._mean) * self.size
        return (self._ssd/(self.size - ddof))
    
    def skew(self):
        self._check_closed()
        self.mean()
        if self._ssd is None:
            self._ssd = calculate_moment(self._cont_list, self.size, 2, self._mean) * self.size
        third_moment = calculate_moment(self._cont_list, self.size, 3, self._mean)
        sk = third_moment / ((self._ssd / self.size) ** (3/2))
        return sk
        
    def kurtosis(self, fisher = True):
        self._check_closed()
        self.mean()
        if self._ssd is None:
            self._ssd = calculate_moment(self._cont_list, self.size, 2, self._mean) * self.size
        fourth_moment = calculate_moment(self._cont_list, self.size, 4, self._mean)
        kurt = fourth_moment / ((self._ssd / self.size) ** 2)
        if not fisher:
            return kurt
        return kurt - 3
    
    def median(self):
        self._check_closed()
        if self._median is None:
            med = median(self._cont_list, self.size)
            if math.isnan(med):
                raise ValueError("The list passed was empty")
            self._median = med
        return self._median

    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        self._cont_list = None

