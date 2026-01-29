from stat_engine import StatArray
import random

# data = [random.random() * 100 for _ in range(1_000_000)]

# arr = StatArray(data)

# print(arr.mean)
# print(arr.std)
# print(arr.variance)

# nums = []
# new_arr = StatArray(nums)
# print(new_arr.mean)
# print(new_arr.std)
# print(new_arr.variance)

# with StatArray([1,2,3,4,5]) as stats:
#     print(stats.mean)
#     print(stats.std)
#     print(stats.variance)


data = [6,3,7,9,1,4]
arr = StatArray(data)
# print(arr.mean)
print(arr.median)