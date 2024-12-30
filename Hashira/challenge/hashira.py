import hashlib
import os
import sys

def md5hash(filename):
	return hashlib.md5(open(filename, "rb").read()).hexdigest()

folder_path = sys.argv[1]
hashes = []

for filename in os.listdir(folder_path):
	file_path = os.path.join(folder_path, filename)
	if os.path.isfile(file_path):
		hashes.append(md5hash(file_path))

with open("/home/ctf/backup", "w") as f:
	f.write("\n".join(hashes))

