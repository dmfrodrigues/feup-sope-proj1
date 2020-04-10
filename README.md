# simpledu

<p align="left">
  <img src="https://github.com/dmfrodrigues/feup-sope-proj1/workflows/test/badge.svg">
</p>

- **Project name:** simpledu
- **Short description:** Clone of Unix command `du` (estimate file space usage), supporting most relevant options
- **Institution:** FEUP
- **Course:** SOPE (Operating Systems)
- **Group members:**
    - Diogo Miguel Ferreira Rodrigues (diogo.rodrigues@fe.up.pt / up201806429@fe.up.pt)
    - João António Cardoso Vieira e Basto de Sousa (up201806613@fe.up.pt)
    - Rafael Soares Ribeiro (up201806330@fe.up.pt)

Estimates file space usage.

For each subdirectory, creates a new process and communicates via pipes.

## Make commands
### Compile

```sh
make
```

### Test

```sh
make test
```

### Cleanup

```sh
make clean
```

### Create ZIP file

```sh
make zip
```

### Test ZIP file contents

```sh
make testzip
```

## User Commands

```sh
./simpledu [OPTION]... [FILE]...
```

Supports the following options, with behaviour similar to `du`:
- `-a, --all`
- `--apparent-size`
- `-B, --block-size=SIZE`
- `-l, --count-links` (activated by default, cannot be deactivated)
- `-L, --dereference`
- `-S, --separate-dirs`
- `-d, --max-depth=N`

May receive as many files as wanted in `[FILE]...`.
