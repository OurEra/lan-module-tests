import json
import sys
from datetime import datetime
from termcolor import colored

orignal_log = open(sys.argv[1], "r", encoding='utf-8')

# filter out MediaStatistics item
FILTER_OUT_MEDIA = True

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
        print (colored(rdate + " : " + platform + " : " + event + " : " + ice, 'red'))
    elif line['item']['event_id'] == 18:
        room = convert_room_name(line['item']['room_state'])
        print (colored(rdate + " : " + platform + " : " + event + " : " + room, 'red'))
    elif FILTER_OUT_MEDIA and line['item']['event_id'] == 30:
        pass
    else:
        print (colored(rdate + " : " + platform + " : " + event, 'red'))

    if FILTER_OUT_MEDIA and line['item']['event_id'] == 30:
        pass
    else:
        print (line)

orignal_log.close()
