# pwonounceable
(CLI) tools for generating human-pronounceable passwords.

## How use?
So far the only tool here is **3pw** — based off of [Tom Van Vleck's `gpw` program](https://multicians.org/thvv/gpw.html).
1. If you have `gcc` and `bash` handy, you should be able to build by simply running `./make.sh`.
  - Everything here is ANSI C compliant, so you _should_ also be able to compile `3pw.c` with virtually any other compiler.
2. Run the built binary from any TTY.
3. Current optional arguments are documented in the header of the main `3pw.c` file.

## 3pw example output
```bash
# Default: 9 characters, case = 1 (pascal)
# ./bin/3pw
UlpOrtHen
RagEnsOrt
EekSioNic

# 15 characters, Default: case = 1 (pascal)
# ./bin/3pw 15
EnsLumIcoPolDse
AchEdmAndOndUmp
EscRunTatErmEte

# No pascal case - vanilla gpw
# ./bin/3pw 6 0
meaggi
imbort
etisma
```
