
import threading
import time

# while loop
count = 10
while count > 0 :
    count = count -1
    print("count ", count)

# threading https://pymotw.com/2/threading/
def target_work():
    print ("in work")
    time.sleep(1)
    print("work over")

t = threading.Thread(target=target_work)
# t.setDaemon(True)
t.start()