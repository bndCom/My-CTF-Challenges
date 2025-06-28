# tshark -r challenge/capture.pcapng -Y '!(http.user_agent contains "sqlmap/1.9.4") and http.request and frame.number >= 95960 and frame.number <= 96851' -T fields -e "frame.time_epoch" | cut -d "." -f1 | uniq > time.txt


with open("time.txt", "r") as f:
    ts = f.read().splitlines()

flag = ""

for i, c in enumerate(ts):
    if i != 0:
        flag += chr(int(c) - int(ts[i - 1]))

print(flag)
