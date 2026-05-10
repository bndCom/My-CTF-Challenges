# right-there - Forensics

## Quick Overview
- A VNC remote access to an infected machine has been provided. We are asked to investigate a cyber attack that took place.

## Solution
- Log files and command history are not useful in this case. Further investigation requires checking for remaining attack artifacts on the system and any persistence mechanisms.
- One common persistence technique is cron jobs, which is the case here.
A non-standard cron job downloads and executes a malware stage: `bash /tmp/.stage1.sh /lib/systemd/system` (the stage no longer exists locally or online).
- The directory passed as an argument is significant and used by the stage. Several possibilities exist, such as modifying service files or hiding files, but the method used here is hiding malicious payloads in file attributes within the directory, documented in MITRE ATT&CK as technique `T1564.014`.
Extract the payload using the `getfattr` command; the first part of the flag is in the extracted payload.
- The payload attempts to download another malware stage that doesn't exist locally. However, a copy exists in the Internet Archive (Wayback Machine), which is often useful for such investigations.

## Flag
`mctf{JU$t_s33K_AND_hiD3_In_4_nEW_3rA}`

