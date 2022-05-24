# pwonounceable
CLI tools for generating human-pronounceable passwords, or, pseudo-randomized, general-purpose strings that a human could (pwobably) pronounce.

## How use?
So far, the only tool here is **3pw** — based off of [Tom Van Vleck's `gpw` program](https://multicians.org/thvv/gpw.html).
1. If you have `gcc` and `bash` handy, you should be able to build by simply running `./make.sh`.
    - *Everything here is tested against ANSI C, so you _should_ be able to compile with virtually any compiler in addition to GCC (which I used).*
2. Run the built binary from any TTY.
    - *Note that if you run and build with WSL, you will only be able to run the binary from a WSL TTY.  The reverse tends not to be true.*
3. The current commandline arguments are documented in the header of the `main_*.c` file(s).

## 3pw example output
```bash
## generate 9-letter strings—
## —25 of them—
## —capitalize on 6-letter word boundaries—
## oh, and with the reproducable seed "pandora":
# ./bin/3pw 9 25 6 pandora
MaticaTea
SionehAnh
HniancEnt
NgenseRtu
HassusTri
OdherpTut
AgicinGen
HentanNit
AvalumFul
OnatenSgs
GarialIgh
EckinoUni
ArbereLio
IounchRit
OstaphEns
LastitAte
IminelLet
IngeopYll
HeaditIqu
IciallEnt
ReolonSil
LodoliGin
ErumbiNju
TtabowAde
RowdraYbe
```

```bash
## generate 11-letter strings—
## —the default number of them—
## —don't capitalize—
## again with the seed "pandora":
# ./bin/3pw 11 - 0 pandora
maticateast
aminallfade
omeleardaml
meristraten
odherptutle
nnourtaille
cychmerruis
epinfildery
```

```bash
## generate 7-letter strings—
## —the default number of them—
## —capitalize first letter only—
## with no given seed:
# ./bin/3pw 7 - 7
Tereona
Gretrop
Atimiti
Ordamin
Ecketra
Kearamp
Ticable
Eldenwa
```
