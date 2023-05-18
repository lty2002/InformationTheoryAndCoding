import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# 设置新罗马字体
font = FontProperties(fname='/System/Library/Fonts/Times.ttc', size=14)
plt.rcParams['font.family'] = font.get_name()
plt.rcParams['font.size'] = font.get_size()


# 二类信息熵
def binary_entropy(p):
    return -(p * np.log2(p) + (1 - p) * np.log2(1 - p))


# 二类信息熵图像
p1 = np.linspace(0 + 1e-10, 1 - 1e-10, 50)
entropy = binary_entropy(p1)

plt.plot(p1, entropy, '-')
plt.xlabel('p')
plt.ylabel('H(p)')
plt.show()
