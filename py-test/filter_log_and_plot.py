import json
from datetime import datetime
from termcolor import colored

orignal_log = open("test-json.txt", "r", encoding='utf-8')

lines = []
for line in orignal_log:
    json_content = json.loads(line)
    if 'item' in json_content:
        lines.append(json_content)
    print (json.dumps(json_content, indent=4))

# https://stackoverflow.com/questions/26924812/python-sort-list-of-json-by-value
lines = sorted(lines, key=lambda k: k['item']['timestamp'])
for line in lines:
    print (colored(line['item']['event_name'] + " : " + datetime.utcfromtimestamp(line['item']['timestamp'] / 1000).strftime('%Y-%m-%d %H:%M:%S'), 'red'))
    print (line)

orignal_log.close()
