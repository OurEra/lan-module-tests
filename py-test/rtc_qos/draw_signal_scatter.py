import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime

def draw_signal(SIGNAL_DICT_BY_TIME):
  figure, ax_signal = plt.subplots(1, 1)

  # yaxis give value 10
  ts_x = []
  value_y = []
  for event_ts, event_desc in SIGNAL_DICT_BY_TIME.items():
    ts_x.append(datetime.strptime(event_ts, '%Y-%m-%d %H:%M:%S'))
    value_y.append(10)

  ax_signal.scatter(ts_x, value_y)
  locator = mdates.AutoDateLocator()
  formatter = mdates.ConciseDateFormatter(locator)
  ax_signal.xaxis.set_major_locator(locator)
  ax_signal.xaxis.set_major_formatter(formatter)

  #plt.text(4, 4, 'Here you are')
  plt.xticks(rotation=30)
  plt.show()
