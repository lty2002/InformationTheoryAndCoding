import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# 设置新罗马字体
font = FontProperties(fname='/System/Library/Fonts/Times.ttc', size=14)
plt.rcParams['font.family'] = font.get_name()
plt.rcParams['font.size'] = font.get_size()


def uniform_quantization(signal, level_num):
    max_val = np.max(np.abs(signal))
    min_val = np.min(np.abs(signal))
    step_size = (max_val - min_val) / (level_num - 1)

    quantized = list(
        map(lambda x: (np.round((np.abs(x) - min_val) / step_size) * step_size + min_val) * (1 if x >= 0 else -1),
            origin_signal))

    length = int(np.log2(level_num)) + 1
    template = '%0' + str(length - 1) + 'd'
    coding = list(map(lambda x: ('1' if x >= 0 else '0') + (template % int(bin(int(abs(x)))[2:])),
                      range(-level_num + 1, level_num)))
    # print(coding)

    return quantized, coding


def figure(quantized_signal, delta_reciprocal, coding):
    plt.figure(figsize=(9, 6))
    plt.plot(origin_signal, 'b.-', quantized_signal, 'r.-')
    plt.title('Quantized Signal (Uniform Quantization)')
    plt.legend(['origin_signal', 'quantized_signal'])
    plt.xticks(range(len(origin_signal))[::len(quantized_signal) // 10])
    plt.yticks(np.linspace(-np.max(quantized_signal), np.max(quantized_signal), delta_reciprocal * 2 - 1)[5:],
               labels=coding[5:])
    plt.grid()
    plt.show()


if __name__ == '__main__':
    x = np.linspace(-2.5, 7.5, 120)
    y = 0.08 * x ** 2 + np.sin(2 * x) - np.cos(3 * x)
    origin_signal = y

    delta_reciprocal = 8
    quantized_signal, coding = uniform_quantization(origin_signal, delta_reciprocal)

    figure(quantized_signal, delta_reciprocal, coding)
