# Hashira

## Write-up

- The challenge description says that they have been regularly doing integrity checks, which lead us to check cronjobs or `.bashrc` file for example, we can find in it `#python3 /opt/hashira.py /usr/bin`

- The python script is getting the **md5 hash** of every file within specific folder specified in the arguments, which is `/usr/bin`, and then the hashes are saved to `/home/ctf/backup`.

- If this backup was taken before the attack, and they have probably left a trojan or backdoor in the `/usr/bin` directory, we can detect which file has been added or modified using the previous hashes list.

- This script [script.py](find-malware.py) shows that a file hash is not within the backup which means that this file was modified after the compromise and most likely it was targeted, the file is `/usr/bin/ucf`

- We can find at the end of the script a command, which is the flag encoded in base32 ( it could be reverse shell or something like that in real scenarios ).


## Flag

`shellmates{hashIr4_4r3_mAlW4Re_sL4y3r}`
