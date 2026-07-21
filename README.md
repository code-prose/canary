## Canary

This project aims to solve your PII concerns.
Parse logs at volume to catch leaked PII such as SSNs, Addresses, etc. within your orgs logs.


## 2 Pass Approach
- Fast Reject
    Leverages SIMD comparison to quickly scan and reject lines that don't have any signs of PII
- Slow Pass
    Uses more computation expensive methods (i.e. Regex) to comprehensively detect leaked PII

Should I bench this?
