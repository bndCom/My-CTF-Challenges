# OSQ Revenge

## Write-up

- The challenge gives us an `osquery` prompt. This tool is used to gather information from a host using SQL queries. We have to investigate a cyber attack using osquery and find the flag parts as follows:  
    1st and 2nd: foothold  
    3rd and 4th: persistence  
    5th: lateral movement  
    6th: privilege escalation  

### First part:
Listing the processes shows that there is an **Apache web server** running on the host. The main page is `/var/www/html/index.html`. It is interesting to inspect it to understand more about the initial access. Since we can't read arbitrary files on the host using osquery, it is possible to send HTTP requests: `select * from curl where url = 'http://127.0.0.1/index.html'`. Here we find the first part of the flag: `shellmates{W3B_Lf1_2Rc3`.

### Second part:
The code shows that the app is vulnerable to **LFI to RCE**, meaning that a `php` webshell can be uploaded to `/upl04d` and used to get an RCE. Discovering that folder shows that it contains a PHP webshell. Its name is the second part of the flag: `_4fO0THoLd`.

### Third and fourth part:
These two parts are about persistence and two very relevant techniques:
- The first one is using a cron job, used to execute a file in the `sysadmin` home directory. The file name is the part of the flag: `_CR0N&_`.
- The second one considers SSH keys used to maintain access on the system. Getting SSH keys is possible with the following query: 
```sql
SELECT u.username, a.algorithm, a.key, a.comment, a.key_file
FROM authorized_keys a
JOIN users u
ON a.uid = u.uid
WHERE u.username = 'sysadmin';
```
The key is the base64 of the fourth part of the flag: `$SH_Ke3p_U_In`.

### Fifth part:
This one considers a lateral movement technique. The hint says: `MiTM` (Man in The Middle attack). In this case, it is **ARP spoofing** or **ARP cache poisoning** technique, which is done by associating an attacker's MAC address to a target IP address, making it possible to intercept packets sent to that IP. Checking ARP cache using osquery shows the following MAC addresses:
```
de:ad:be:ef:ff:ff 
5f:34:72:70:5f:61 
92:0f:ec:94:7e:67 
```
The second address is the fifth part of the flag: `_4rp_a`.  
The first one is `deadbeef:ff:ff`, which is used to get the attention of the player because it is not very relevant to have a part of the flag as a MAC address. Many clues were given to make this part not guessy (sorry about it xd).

### Sixth part:
The last one is about privilege escalation. One relevant `sudo` technique allows a normal user on the system to execute commands on the host as `root`. To make this possible, an entry must be added to the file `/etc/sudoers`. It can be shown using the query: `select * from sudoers;`. The last part is the name of the file to be executed as root by the user `sysadmin` => `Nd_Sud0_4ur_amB1TiOn}`.

## Flag
`shellmates{W3B_Lf1_2Rc3_4fO0THoLd_CR0N&_$SH_Ke3p_U_In_4rp_aNd_Sud0_4ur_amB1TiOn}`