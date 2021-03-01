import matplotlib.colors as mcolors
from datetime import datetime
from termcolor import colored
from matplotlib import pyplot as plt

def draw_media_qos(TRACK_QOS_LINE_DICT, plot_track_choose):
  figure, (ax_bps, ax_fps) = plt.subplots(2, 1)
  choose_colors = list(mcolors.TABLEAU_COLORS.keys())
  color_index = 0
  for track_id, track_stat in TRACK_QOS_LINE_DICT.items():
    track_ts_x = []
    track_kbps_y = []
    track_fps_y = []
    kind = 'VIDEO'
    direct = 'PUB'
    for one in track_stat:
        rdate = datetime.fromtimestamp(one['timestamp'] / 1000).strftime('%Y-%m-%d %H:%M:%S')
        if 'bps' not in one:
          print (colored("BAD FORMAT: " + str(one), 'red'))
        else:
          track_ts_x.append(datetime.strptime(rdate, '%Y-%m-%d %H:%M:%S'))
          track_kbps_y.append(one['bps'] / 1000)
          kind = one['kind']
          if kind.upper() == 'VIDEO':
            track_fps_y.append(one['framerate'])
          if one['extra_stats']['bytes_sent'] > 0:
             direct = 'PUB'
          else:
             direct = 'SUB'
  
    track_label = direct + ":" + kind + ':' + track_id
    # check user choose to plot
    should_plot = False
    if len(plot_track_choose) > 0 and track_id in plot_track_choose:
        should_plot = True
    elif len(plot_track_choose) == 0:
        should_plot = True
  
    # plot birate
    if should_plot and len(track_kbps_y) > 0:
      ax_bps.set(xlabel='date', ylabel='kbps')
      ax_bps.plot(track_ts_x, track_kbps_y, color=choose_colors[color_index], label=track_label)
      ax_bps.legend()
    # plot framerate
    if should_plot and len(track_fps_y) > 0:
      ax_fps.set(xlabel='date', ylabel='fps')
      ax_fps.plot(track_ts_x, track_fps_y, color=choose_colors[color_index], label=track_label)
      ax_fps.legend()
    color_index = (color_index + 1)  % len(choose_colors)
    print ('bps count ' + str(len(track_stat)) + ', fps count ' + str(len(track_fps_y)) + ', ' + track_id + ', ' + kind + ', ' + direct)
  
  # plot subplots
  if len(TRACK_QOS_LINE_DICT) > 0:
    plt.xticks(rotation=30)
    plt.show()
