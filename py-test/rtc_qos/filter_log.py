# python3
# python3 filter_log_and_plt.py qos_log.txt [track_id_1...]
import json
import sys
import random
import numpy
from datetime import datetime
from termcolor import colored
import draw_qos_plot
import draw_signal_scatter

# organize media qos and ready to plot
def organize_media_qos(qos):
    track_stats = qos['item']['track_stats']
    for track_stat in track_stats:
      track_id = track_stat['track_id']
      # add timestamp in track_stats for plot convenient
      track_stat['timestamp'] = qos['item']['timestamp']
      if track_id in TRACK_QOS_LINE_DICT:
        qos_list = TRACK_QOS_LINE_DICT.get(track_id)
        qos_list.append(track_stat)
      else:
        qos_list = [track_stat]
        TRACK_QOS_LINE_DICT[track_id] = qos_list

# IDLE,
# CONNECTING,
# CONNECTED,
# RECONNECTING,
# RECONNECTED,
def convert_room_name(id):
    if id == 0:
        return 'IDLE'
    elif id == 1:
        return 'CONNECTING'
    elif id == 2:
        return 'CONNECTED'
    elif id == 3:
        return 'RECONNECTING'
    elif id == 4:
        return 'RECONNECTED'

# NEW,
# CHECKING,
# CONNECTED,
# COMPLETED,
# FAILED,
# DISCONNECTED,
# CLOSED;
def convert_ice_state(id, type):
    pc = 'PUB'
    if type == 0:
        pc = 'PUB'
    elif type == 1:
        pc = 'SUB'

    if id == 0:
        return pc + ' NEW'
    elif id == 1:
        return pc + ' CHECKING'
    elif id == 2:
        return pc + ' CONNECTED'
    elif id == 3:
        return pc + ' COMPLETED'
    elif id == 4:
        return pc + ' FAILED'
    elif id == 5:
        return pc + ' DISCONNECTED'
    elif id == 6:
        return pc + ' CLOSED'

orignal_log = open(sys.argv[1], "r", encoding='utf-8')
plot_track_choose = []
if len(sys.argv) > 2:
    plot_track_choose = sys.argv[2:]

# global media qos line dict by track id
DUMP_TRACK_QOS = False
TRACK_QOS_LINE_DICT = {}

SIGNAL_DICT_BY_TIME = {}

lines = []
for line in orignal_log:
    json_content = json.loads(line)
    try:
        checkitem = json_content['item']['timestamp']
        lines.append(json_content)
    except KeyError:
        pass
    #print (json.dumps(json_content, indent=4))

# https://stackoverflow.com/questions/26924812/python-sort-list-of-json-by-value
lines = sorted(lines, key=lambda k: int(k['item']['timestamp']))
for line in lines:
    event = line['item']['event_name']
    rdate = datetime.fromtimestamp(line['item']['timestamp'] / 1000).strftime('%Y-%m-%d %H:%M:%S')
    platform = line['base']['os_platform']
    if line['item']['event_id'] == 15:
        ice = convert_ice_state(line['item']['state'], line['item']['pc_type'])
        SIGNAL_DICT_BY_TIME[rdate] =  ice
        print (colored(rdate + " : " + platform + " : " + event + " : " + ice, 'red'))
    elif line['item']['event_id'] == 18:
        room = convert_room_name(line['item']['room_state'])
        SIGNAL_DICT_BY_TIME[rdate] =  room
        print (colored(rdate + " : " + platform + " : " + event + " : " + room, 'red'))
    elif line['item']['event_id'] == 30:
        organize_media_qos(line)
    else:
        SIGNAL_DICT_BY_TIME[rdate] =  event
        print (colored(rdate + " : " + platform + " : " + event, 'red'))

orignal_log.close()

if DUMP_TRACK_QOS:
  print (colored('\nDUMP TRACK QOS', 'red'))
  for key, value in TRACK_QOS_LINE_DICT.items():
    len_val = len(value)
    print ('count ' + str(len_val) + ', ' + key)
    print (value)

draw_qos_plot.draw_media_qos(TRACK_QOS_LINE_DICT, plot_track_choose)
draw_signal_scatter.draw_signal(SIGNAL_DICT_BY_TIME)