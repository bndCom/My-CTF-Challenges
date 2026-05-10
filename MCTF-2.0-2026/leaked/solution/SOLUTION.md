# leaked - Forensics/CTI

## Overview
The description is clear: 'ServiceMaster Clean Services Group' has been attacked and their data leaked. Our job is to find which data has been exposed—proof will be a the hash of a specific within the leaked data.

## Solution
- Searching online for attacks related to the company reveals many [results](https://www.ransomware.live/id/U2VydmljZU1hc3RlciBDbGVhbiBzZXJ2aWNlc0Bha2lyYQ). They show that this group was targeted by the ransomware group `Akira`.
- Ransomware groups typically post data leaks on their websites and expose them publicly. In cyber threat intelligence, it's important to know which data is shared publicly. To verify which data has been leaked, visit the official Akira website on the [dark web](https://www.watchguard.com/wgrd-security-hub/ransomware-tracker/akira).
- The website confirms that they exposed data for the company (approximately 4GB). The target file is located in the third part of the data.

## Flag
`mctf{99744fd1c94a3437d9ff6f89caea3d24}`